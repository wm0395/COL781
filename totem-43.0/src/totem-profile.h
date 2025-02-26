/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2001,2002,2003 Bastien Nocera <hadess@hadess.net>
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

#pragma once

#ifdef GNOME_ENABLE_DEBUG

#include <sys/time.h>
#include <glib.h>

#define TOTEM_PROFILE(function)     \
    do{                             \
      struct timeval current_time;  \
      double dtime;                 \
      gettimeofday(&current_time, NULL); \
      dtime = -(current_time.tv_sec + (current_time.tv_usec / 1000000.0)); \
      function;                     \
      gettimeofday(&current_time, NULL); \
      dtime += current_time.tv_sec + (current_time.tv_usec / 1000000.0); \
      printf("(%s:%d) took %lf seconds\n", \
	     G_STRFUNC, __LINE__, dtime ); \
    }while(0)

#else /* GNOME_ENABLE_DEBUG */

#define TOTEM_PROFILE(function) function

#endif /* GNOME_ENABLE_DEBUG */
