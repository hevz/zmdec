/*
 ============================================================================
 Name        : hev-zmquery.h
 Author      : Heiher <admin@heiher.info>
 Version     : 0.0.1
 Copyright   : Copyright (c) 2010 everyone.
 Description : Zhengma decoder
 ============================================================================
 */

#ifndef __HEV_ZMQUERY_H__
#define __HEV_ZMQUERY_H__

#include <gio/gio.h>

G_BEGIN_DECLS

#define HEV_TYPE_ZMQUERY	(hev_zmquery_get_type())
#define HEV_ZMQUERY(obj)	(G_TYPE_CHECK_INSTANCE_CAST((obj), HEV_TYPE_ZMQUERY, HevZmQuery))
#define HEV_IS_ZMQUERY(obj)	(G_TYPE_CHECK_INSTANCE_TYPE((obj), HEV_TYPE_ZMQUERY))
#define HEV_ZMQUERY_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST((klass), HEV_TYPE_ZMQUERY, HevZmQueryClass))
#define HEV_IS_ZMQUERY_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass), HEV_TYPE_ZMQUERY))
#define HEV_ZMQUERY_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), HEV_TYPE_ZMQUERY, HevZmQueryClass))

typedef struct _HevZmQuery HevZmQuery;
typedef struct _HevZmQueryClass HevZmQueryClass;

typedef gint (*HevZmQueryLookupHandler)(const GString * const keyword, const GString * const value, gpointer data);

struct _HevZmQuery
{
	GObject parent_instance;
};

struct _HevZmQueryClass
{
	GObjectClass parent_class;
};

GType hev_zmquery_get_type(void);

GObject * hev_zmquery_new(GCancellable * cancellable, GError ** error);
gint hev_zmquery_lookup(HevZmQuery * self, const char * const keyword, HevZmQueryLookupHandler handler, gpointer data);

G_END_DECLS

#endif /* __HEV_ZMQUERY_H__ */

