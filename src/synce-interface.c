/*
 * synce-interface.c
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

#include <rapi.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <synce_log.h>
#include <gtk/gtk.h>
#include "synce-interface.h"
#include "gtk-synce-interface.h"

int synce_init ()
{
    if (synce_inited == 1)
        return 1;
        
    synce_log_set_level (SYNCE_LOG_LEVEL_LOWEST);
    
    hr = CeRapiInit ();
    
    if (FAILED (hr))
        return 0;
    
    memset (&ce_version, 0, sizeof (ce_version));
    ce_version.dwOSVersionInfoSize = sizeof (ce_version);
    memset (&ce_system, 0, sizeof (ce_system));
    memset (&ce_power, 0, sizeof (SYSTEM_POWER_STATUS_EX));
    memset (&ce_store, 0, sizeof (ce_store));
    
    synce_inited = 1;
    
    return 1;
}

void synce_uninit ()
{ 
    CeRapiUninit();
    
    synce_inited = 0;
}

int synce_read_all ()
{ 
    if (CeGetVersionEx (&ce_version) == 0)
        return 0;
    
    CeGetSystemInfo (&ce_system);
    
    if (CeGetSystemPowerStatusEx (&ce_power, false) == false)
        return 0;
    
    if (CeGetStoreInformation (&ce_store) == false)
        return 0;
    
    return 1;
}

char* synce_get_version_string ()
{ 
    char *version_string;
    
    version_string = "Unknown";
    
    /* The following code is from pstatus.c (synce-librapi2 tools). */
    if (ce_version.dwMajorVersion == 4 &&
        ce_version.dwMinorVersion == 20 &&
        ce_version.dwBuildNumber  == 1081)
    {
        version_string = "Ozone: Pocket PC 2003";
    } 
    else if (ce_version.dwMajorVersion == 3 && ce_version.dwMinorVersion == 0)
    {
        switch (ce_version.dwBuildNumber)
        {
            case 9348:
                version_string = "Rapier: Pocket PC";
                break;
            case 11171: 
                version_string = "Merlin: Pocket PC 2002";
                break;
            case 12255: 
                version_string = "Stinger: Smart Phone 2002";
                break;
        }
    }
    else if (ce_version.dwMajorVersion == 2 && ce_version.dwMinorVersion == 1)
    {
        version_string = 
        "Gryphon: Windows CE for P/PC V1 (Palm-size PC)"
        " / "
        "Apollo: Windows CE for A/PC V1 (Auto PC)";
    }
    /* End of the 'borrowed' code. */
    
    return version_string;
}

char* synce_get_platform_string ()
{ 
    char *platform_string;
    
    switch (ce_version.dwPlatformId)
    {
        case VER_PLATFORM_WIN32_CE:
            platform_string = "Windows CE";
            break;
        default:
            platform_string = "Unknown";
            break;
    }
    
    return platform_string;
}

char* synce_get_architecture_string ()
{ 
    char *architecture_string;
    char *architecture[] = {"Intel", "MIPS", "Alpha", "PPC", "SHX", "ARM", "IA64", "ALPHA64"};
    
    if (ce_system.wProcessorArchitecture > 8)
        architecture_string = "Unknown";
    else
        architecture_string = architecture[ce_system.wProcessorArchitecture];
                 
    return architecture_string;
}
    
char* synce_get_type_string ()
{ 
    char *type_string;
    
    switch (ce_system.dwProcessorType)
    {
        case PROCESSOR_STRONGARM:
            type_string = "StrongARM";
            break;
        case PROCESSOR_MIPS_R4000:
            type_string = "MIPS R4000";
            break;
        case PROCESSOR_HITACHI_SH3:  
            type_string = "SH3";
            break;
        default: 
            type_string = "Unknown";
            break;
    }

    return type_string;
}

char* synce_get_power_status ()
{ 
    char *status_string;
    
    switch (ce_power.ACLineStatus)
    {
        case AC_LINE_OFFLINE:
            status_string = "Offline";
            break;
        case AC_LINE_ONLINE:
            status_string = "Online";
            break;
        case AC_LINE_BACKUP_POWER:
            status_string = "Backup Power";
            break;
        case AC_LINE_UNKNOWN:       
            status_string = "Unknown";       
            break;
        default:
            status_string = "Invalid";
            break;
    }
    
    return status_string;
}

char* synce_get_main_battery_flag_string ()
{ 
    char *flag_string;
    
    switch (ce_power.BatteryFlag)
    {
        case BATTERY_FLAG_HIGH:
            flag_string = "High";
            break;
        case BATTERY_FLAG_LOW:
            flag_string = "Low";
            break;
        case BATTERY_FLAG_CRITICAL:
            flag_string = "Critical";
            break;
        case BATTERY_FLAG_CHARGING:
            flag_string = "Charging";
            break;
        case BATTERY_FLAG_NO_BATTERY:
            flag_string = "No Battery";
            break;
        default:
            flag_string = "Unknown";
            break;
    }

    return flag_string;
}

double synce_get_main_battery_level ()
{ 
    double percent;
    
    if (ce_power.BatteryLifePercent == BATTERY_PERCENTAGE_UNKNOWN)
        percent = 0.0;
    else
        percent = (double) ce_power.BatteryLifePercent / 100.0;
        
    return percent;
}

char* synce_get_backup_battery_flag_string ()
{ 
    char *flag_string;
    
    switch (ce_power.BackupBatteryFlag)
    {
        case BATTERY_FLAG_HIGH:
            flag_string = "High";
            break;
        case BATTERY_FLAG_LOW:
            flag_string = "Low";
            break;
        case BATTERY_FLAG_CRITICAL:
            flag_string = "Critical";
            break;
        case BATTERY_FLAG_CHARGING:
            flag_string = "Charging";
            break;
        case BATTERY_FLAG_NO_BATTERY:
            flag_string = "No Battery";
            break;
        default:
            flag_string = "Unknown";
            break;
    }

    return flag_string;
}

double synce_get_backup_battery_level ()
{ 
    double percent;
       
    if (ce_power.BackupBatteryLifePercent == BATTERY_PERCENTAGE_UNKNOWN)
        percent = 0.0;
    else
        percent = (double) ce_power.BackupBatteryLifePercent / 100.0;
        
    return percent;
}

char* synce_get_store_use_string ()
{ 
    char *store_string;
    
    store_string = malloc (sizeof (char) * 13);
    
    if (store_string == NULL)
    {
        strcpy (store_string, "?/? MB");
    }
    else
    {
        sprintf (store_string, "%i/%i MB", (ce_store.dwStoreSize - ce_store.dwFreeSize) / (1024 * 1024),
                 ce_store.dwStoreSize / (1024 * 1024));
    }
    
    return store_string;
}

double synce_get_store_use ()
{ 
    double percent;
    
    percent = (double) (ce_store.dwStoreSize - ce_store.dwFreeSize) / (double) ce_store.dwStoreSize;
        
    return percent;
}

char* synce_get_device_name_string ()
{ 
    LONG result;
    WCHAR *subkey_name_wide;
    WCHAR buffer[PATH_MAX];
    HKEY key;
    DWORD size;
    char *device_name;
    
    subkey_name_wide = wstr_from_ascii ("Ident");
    
    result = CeRegOpenKeyEx (HKEY_LOCAL_MACHINE, subkey_name_wide, 0, 0, &key);
    
    wstr_free_string (subkey_name_wide);
    
    if (result != ERROR_SUCCESS)
    {
        device_name = "Unknown";
        return device_name;
    }
    else
    {
        subkey_name_wide = wstr_from_ascii ("Name");
        
        size = sizeof (buffer);
        
        result = CeRegQueryValueEx (key, subkey_name_wide,
                                    NULL, NULL, (LPBYTE) buffer,
                                    &size);
        
        wstr_free_string (subkey_name_wide);
        
        if (result == ERROR_SUCCESS)
        {
            device_name = wstr_to_ascii (buffer);
        }
        else
        {
            device_name = "Unknown";
        }
        
        CeRegCloseKey (key);
    }
    
    return device_name;
}

char* synce_get_installed_programs_string ()
{ 
    char *installed_programs;
    char *application_string;
    LONG enum_result;
    LONG query_result;    
    LONG open_result;    
    HKEY parent_key;
    HKEY program_key;
    WCHAR *parent_key_name;
    WCHAR *value_name;
    WCHAR application_wide_string[MAX_PATH];
    DWORD i;
    DWORD installed_size;
    DWORD wide_string_size;
    BYTE installed;
    
    installed = 0;

    installed_programs = malloc (sizeof (char));
    
    installed_programs[0]='\0';

    parent_key_name = wstr_from_ascii ("Software\\Apps");
    
    value_name = wstr_from_ascii ("Instl");
    
    open_result = CeRegOpenKeyEx (HKEY_LOCAL_MACHINE, parent_key_name, 0, 0, &parent_key);

    if (open_result != ERROR_SUCCESS)
        return installed_programs;
        
    /*reg_result = CeRegQueryInfoKey (parent_key, NULL, NULL, NULL, number_of_subkeys, 
                                    NULL, NULL, NULL, NULL, NULL, NULL, NULL);*/
                               
    i = 0;
    
    do
    {
        wide_string_size = sizeof (WCHAR) * MAX_PATH;
        
        installed_size = sizeof (DWORD);

        enum_result = CeRegEnumKeyEx (parent_key, i, application_wide_string, &wide_string_size,
                                      NULL, NULL, NULL, NULL);
    
        if (enum_result != ERROR_SUCCESS)
            return installed_programs;
        
        open_result = CeRegOpenKeyEx (parent_key, application_wide_string, 0, 0, &program_key);

        if (open_result != ERROR_SUCCESS)
            return installed_programs;
        
        query_result = CeRegQueryValueEx (program_key, value_name, NULL, NULL, &installed, &installed_size);

        //if (query_result != ERROR_SUCCESS)
            //return installed_programs;
    
        CeRegCloseKey (program_key);

        if ((query_result == ERROR_SUCCESS) && installed)
        {
            application_string = wstr_to_ascii (application_wide_string);

            installed_programs = realloc (installed_programs, 
                                          (strlen (installed_programs) + strlen (application_string) + 2)
                                          * sizeof (char)); 

            strcat (installed_programs, application_string);

            strcat (installed_programs, ":");

            free (application_string);
        }

         i++;        
    } while (enum_result != ERROR_NO_MORE_ITEMS);
    
    CeRegCloseKey (parent_key);
    
    return installed_programs;
}

void synce_add_program (char *file_name)
{ 
    synce_create_directory ("\\Windows\\AppMgr");

    synce_create_directory ("\\Windows\\AppMgr\\Install");

    synce_copy_file (file_name);    

    synce_create_process ("wceload.exe", "");
}

void synce_remove_program (char *program_name)
{ 
    synce_create_process ("unload.exe", program_name);
}

int synce_copy_file (char *file_name)
{ 
    WCHAR *remote_file_name_wide; 
    //char remote_file_name[PATH_MAX];    
    FILE *local_file;
    HANDLE remote_file;
    int BUFFER_SIZE = 4 * 1024;
    char buffer[BUFFER_SIZE];
    struct stat file_info;
    size_t bytes_read = 0;
    size_t bytes_read_total = 0;
    size_t bytes_written = 0;
    size_t bytes_written_total = 0;
    GtkSynceProgressWindow *gpw;
    
    if (stat (file_name, &file_info) != 0)
        return 0;
    
    local_file = fopen (file_name, "r");

    if (local_file == NULL)
        return 0;
    
    /*strcpy (remote_file_name, "\\Windows\\AppMgr\\Install\\gtk-synce-install.cab");
    
    remote_file_name_wide = wstr_from_ascii (remote_file_name);*/

    remote_file_name_wide = wstr_from_ascii ("\\Windows\\AppMgr\\Install\\gtk-synce-install.cab");
    
    remote_file = CeCreateFile (remote_file_name_wide, GENERIC_WRITE,
                                0, NULL, CREATE_ALWAYS,
                                FILE_ATTRIBUTE_NORMAL, 0);

    if (remote_file == INVALID_HANDLE_VALUE)
        return 0;
    
    gpw = gtk_synce_progress_window_new ();
    
    gtk_widget_show_all (gpw->window);

    gtk_synce_progress_window_set_label (gpw, "Copying cab file to device.");
    
    while (bytes_read_total != file_info.st_size)
    {
        bytes_read = fread (buffer, sizeof (char), BUFFER_SIZE, local_file);
        
        if (CeWriteFile (remote_file, buffer, bytes_read, &bytes_written, NULL) == 0)
        {
            gtk_synce_progress_window_destroy (gpw);
            return 0;
        }
        
        bytes_read_total += bytes_read;
        bytes_written_total += bytes_written;
        
        gtk_synce_progress_window_set_fraction (gpw, 
            ((gdouble) bytes_written_total) / ((gdouble) file_info.st_size));
    }
    
    fclose (local_file);
    CeCloseHandle (remote_file);

    gtk_synce_progress_window_destroy (gpw);
    
    wstr_free_string (remote_file_name_wide);

    return 1;
}

int synce_create_directory (char *directory)
{ 
    WCHAR *directory_wide_string;

    directory_wide_string = wstr_from_ascii (directory);
    
    if (!CeCreateDirectory (directory_wide_string, NULL))
        return 0;

    wstr_free_string (directory_wide_string);
    
    return 1;
}

int synce_create_process (char *command, char *args)
{ 
    WCHAR *command_wide_string;
    WCHAR *args_wide_string;
    PROCESS_INFORMATION info;

    command_wide_string = wstr_from_ascii (command);
    args_wide_string = wstr_from_ascii (args);

    memset (&info, 0, sizeof (info));

    if (!CeCreateProcess (command_wide_string,
                          args_wide_string,
                          NULL, NULL, FALSE,
                          0, NULL, NULL, NULL, &info))
        return 0;

    CeCloseHandle (info.hProcess);
    CeCloseHandle (info.hThread);

    wstr_free_string (command_wide_string);
    wstr_free_string (args_wide_string);

    return 1;
}
