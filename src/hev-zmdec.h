/*
 ============================================================================
 Name        : hev-zmdec.h
 Author      : Heiher <admin@heiher.info>
 Version     : 0.0.1
 Copyright   : Copyright (c) 2010 everyone.
 Description : Zhengma Decoder
 ============================================================================
 */

#ifndef __HEV_ZMDEC_H__
#define __HEV_ZMDEC_H__

#include <gio/gio.h>

G_BEGIN_DECLS

#define HEV_TYPE_ZMDEC	(hev_zmdec_get_type())
#define HEV_ZMDEC(obj)	(G_TYPE_CHECK_INSTANCE_CAST((obj), HEV_TYPE_ZMDEC, HevZmDec))
#define HEV_IS_ZMDEC(obj)	(G_TYPE_CHECK_INSTANCE_TYPE((obj), HEV_TYPE_ZMDEC))
#define HEV_ZMDEC_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST((klass), HEV_TYPE_ZMDEC, HevZmDecClass))
#define HEV_IS_ZMDEC_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass), HEV_TYPE_ZMDEC))
#define HEV_ZMDEC_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), HEV_TYPE_ZMDEC, HevZmDecClass))

typedef struct _HevZmDec HevZmDec;
typedef struct _HevZmDecClass HevZmDecClass;

struct _HevZmDec
{
	GObject parent_instance;
};

struct _HevZmDecClass
{
	GObjectClass parent_class;
};

GType hev_zmdec_get_type(void);

GObject * hev_zmdec_new(GCancellable * cancellable, GError ** error);

G_END_DECLS

#endif /* __HEV_ZMDEC_H__ */

