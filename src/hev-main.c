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

gchar * hev_build_res_path(const gchar *res)
{
	GFile *file = NULL;
	gchar *path = NULL;

	file = g_file_new_for_path("/proc/self/exe");
	if(file)
	{
		GFileInfo *file_info = NULL;

		file_info = g_file_query_info(file, G_FILE_ATTRIBUTE_STANDARD_SYMLINK_TARGET,
					G_FILE_QUERY_INFO_NONE, NULL, NULL);
		if(file_info)
		{
			const gchar *p = g_file_info_get_symlink_target(file_info);
			path = g_strdup(p);
			g_object_unref(file_info);
		}

		g_object_unref(file);
	}

	file = g_file_new_for_path(path);
	g_free(path);
	if(file)
	{
		GFile *fp = NULL;

		fp = g_file_get_parent(file);
		if(fp)
		{
			gchar *p = g_file_get_path(fp);
			path = g_build_filename(p, res, NULL);
			g_free(p);
			g_object_unref(fp);
		}

		g_object_unref(file);
	}

	return path;
}

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
