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
 
#include <gtk/gtk.h>
#include "gtk-synce-interface.h"
#include "synce-interface.h"

guint update_window_handler;

gboolean update_window (gpointer data)
{ 
    GtkSynceWindow *gsw = data;
    guint page_number;
    const gchar *title;

    /* If synce is 'busy' quit now. */
    if (synce_inited == 1)
        return TRUE;
    
    /* Connected: update the tabs. */
    if (synce_init () == 1)
    {
        /* If connected, use the device name as the window title. */
        title = gtk_window_get_title (GTK_WINDOW (gsw->base_window));
        if (g_ascii_strcasecmp  (title, "gtk-synce") == 0)
        {
            gtk_window_set_title (GTK_WINDOW (gsw->base_window), synce_get_device_name_string ());
        }
        synce_uninit ();
        
        /* Determine the currently focused page. */
        page_number = gtk_notebook_get_current_page (GTK_NOTEBOOK (gsw->notebook));
        
        /* The status tab is visible. */
        if (page_number == 0)
        {
            /* Notify the user we are getting data. */
            gtk_statusbar_push (GTK_STATUSBAR (gsw->statusbar), 0, "Getting device information...");    
            while (g_main_context_iteration (NULL, FALSE));
                
            /* Update the status tab. */
            gtk_synce_update_status_tab (gsw->status_tab);
            
            /* Remove the user notification. */
            gtk_statusbar_pop (GTK_STATUSBAR (gsw->statusbar), 0);
        }
        /* The programs tab is visible. */
        else
        {
            if (gsw->programs_tab->need_update)
            {
                /* Notify the user we are getting data. */
                gtk_statusbar_push (GTK_STATUSBAR (gsw->statusbar), 0, "Getting device information...");    
                while (g_main_context_iteration (NULL, FALSE));
                
                /* Update the programs tab. */                
                gtk_synce_clear_programs_tab (gsw->programs_tab);
                gtk_synce_update_programs_tab (gsw->programs_tab);
                
                /* Remove the user notification. */
                gtk_statusbar_pop (GTK_STATUSBAR (gsw->statusbar), 0);
            }
        }
    }
    /* Not connected: clear the tabs. */
    else
    {
        gtk_window_set_title (GTK_WINDOW (gsw->base_window), "gtk-synce");
        gtk_synce_clear_status_tab (gsw->status_tab);
        gtk_synce_clear_programs_tab (gsw->programs_tab);
    }
    
    return TRUE;
}

gboolean remove_timeout (gpointer data)
{ 
    gtk_timeout_remove (update_window_handler);

    return TRUE;    
}

int main (int argc, char *argv[])
{
    GtkSynceWindow *window;

    /* Initialize GTK. */
    gtk_init (&argc, &argv);
    
    /* Print version information. */
    g_print ("%s-%s Gunter Wambaugh\n", PACKAGE, VERSION);
    
    /* Create a new window. */
    window = gtk_synce_window_new ();
   
    /* Show the window. */
    gtk_widget_show_all (window->base_window);

    /* Update the window upon initialization of GTK. */
    gtk_init_add ((GtkFunction) update_window, (gpointer) window);

    /* Add a timeout to update the window. */
    update_window_handler = gtk_timeout_add (10000,
        (GtkFunction) update_window, (gpointer) window);

    /* Remove the timeout when the main loop of GTK quits. */
    gtk_quit_add (0, (GtkFunction) remove_timeout, NULL);
    
    /* Run the main loop of GTK. */
    gtk_main ();
    
    return 0;
}
