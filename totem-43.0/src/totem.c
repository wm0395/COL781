/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/* 
 * Copyright (C) 2001-2007 Bastien Nocera <hadess@hadess.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA.
 *
 * The Totem project hereby grant permission for non-gpl compatible GStreamer
 * plugins to be used and distributed together with GStreamer and Totem. This
 * permission are above and beyond the permissions granted by the GPL license
 * Totem is covered by.
 *
 * Monday 7th February 2005: Christian Schaller: Add exception clause.
 * See license_change file for details.
 *
 */

#include "config.h"

#include <glib.h>
#include <glib-object.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <locale.h>
#include <string.h>

#include "totem.h"

int
main (int argc, char **argv)
{
	Totem *totem;

	setlocale (LC_ALL, "");
	bindtextdomain (GETTEXT_PACKAGE, GNOMELOCALEDIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);

	g_set_prgname ("totem");
#if DEVELOPMENT_VERSION
	g_set_application_name (_("Videos Preview"));
#else
	g_set_application_name (_("Videos"));
#endif
	gtk_window_set_default_icon_name (APPLICATION_ID);
	g_setenv("PULSE_PROP_media.role", "video", TRUE);
	g_setenv("PULSE_PROP_application.icon_name", APPLICATION_ID, TRUE);

	/* Build the main Totem object */
	totem = g_object_new (TOTEM_TYPE_OBJECT,
			      "application-id", APPLICATION_ID,
			      "flags", G_APPLICATION_HANDLES_OPEN,
			      NULL);

	g_application_run (G_APPLICATION (totem), argc, argv);

	return 0;
}
