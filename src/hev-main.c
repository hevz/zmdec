/*
 ============================================================================
 Name        : hev-main.c
 Author      : Heiher <admin@heiher.info>
 Version     : 0.0.1
 Copyright   : Copyright (c) 2010 everyone.
 Description : Zhengma decoder
 ============================================================================
 */

#include <gtk/gtk.h>

#include "hev-zmdec.h"

int main(int argc, char * argv[])
{
	GObject * zmdec = NULL;

	gtk_init(&argc, &argv);

	zmdec = hev_zmdec_new(NULL, NULL);
	if(HEV_IS_ZMDEC(zmdec))
	{
		gtk_main();

		g_object_unref(zmdec);
	}

	return 0;
}
