/*
 * gtk-synce-callbacks.h
 *
 * Copyright 2003 Gunter Wambaugh
 *
 * This file is part of gtk-synce.
 *
 * gtk-synce is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * gtk-synce is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gtk-splitter; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef GTK_SYNCE_CALLBACKS_H
#define GTK_SYNCE_CALLBACKS_H

#include <gtk/gtk.h>
#include "gtk-synce-interface.h"

void gtk_synce_add_program (GtkWidget *widget, gpointer data);

void get_file_name_and_add (GtkWidget *widget, gpointer data);

void gtk_synce_remove_program (GtkWidget *widget, gpointer data);

void gtk_synce_window_destroy_cb (GtkWidget *widget, gpointer data);

void gtk_synce_set_update_flag (GtkWidget *widget, gpointer data);

#endif /* GTK_SYNCE_CALLBACKS_H */
