/*
 * gtk-synce-interface.h
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

#ifndef GTK_SYNCE_INTERFACE_H
#define GTK_SYNCE_INTERFACE_H

#include <gtk/gtk.h>

enum {COLUMN_PROGRAM_STRING, NUMBER_OF_COLUMNS};

typedef struct
{
    GtkWidget *base_widget;
    GtkWidget *version_header_label;
    GtkWidget *version_label;
    GtkWidget *platform_header_label;
    GtkWidget *platform_label;
    GtkWidget *architecture_header_label;
    GtkWidget *architecture_label;
    GtkWidget *type_header_label;
    GtkWidget *type_label;
    GtkWidget *power_header_label;
    GtkWidget *power_label;
    GtkWidget *main_battery_header_label;
    GtkWidget *main_battery_progress_bar;
    GtkWidget *backup_battery_header_label;
    GtkWidget *backup_battery_progress_bar;
    GtkWidget *store_use_header_label;
    GtkWidget *store_use_progress_bar;
} GtkSynceStatusTab;

typedef struct
{
    GtkWidget *base_widget;
    GtkWidget *button_box;
    GtkWidget *add_button;
    GtkWidget *remove_button;
    GtkWidget *scroll_pane;
    GtkWidget *tree_view;
    GtkListStore *store;
    gboolean need_update;
} GtkSynceProgramsTab;

typedef struct
{
    GtkWidget *base_window;
    GtkWidget *vertical_box;
    GtkWidget *notebook;
    GtkWidget *notebook_status_label;
    GtkWidget *notebook_programs_label;
    GtkWidget *statusbar;
    GdkPixbuf *icon;
    GtkSynceStatusTab *status_tab;
    GtkSynceProgramsTab *programs_tab;
} GtkSynceWindow;

typedef struct
{
    GtkWidget *window;
    GtkWidget *table;
    GtkWidget *label;
    GtkWidget *progress_bar;
} GtkSynceProgressWindow;

GtkSynceWindow* gtk_synce_window_new ();

void gtk_synce_window_destroy (GtkSynceWindow *gsw);

GtkSynceStatusTab* gtk_synce_status_tab_new ();

gboolean gtk_synce_update_status_tab (GtkSynceStatusTab *gst);

void gtk_synce_clear_status_tab (GtkSynceStatusTab *gst);

void gtk_synce_status_tab_destroy (GtkSynceStatusTab *gst);

GtkSynceProgramsTab* gtk_synce_programs_tab_new ();

gboolean gtk_synce_update_programs_tab (GtkSynceProgramsTab *gpt);

void gtk_synce_clear_programs_tab (GtkSynceProgramsTab *gpt);

void gtk_synce_programs_tab_destroy (GtkSynceProgramsTab *gpt);

GtkSynceProgressWindow* gtk_synce_progress_window_new ();

void gtk_synce_progress_window_set_label (GtkSynceProgressWindow *gpw, gchar *message);

void gtk_synce_progress_window_set_fraction (GtkSynceProgressWindow *gpw, gdouble fraction);

void gtk_synce_progress_window_destroy (GtkSynceProgressWindow *gpw);

void gtk_synce_status_dialog (GtkWidget *widget, gpointer data);

#endif /* GTK_SYNCE_INTERFACE_H */
