/*
 * gtk-synce-callbacks.c
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
#include "gtk-synce-callbacks.h"
#include "gtk-synce-interface.h"
#include "synce-interface.h"

void gtk_synce_add_program (GtkWidget *widget, gpointer data)
{ 
    GtkSynceProgramsTab *gpt = data;
    static GtkWidget *file_selector = NULL;
    
    if (file_selector == NULL)
    {
        file_selector = gtk_file_selection_new ("Select a CAB file to install");
    
        g_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (file_selector)->ok_button),
                          "clicked",
                          G_CALLBACK (get_file_name_and_add),
                          (gpointer) file_selector);

        g_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (file_selector)->ok_button),
                          "clicked",
                          G_CALLBACK (gtk_synce_status_dialog),
                          (gpointer) "The cab file has been copied and the installation " \
                                     "process started.  Please follow the directions on " \
                                     "your device to complete the installation.");

        g_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (file_selector)->ok_button),
                          "clicked",
                          G_CALLBACK (gtk_synce_set_update_flag),
                          (gpointer) gpt);

        g_signal_connect_swapped (GTK_OBJECT (GTK_FILE_SELECTION (file_selector)->ok_button),
                                  "clicked",
                                  G_CALLBACK (gtk_widget_destroy), 
                                  (gpointer) file_selector);
    
        g_signal_connect_swapped (GTK_OBJECT (GTK_FILE_SELECTION (file_selector)->cancel_button),
                                  "clicked",
                                  G_CALLBACK (gtk_widget_destroy),
                                  (gpointer) file_selector);
                                  
        g_signal_connect (GTK_OBJECT (file_selector), 
                          "destroy",
                          G_CALLBACK (gtk_widget_destroyed),
                          &file_selector);
                              
        gtk_widget_show_all (file_selector);
    }
}

void get_file_name_and_add (GtkWidget *widget, gpointer data)
{ 
    GtkWidget *file_selector = data;
    
    gchar *selected_file;
    
    selected_file = (gchar*) gtk_file_selection_get_filename (GTK_FILE_SELECTION (file_selector));
    
    /* Hide the file selector.  After synce_add_program () returns,
       the file selector will be destroyed through a callback. */
    gtk_widget_hide_all (file_selector);
    
    while (g_main_context_iteration (NULL, FALSE));
    
    if (synce_init () == 1);
    {
        synce_add_program (selected_file);
    
        synce_uninit ();
    }
}

void gtk_synce_remove_program (GtkWidget *widget, gpointer data)
{ 
    GtkSynceProgramsTab *gpt = data;
    GtkTreeModel *model;
    GtkTreeIter iter;
    GtkTreeSelection *selection;
    gchar *selected_string;
    
    selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (gpt->tree_view)); 

    model = gtk_tree_view_get_model (GTK_TREE_VIEW (gpt->tree_view));
    
    if (gtk_tree_selection_get_selected (selection, &model, &iter))
    {
        gtk_tree_model_get (model, &iter, 0, &selected_string, -1);
        
        if (synce_init () == 1);
        {
            synce_remove_program (selected_string);
    
            synce_uninit ();
        
            gtk_list_store_remove (gpt->store, &iter);
        
            while (g_main_context_iteration (NULL, FALSE));
        
            gtk_synce_status_dialog (NULL, (gpointer) "The uninstallation process has been started.  " \
                                                      "Please follow the directions on your device "   \
                                                      "to complete the uninstallation.");
            
            gtk_synce_set_update_flag (NULL, (gpointer) gpt);
        }
        
        g_free (selected_string);
    }
}

void gtk_synce_window_destroy_cb (GtkWidget *widget, gpointer data)
{ 
    GtkSynceWindow *gsw = data;

    gtk_synce_window_destroy (gsw);
}

void gtk_synce_set_update_flag (GtkWidget *widget, gpointer data)
{ 
    GtkSynceProgramsTab *gpt = data;
    
    gpt->need_update = TRUE;
}
