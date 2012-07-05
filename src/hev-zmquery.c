/*
 ============================================================================
 Name        : hev-zmquery.c
 Author      : Heiher <admin@heiher.info>
 Version     : 0.0.1
 Copyright   : Copyright (c) 2010 everyone.
 Description : Zhengma decoder
 ============================================================================
 */

#include <stdio.h>
#include <sqlite3.h>

#include "hev-zmquery.h"
#include "hev-main.h"

#define HEV_ZMQUERY_DB_FILE_PATH		"./res/zhengma.db"

#define HEV_ZMQUERY_GET_PRIVATE(obj)	(G_TYPE_INSTANCE_GET_PRIVATE((obj), HEV_TYPE_ZMQUERY, HevZmQueryPrivate))

typedef struct _HevZmQueryPrivate HevZmQueryPrivate;

struct _HevZmQueryPrivate
{
	sqlite3 * sqlite_db;
};

static void hev_zmquery_initable_iface_init(GInitableIface * iface);
static gboolean hev_zmquery_initable_init(GInitable * initable,
					GCancellable * cancellable, GError ** error);
static int hev_zmquery_db_exec_handler(void * p, int argc, char * argv[], char * colname[]);

G_DEFINE_TYPE_WITH_CODE(HevZmQuery, hev_zmquery, G_TYPE_OBJECT,
		G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE, hev_zmquery_initable_iface_init));

static void hev_zmquery_dispose(GObject * obj)
{
	HevZmQuery * self = HEV_ZMQUERY(obj);
	HevZmQueryPrivate * priv = HEV_ZMQUERY_GET_PRIVATE(self);

	G_OBJECT_CLASS(hev_zmquery_parent_class)->dispose(obj);
}

static void hev_zmquery_finalize(GObject * obj)
{
	HevZmQuery * self = HEV_ZMQUERY(obj);
	HevZmQueryPrivate * priv = HEV_ZMQUERY_GET_PRIVATE(self);

	/* sqlite_db START */
	if(priv->sqlite_db)
	{
		sqlite3_close(priv->sqlite_db);

		priv->sqlite_db = NULL;
	}
	/* sqlite_db END */

	G_OBJECT_CLASS(hev_zmquery_parent_class)->finalize(obj);
}

static GObject * hev_zmquery_constructor(GType type, guint n, GObjectConstructParam * param)
{
	return G_OBJECT_CLASS(hev_zmquery_parent_class)->constructor(type, n, param);
}

static void hev_zmquery_constructed(GObject * obj)
{
}

static void hev_zmquery_class_init(HevZmQueryClass * klass)
{
	GObjectClass * obj_class = G_OBJECT_CLASS(klass);

	obj_class->constructor = hev_zmquery_constructor;
	obj_class->constructed = hev_zmquery_constructed;
	obj_class->dispose = hev_zmquery_dispose;
	obj_class->finalize = hev_zmquery_finalize;

	g_type_class_add_private(klass, sizeof(HevZmQueryPrivate));
}

static void hev_zmquery_initable_iface_init(GInitableIface * iface)
{
	iface->init = hev_zmquery_initable_init;
}

static void hev_zmquery_init(HevZmQuery * self)
{
	HevZmQueryPrivate * priv = HEV_ZMQUERY_GET_PRIVATE(self);
}

static gboolean hev_zmquery_initable_init(GInitable * initable,
					GCancellable * cancellable, GError ** error)
{
	HevZmQuery * self = HEV_ZMQUERY(initable);
	HevZmQueryPrivate * priv = HEV_ZMQUERY_GET_PRIVATE(self);

	/* sqlite_db START */
	{
		int rc = 0;
		gchar *path = NULL;

		path = hev_build_res_path(HEV_ZMQUERY_DB_FILE_PATH);

		rc = sqlite3_open(path, &priv->sqlite_db);
		g_free(path);
		if(rc)
		{
			g_critical("Open db file failed!");
			sqlite3_close(priv->sqlite_db);
			priv->sqlite_db = NULL;

			return FALSE;
		}
	}
	/* sqlite_db END */

	return TRUE;
}

GObject * hev_zmquery_new(GCancellable * cancellable, GError ** error)
{
	return g_initable_new(HEV_TYPE_ZMQUERY, cancellable, error, NULL);
}

gint hev_zmquery_lookup(HevZmQuery * self, const char * const keyword, HevZmQueryLookupHandler handler, gpointer data)
{
	HevZmQueryPrivate * priv = NULL;
	GString * sql_str = NULL;
	gint rc = 0;
	char * errmsg = NULL;
	gpointer pdata[2] = {handler, data};

	g_return_val_if_fail(HEV_IS_ZMQUERY(self), -1);
	g_return_val_if_fail(handler, -1);

	priv = HEV_ZMQUERY_GET_PRIVATE(self);
	sql_str = g_string_new(NULL);

	g_string_printf(sql_str, "SELECT phrase, mlen, m0, m1, m2, m3 FROM phrases WHERE phrase = '%s';", keyword);
	rc = sqlite3_exec(priv->sqlite_db, sql_str->str, hev_zmquery_db_exec_handler, pdata, &errmsg);
	if(rc)
	{
		g_critical("%s", errmsg);
		sqlite3_free(errmsg);
	}

	g_string_free(sql_str, TRUE);

	return 0;
}

static int hev_zmquery_db_exec_handler(void * p, int argc, char * argv[], char * colname[])
{
	gpointer * pdata = p;
	HevZmQueryLookupHandler handler = (HevZmQueryLookupHandler)pdata[0];
	gpointer data = pdata[1];
	GString * keyword = g_string_new(argv[0]);
	GString * value = g_string_new(NULL);
	int rc = 0;

	switch(argv[1][0])
	{
	case '1':
		{
			int m0 = 0;

			sscanf(argv[2], "%d", &m0);
			g_string_printf(value, "%c", m0+96);
			break;
		}
	case '2':
		{
			int m0 = 0, m1 = 0;

			sscanf(argv[2], "%d", &m0);
			sscanf(argv[3], "%d", &m1);
			g_string_printf(value, "%c%c", m0+96, m1+96);
			break;
		}
	case '3':
		{
			int m0 = 0, m1 = 0, m2 = 0;

			sscanf(argv[2], "%d", &m0);
			sscanf(argv[3], "%d", &m1);
			sscanf(argv[4], "%d", &m2);
			g_string_printf(value, "%c%c%c", m0+96, m1+96, m2+96);
			break;
		}
	case '4':
		{
			int m0 = 0, m1 = 0, m2 = 0, m3 = 0;

			sscanf(argv[2], "%d", &m0);
			sscanf(argv[3], "%d", &m1);
			sscanf(argv[4], "%d", &m2);
			sscanf(argv[5], "%d", &m3);
			g_string_printf(value, "%c%c%c%c", m0+96, m1+96, m2+96, m3+96);
			break;
		}
	default:
		break;
	}

	rc = handler(keyword, value, data);
	g_string_free(keyword, TRUE);
	g_string_free(value, TRUE);

	return rc;
}
