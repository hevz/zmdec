/*
 ============================================================================
 Name        : hev-zmdec.c
 Author      : Heiher <admin@heiher.info>
 Version     : 0.0.1
 Copyright   : Copyright (c) 2010 everyone.
 Description : Zhengma Deocder
 ============================================================================
 */

#include <gtk/gtk.h>

#include "hev-zmdec.h"
#include "hev-zmquery.h"

enum
{
	COLUMN_KEYWORD,
	COLUMN_VALUE,
	NUM_COLUMNS
};

#define HEV_ZMDEC_GET_PRIVATE(obj)	(G_TYPE_INSTANCE_GET_PRIVATE((obj), HEV_TYPE_ZMDEC, HevZmDecPrivate))

typedef struct _HevZmDecPrivate HevZmDecPrivate;

struct _HevZmDecPrivate
{
	GObject * zmquery;
	GtkWidget * main_window;
	GtkWidget * keyword_entry;
	GtkWidget * lookup_button;
	GtkListStore * result_liststore;
};

static void hev_zmdec_initable_iface_init(GInitableIface * iface);
static gboolean hev_zmdec_initable_init(GInitable * initable,
					GCancellable * cancellable, GError ** error);

static gboolean hev_zmdec_ui_init(HevZmDec * self);
static void hev_zmdec_lookup_button_clicked_handler(GtkButton * button, gpointer data);
static void hev_zmdec_keyword_entry_activate_handler(GtkEntry * entry, gpointer data);
static gint hev_zmdec_lookup_handler(const GString * const keyword, const GString * const value, gpointer data);

G_DEFINE_TYPE_WITH_CODE(HevZmDec, hev_zmdec, G_TYPE_OBJECT,
		G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE, hev_zmdec_initable_iface_init));

static void hev_zmdec_dispose(GObject * obj)
{
	HevZmDec * self = HEV_ZMDEC(obj);
	HevZmDecPrivate * priv = HEV_ZMDEC_GET_PRIVATE(self);

	/* zmquery START */
	if(G_IS_OBJECT(priv->zmquery))
	{
		g_object_unref(priv->zmquery);

		priv->zmquery = NULL;
	}
	/* zmquery END */

	G_OBJECT_CLASS(hev_zmdec_parent_class)->dispose(obj);
}

static void hev_zmdec_finalize(GObject * obj)
{
	HevZmDec * self = HEV_ZMDEC(obj);
	HevZmDecPrivate * priv = HEV_ZMDEC_GET_PRIVATE(self);

	G_OBJECT_CLASS(hev_zmdec_parent_class)->finalize(obj);
}

static GObject * hev_zmdec_constructor(GType type, guint n, GObjectConstructParam * param)
{
	return G_OBJECT_CLASS(hev_zmdec_parent_class)->constructor(type, n, param);
}

static void hev_zmdec_constructed(GObject * obj)
{
}

static void hev_zmdec_class_init(HevZmDecClass * klass)
{
	GObjectClass * obj_class = G_OBJECT_CLASS(klass);

	obj_class->constructor = hev_zmdec_constructor;
	obj_class->constructed = hev_zmdec_constructed;
	obj_class->dispose = hev_zmdec_dispose;
	obj_class->finalize = hev_zmdec_finalize;

	g_type_class_add_private(klass, sizeof(HevZmDecPrivate));
}

static void hev_zmdec_initable_iface_init(GInitableIface * iface)
{
	iface->init = hev_zmdec_initable_init;
}

static void hev_zmdec_init(HevZmDec * self)
{
	HevZmDecPrivate * priv = HEV_ZMDEC_GET_PRIVATE(self);
}

static gboolean hev_zmdec_initable_init(GInitable * initable,
					GCancellable * cancellable, GError ** error)
{
	HevZmDec * self = HEV_ZMDEC(initable);
	HevZmDecPrivate * priv = HEV_ZMDEC_GET_PRIVATE(self);

	/* zmquery START */
	priv->zmquery = hev_zmquery_new(NULL, NULL);
	if(FALSE == HEV_IS_ZMQUERY(priv->zmquery))
	{
		return FALSE;
	}
	/* zmquery END */

	if(FALSE == hev_zmdec_ui_init(HEV_ZMDEC(initable)))
	{
		g_object_unref(priv->zmquery);
		priv->zmquery = NULL;

		return FALSE;
	}

	return TRUE;
}

GObject * hev_zmdec_new(GCancellable * cancellable, GError ** error)
{
	return g_initable_new(HEV_TYPE_ZMDEC, cancellable, error, NULL);
}

static gboolean hev_zmdec_ui_init(HevZmDec * self)
{
	HevZmDecPrivate * priv = HEV_ZMDEC_GET_PRIVATE(self);

	/* main window START */
	{
		priv->main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		if(FALSE == GTK_IS_WINDOW(priv->main_window))
		{
			return FALSE;
		}

		gtk_window_set_title(GTK_WINDOW(priv->main_window), "郑码查询器");
		gtk_window_set_icon_from_file(GTK_WINDOW(priv->main_window), "./res/zhengma.svg", NULL);
		gtk_window_resize(GTK_WINDOW(priv->main_window), 400, 280);
		gtk_window_set_position(GTK_WINDOW(priv->main_window), GTK_WIN_POS_CENTER_ALWAYS);
		g_signal_connect(G_OBJECT(priv->main_window), "destroy",
				G_CALLBACK(gtk_main_quit), NULL);
	}
	/* main window END */

	/* vbox START */
	{
		GtkWidget * vbox = NULL;

		vbox = gtk_vbox_new(FALSE, 0);
		gtk_container_set_border_width(GTK_CONTAINER(vbox), 4);
		gtk_container_add(GTK_CONTAINER(priv->main_window), vbox);

		/* hbox START */
		{
			GtkWidget * hbox = NULL;

			hbox = gtk_hbox_new(FALSE, 0);
			gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);

			/* keyword entry START */
			priv->keyword_entry = gtk_entry_new();
			g_signal_connect(G_OBJECT(priv->keyword_entry), "activate",
						G_CALLBACK(hev_zmdec_keyword_entry_activate_handler), self);
			gtk_box_pack_start(GTK_BOX(hbox), priv->keyword_entry, TRUE, TRUE, 0);
			/* keyword entry END */

			/* lookup button START */
			priv->lookup_button = gtk_button_new_with_label("查  询");
			g_signal_connect(G_OBJECT(priv->lookup_button), "clicked",
					G_CALLBACK(hev_zmdec_lookup_button_clicked_handler), self);
			gtk_box_pack_start(GTK_BOX(hbox), priv->lookup_button, FALSE, TRUE, 0);
			/* lookup button END */
		}
		/* hbox END */

		/* scrolled window START */
		{
			GtkWidget * scrolled_window = NULL;

			scrolled_window = gtk_scrolled_window_new(NULL, NULL);
			gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
					GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
			gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolled_window),
					GTK_SHADOW_IN);
			gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);

			/* result treeview START */
			{
				GtkWidget * tree_view = NULL;
				GtkCellRenderer * renderer = NULL;
				GtkTreeViewColumn * column = NULL;

				priv->result_liststore = gtk_list_store_new(NUM_COLUMNS,
						G_TYPE_STRING, G_TYPE_STRING);
				tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(priv->result_liststore));

				renderer = gtk_cell_renderer_text_new();
				column = gtk_tree_view_column_new_with_attributes("关键词",
																 renderer,
																 "text",
																 COLUMN_KEYWORD,
																 NULL);
				gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

				renderer = gtk_cell_renderer_text_new();
				column = gtk_tree_view_column_new_with_attributes("编码值",
																 renderer,
																 "text",
																 COLUMN_VALUE,
																 NULL);
				gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

				gtk_container_add(GTK_CONTAINER(scrolled_window), tree_view);
			}
			/* result treeview END */
		}
		/* scrolled window END */
	}
	/* vbox END */

	gtk_widget_show_all(priv->main_window);

	return TRUE;
}

static void hev_zmdec_lookup_button_clicked_handler(GtkButton * button, gpointer data)
{
	HevZmDec * self = HEV_ZMDEC(data);
	HevZmDecPrivate * priv = HEV_ZMDEC_GET_PRIVATE(self);
	const char * keyword = gtk_entry_get_text(GTK_ENTRY(priv->keyword_entry));

	if(0 == gtk_entry_get_text_length(GTK_ENTRY(priv->keyword_entry)))
	{
		gtk_list_store_clear(GTK_LIST_STORE(priv->result_liststore));
	}

	hev_zmquery_lookup(HEV_ZMQUERY(priv->zmquery), keyword, hev_zmdec_lookup_handler, data);
}

static void hev_zmdec_keyword_entry_activate_handler(GtkEntry * entry, gpointer data)
{
	HevZmDec * self = HEV_ZMDEC(data);
	HevZmDecPrivate * priv = HEV_ZMDEC_GET_PRIVATE(self);

	gtk_button_clicked(GTK_BUTTON(priv->lookup_button));
}

static gint hev_zmdec_lookup_handler(const GString * const keyword, const GString * const value, gpointer data)
{
	HevZmDec * self = HEV_ZMDEC(data);
	HevZmDecPrivate * priv = HEV_ZMDEC_GET_PRIVATE(self);
	GtkTreeIter tree_iter = { 0 };

	gtk_list_store_append(GTK_LIST_STORE(priv->result_liststore), &tree_iter);
	gtk_list_store_set(GTK_LIST_STORE(priv->result_liststore), &tree_iter,
			COLUMN_KEYWORD, keyword->str, COLUMN_VALUE, value->str, -1);

	return 0;
}
