/*
 * gtk-synce-interface.c
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

#include <gtk/gtk.h>
#include <string.h>
#include "gtk-synce-interface.h"
#include "gtk-synce-callbacks.h"
#include "synce-interface.h"

GtkSynceWindow* gtk_synce_window_new ()
{ 
    GtkSynceWindow *gsw;

    gsw = g_malloc (sizeof (GtkSynceWindow));
    
    if (gsw == NULL)
        return NULL;

    gsw->base_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (gsw->base_window), "gtk-synce");
    gtk_window_set_resizable (GTK_WINDOW (gsw->base_window), FALSE);

    gsw->icon = gdk_pixbuf_new_from_file (ICON_AND_PATH, NULL);    

    gtk_window_set_icon (GTK_WINDOW (gsw->base_window), gsw->icon);

    gsw->vertical_box = gtk_vbox_new (FALSE, 0);
    
    gsw->notebook = gtk_notebook_new ();
    gsw->notebook_status_label = gtk_label_new ("Device Status");
    gsw->notebook_programs_label = gtk_label_new ("Installed Programs");

    gsw->status_tab = gtk_synce_status_tab_new ();
    gsw->programs_tab = gtk_synce_programs_tab_new ();
    
    gtk_notebook_append_page (GTK_NOTEBOOK (gsw->notebook), gsw->status_tab->base_widget, 
                              gsw->notebook_status_label);
    gtk_notebook_append_page (GTK_NOTEBOOK (gsw->notebook), gsw->programs_tab->base_widget, 
                              gsw->notebook_programs_label);
    
    gsw->statusbar = gtk_statusbar_new ();

    gtk_container_add (GTK_CONTAINER (gsw->base_window), gsw->vertical_box);
    
    gtk_box_pack_start (GTK_BOX (gsw->vertical_box), gsw->notebook, TRUE, TRUE, 0);
    
    gtk_box_pack_start (GTK_BOX (gsw->vertical_box), gsw->statusbar, TRUE, TRUE, 0);

    g_signal_connect (GTK_OBJECT (gsw->base_window), "destroy",
                      G_CALLBACK (gtk_widget_destroyed), &gsw->base_window);
                      
    g_signal_connect (GTK_OBJECT (gsw->base_window), "destroy",
                      G_CALLBACK (gtk_synce_window_destroy_cb), (gpointer) gsw);

    g_signal_connect (GTK_OBJECT (gsw->base_window), "destroy",
                      G_CALLBACK (gtk_main_quit), NULL);

    return gsw;
}

void gtk_synce_window_destroy (GtkSynceWindow *gsw)
{ 
    if (gsw->base_window != NULL)
        gtk_widget_destroy (gsw->base_window);
    
    gtk_synce_status_tab_destroy (gsw->status_tab);
    
    gtk_synce_programs_tab_destroy (gsw->programs_tab);
    
    g_free (gsw);
}

GtkSynceStatusTab* gtk_synce_status_tab_new ()
{ 
    GtkSynceStatusTab *gst;

    gst = g_malloc (sizeof (GtkSynceStatusTab));
    
    if (gst == NULL)
        return NULL;

    gst->version_header_label = gtk_label_new ("Version:");
    gst->version_label = gtk_label_new ("");
    
    gst->platform_header_label = gtk_label_new ("Platform:");
    gst->platform_label = gtk_label_new ("");
    
    gst->architecture_header_label = gtk_label_new ("Processor Architecture:");
    gst->architecture_label = gtk_label_new ("");
    
    gst->type_header_label = gtk_label_new ("Processor Type:");
    gst->type_label = gtk_label_new ("");
    
    gst->power_header_label = gtk_label_new ("Power Status:");
    gst->power_label = gtk_label_new ("");
    
    gst->main_battery_header_label = gtk_label_new ("Main Battery:");
    gst->main_battery_progress_bar = gtk_progress_bar_new ();
    
    gst->backup_battery_header_label = gtk_label_new ("Backup Battery:");
    gst->backup_battery_progress_bar = gtk_progress_bar_new ();
    
    gst->store_use_header_label = gtk_label_new ("Store Use:");
    gst->store_use_progress_bar = gtk_progress_bar_new ();
    
    gst->base_widget = gtk_table_new (8, 2, TRUE); 
    
    gtk_container_set_border_width (GTK_CONTAINER (gst->base_widget), 5);
    
    gtk_table_attach (GTK_TABLE (gst->base_widget), gst->version_header_label,
	              0, 1, 0, 1, GTK_FILL|GTK_EXPAND, GTK_FILL|GTK_EXPAND, 0, 5);
    gtk_table_attach (GTK_TABLE (gst->base_widget), gst->version_label,
	              1, 2, 0, 1, GTK_FILL|GTK_EXPAND, GTK_FILL|GTK_EXPAND, 0, 5);
	              
    gtk_table_attach (GTK_TABLE (gst->base_widget), gst->platform_header_label,
	              0, 1, 1, 2, GTK_FILL|GTK_EXPAND, GTK_FILL|GTK_EXPAND, 0, 5);
    gtk_table_attach (GTK_TABLE (gst->base_widget), gst->platform_label,
	              1, 2, 1, 2, GTK_FILL|GTK_EXPAND, GTK_FILL|GTK_EXPAND, 0, 5);
	              
    gtk_table_attach (GTK_TABLE (gst->base_widget), gst->architecture_header_label,
	              0, 1, 2, 3, GTK_FILL|GTK_EXPAND, GTK_FILL|GTK_EXPAND, 0, 5);
    gtk_table_attach (GTK_TABLE (gst->base_widget), gst->architecture_label,
	              1, 2, 2, 3, GTK_FILL|GTK_EXPAND, GTK_FILL|GTK_EXPAND, 0, 5);

    gtk_table_attach (GTK_TABLE (gst->base_widget), gst->type_header_label,
	              0, 1, 3, 4, GTK_FILL|GTK_EXPAND, GTK_FILL|GTK_EXPAND, 0, 5);
    gtk_table_attach (GTK_TABLE (gst->base_widget), gst->type_label,
	              1, 2, 3, 4, GTK_FILL|GTK_EXPAND, GTK_FILL|GTK_EXPAND, 0, 5);

    gtk_table_attach (GTK_TABLE (gst->base_widget), gst->power_header_label,
	              0, 1, 4, 5, GTK_FILL|GTK_EXPAND, GTK_FILL|GTK_EXPAND, 0, 5);
    gtk_table_attach (GTK_TABLE (gst->base_widget), gst->power_label,
	              1, 2, 4, 5, GTK_FILL|GTK_EXPAND, GTK_FILL|GTK_EXPAND, 0, 5);

    gtk_table_attach (GTK_TABLE (gst->base_widget), gst->main_battery_header_label,
	              0, 1, 5, 6, GTK_FILL|GTK_EXPAND, GTK_FILL|GTK_EXPAND, 0, 5);
    gtk_table_attach (GTK_TABLE (gst->base_widget), gst->main_battery_progress_bar,
	              1, 2, 5, 6, GTK_FILL|GTK_EXPAND, GTK_FILL|GTK_EXPAND, 0, 5);

    gtk_table_attach (GTK_TABLE (gst->base_widget), gst->backup_battery_header_label,
	              0, 1, 6, 7, GTK_FILL|GTK_EXPAND, GTK_FILL|GTK_EXPAND, 0, 5);
    gtk_table_attach (GTK_TABLE (gst->base_widget), gst->backup_battery_progress_bar,
	              1, 2, 6, 7, GTK_FILL|GTK_EXPAND, GTK_FILL|GTK_EXPAND, 0, 5);

    gtk_table_attach (GTK_TABLE (gst->base_widget), gst->store_use_header_label,
	              0, 1, 7, 8, GTK_FILL|GTK_EXPAND, GTK_FILL|GTK_EXPAND, 0, 5);
    gtk_table_attach (GTK_TABLE (gst->base_widget), gst->store_use_progress_bar,
	              1, 2, 7, 8, GTK_FILL|GTK_EXPAND, GTK_FILL|GTK_EXPAND, 0, 5);
                  
    return gst;
}

gboolean gtk_synce_update_status_tab (GtkSynceStatusTab *gst)
{ 
    char *store_use_string;
    int init_status;
    int read_status;
    
    init_status = 0;
    read_status = 0;
    
    /* Initialize the synce interface. */
    init_status = synce_init ();
    
    /* If the synce interface initialized correctly,
       read data from the device. */ 
    if (init_status == 1);
        read_status = synce_read_all ();
    
    /* If the interface initialized okay, and we have data,
       then put data into the GUI. */
    if ((init_status) && (read_status))
    {
        gtk_label_set_text (GTK_LABEL (gst->version_label), synce_get_version_string ());

        gtk_label_set_text (GTK_LABEL (gst->platform_label), synce_get_platform_string ());

        gtk_label_set_text (GTK_LABEL (gst->architecture_label), synce_get_architecture_string ());

        gtk_label_set_text (GTK_LABEL (gst->type_label), synce_get_type_string ());

        gtk_label_set_text (GTK_LABEL (gst->power_label), synce_get_power_status ());

        gtk_progress_bar_set_text (GTK_PROGRESS_BAR (gst->main_battery_progress_bar), 
                                   synce_get_main_battery_flag_string ());
   
        gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (gst->main_battery_progress_bar),
                                       synce_get_main_battery_level ());

        gtk_progress_bar_set_text (GTK_PROGRESS_BAR (gst->backup_battery_progress_bar), 
                                   synce_get_backup_battery_flag_string ());

        gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (gst->backup_battery_progress_bar), 
                                       synce_get_backup_battery_level ());

        store_use_string = synce_get_store_use_string ();
        gtk_progress_bar_set_text (GTK_PROGRESS_BAR (gst->store_use_progress_bar), 
                                   store_use_string);
        g_free (store_use_string);

        gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (gst->store_use_progress_bar), 
                                       synce_get_store_use ());   

        while (g_main_context_iteration (NULL, FALSE));
            
        synce_uninit ();  
    }
    /* No device or no data, so clear the GUI. */
    else
    {
        gtk_synce_clear_status_tab (gst);
    }

    /* Return TRUE for gtk_timeout_add (). */
    return TRUE;
}

void gtk_synce_clear_status_tab (GtkSynceStatusTab *gst)
{ 
    gtk_label_set_text (GTK_LABEL (gst->version_label), "");

    gtk_label_set_text (GTK_LABEL (gst->platform_label), "");
        
    gtk_label_set_text (GTK_LABEL (gst->architecture_label), "");

    gtk_label_set_text (GTK_LABEL (gst->type_label), "");

    gtk_label_set_text (GTK_LABEL (gst->power_label), "");

    gtk_progress_bar_set_text (GTK_PROGRESS_BAR (gst->main_battery_progress_bar), "");

    gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (gst->main_battery_progress_bar), 0.0);

    gtk_progress_bar_set_text (GTK_PROGRESS_BAR (gst->backup_battery_progress_bar), "");

    gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (gst->backup_battery_progress_bar), 0.0);

    gtk_progress_bar_set_text (GTK_PROGRESS_BAR (gst->store_use_progress_bar), "");

    gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (gst->store_use_progress_bar), 0.0);
        
    while (g_main_context_iteration (NULL, FALSE));
}

void gtk_synce_status_tab_destroy (GtkSynceStatusTab *gst)
{ 
    g_free (gst);
}

GtkSynceProgramsTab* gtk_synce_programs_tab_new ()
{ 
    GtkSynceProgramsTab *gpt;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    
    gpt = g_malloc (sizeof (GtkSynceProgramsTab));
    
    if (gpt == NULL)
        return NULL;

    gpt->store = gtk_list_store_new (NUMBER_OF_COLUMNS, G_TYPE_STRING);
	
    gpt->tree_view = gtk_tree_view_new_with_model (GTK_TREE_MODEL (gpt->store));
    
    renderer = gtk_cell_renderer_text_new ();
    
    column = gtk_tree_view_column_new_with_attributes ("", renderer, "text",
                                                       COLUMN_PROGRAM_STRING, NULL);
    
    gtk_tree_view_append_column (GTK_TREE_VIEW (gpt->tree_view), column);
    
    gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (gpt->tree_view), FALSE);

    gpt->scroll_pane = gtk_scrolled_window_new (NULL, NULL);
    
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (gpt->scroll_pane), 
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	
    gtk_container_add (GTK_CONTAINER (gpt->scroll_pane), gpt->tree_view);
    
    gpt->base_widget = gtk_vbox_new (FALSE, 0);
    
    gtk_container_set_border_width (GTK_CONTAINER (gpt->base_widget), 5);
    
    gtk_box_pack_start (GTK_BOX (gpt->base_widget), gpt->scroll_pane, TRUE, TRUE, 5);
    
    gpt->add_button = gtk_button_new_from_stock (GTK_STOCK_ADD);
    
    gpt->remove_button = gtk_button_new_from_stock (GTK_STOCK_REMOVE);
    
    gpt->button_box = gtk_hbutton_box_new ();
    
    gtk_button_box_set_layout (GTK_BUTTON_BOX (gpt->button_box), GTK_BUTTONBOX_SPREAD);
    
    gtk_container_add (GTK_CONTAINER (gpt->button_box), gpt->add_button);
    
    gtk_container_add (GTK_CONTAINER (gpt->button_box), gpt->remove_button);
    
    gtk_box_pack_start (GTK_BOX (gpt->base_widget), gpt->button_box, FALSE, FALSE, 0);
    
    gtk_widget_set_sensitive (gpt->add_button, FALSE);
        
    gtk_widget_set_sensitive (gpt->remove_button, FALSE);
    
    g_signal_connect (GTK_OBJECT (gpt->add_button), "clicked",
                      G_CALLBACK (gtk_synce_add_program), (gpointer) gpt);
                      
    g_signal_connect (GTK_OBJECT (gpt->remove_button), "clicked",
                      G_CALLBACK (gtk_synce_remove_program), (gpointer) gpt);
                      
    gpt->need_update = TRUE;
    
    return gpt;
}

gboolean gtk_synce_update_programs_tab (GtkSynceProgramsTab *gpt)
{ 
    GtkTreeIter iter;
    gchar *programs_list;
    gchar *current;
    
    if (synce_init ())
    {
        programs_list = synce_get_installed_programs_string ();

        current = strtok (programs_list, ":");
        
        while (current != NULL)
        {
            gtk_list_store_append (gpt->store, &iter);
    
            gtk_list_store_set (gpt->store, &iter,
                                COLUMN_PROGRAM_STRING,
                                current, -1);
                                
            current = strtok (NULL, ":");
        }
        
        while (g_main_context_iteration (NULL, FALSE));
            
        synce_uninit ();
        
        gtk_widget_set_sensitive (gpt->add_button, TRUE);
        gtk_widget_set_sensitive (gpt->remove_button, TRUE);
        
        gpt->need_update = FALSE;
        
        g_free (programs_list);
    }
    else
    {
        gtk_synce_clear_programs_tab (gpt);
    }
    
    return TRUE;
}

void gtk_synce_clear_programs_tab (GtkSynceProgramsTab *gpt)
{ 
    gtk_list_store_clear (gpt->store);
    
    gtk_widget_set_sensitive (gpt->add_button, FALSE);
    gtk_widget_set_sensitive (gpt->remove_button, FALSE);
    
    gpt->need_update = TRUE;
    
    while (g_main_context_iteration (NULL, FALSE));
}

void gtk_synce_programs_tab_destroy (GtkSynceProgramsTab *gpt)
{ 
    g_free (gpt);
}

GtkSynceProgressWindow* gtk_synce_progress_window_new ()
{ 
    GtkSynceProgressWindow *gpw;
    
    gpw = g_malloc (sizeof (GtkSynceProgressWindow));
    if (gpw == NULL)
        return NULL;
    
    gpw->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_resizable (GTK_WINDOW (gpw->window), FALSE);
    gtk_window_set_title (GTK_WINDOW (gpw->window), "");
    
    gpw->table = gtk_table_new (3, 1, FALSE);
    
    gpw->label = gtk_label_new ("");
    gpw->progress_bar = gtk_progress_bar_new ();
    
    gtk_container_add (GTK_CONTAINER (gpw->window), gpw->table);
    
    gtk_table_attach (GTK_TABLE (gpw->table), gpw->label, 0, 1, 0, 1, GTK_FILL, GTK_FILL, 10, 10);
    gtk_table_attach (GTK_TABLE (gpw->table), gtk_hseparator_new (), 0, 1, 1, 2, GTK_FILL, GTK_FILL, 10, 5);
    gtk_table_attach (GTK_TABLE (gpw->table), gpw->progress_bar, 0, 1, 2, 3, GTK_FILL, GTK_FILL, 10, 10);
   
    g_signal_connect (GTK_OBJECT (gpw->window), "destroy",
                      G_CALLBACK (gtk_widget_destroyed), &gpw->window);   
   
    return gpw;
}

void gtk_synce_progress_window_set_label (GtkSynceProgressWindow *gpw, gchar *message)
{ 
    gtk_label_set_text (GTK_LABEL (gpw->label), message);
    
    while (g_main_context_iteration (NULL, FALSE));
}

void gtk_synce_progress_window_set_fraction (GtkSynceProgressWindow *gpw, gdouble fraction)
{ 
    gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (gpw->progress_bar), fraction);
    
    while (g_main_context_iteration (NULL, FALSE));
}

void gtk_synce_progress_window_destroy (GtkSynceProgressWindow *gpw)
{ 
    if (gpw->window != NULL)
        gtk_widget_destroy (gpw->window);
    
    g_free (gpw);
}

void gtk_synce_status_dialog (GtkWidget *widget, gpointer data)
{ 
    gchar *message = data;
    GtkWidget *dialog;
    
    dialog = gtk_message_dialog_new (NULL,
                                     GTK_DIALOG_MODAL,
                                     GTK_MESSAGE_INFO,
                                     GTK_BUTTONS_CLOSE,
                                     "%s", message);

    gtk_dialog_run (GTK_DIALOG (dialog));
    
    gtk_widget_destroy (dialog);
}
