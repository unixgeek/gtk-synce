/*
 * synce-interface.h
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

#ifndef SYNCE_INTERFACE_H
#define SYNCE_INTERFACE_H

#include <rapi.h>
#include <gtk/gtk.h>

HRESULT hr;
CEOSVERSIONINFO ce_version;
SYSTEM_INFO ce_system;
SYSTEM_POWER_STATUS_EX ce_power;
STORE_INFORMATION ce_store;
int synce_inited;

int synce_init ();

void synce_uninit ();

int synce_read_all ();

char* synce_get_version_string ();

char* synce_get_platform_string ();

char* synce_get_architecture_string ();

char* synce_get_type_string ();

char* synce_get_power_status ();

char* synce_get_main_battery_flag_string ();

double synce_get_main_battery_level ();

char* synce_get_backup_battery_flag_string ();

double synce_get_backup_battery_level ();

char* synce_get_store_use_string ();

double synce_get_store_use ();

char* synce_get_device_name_string ();

char* synce_get_installed_programs_string ();

void synce_add_program (char *file_name);

void synce_remove_program (char *program_name);

int synce_copy_file (char *file_name);

int synce_create_directory (char *directory);

int synce_create_process (char *command, char *args);

#endif /* SYNCE_INTERFACE_H */
