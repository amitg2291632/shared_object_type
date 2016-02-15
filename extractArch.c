/*********************************************************
 * Author :- Amit Garg
 * Date :- 10/28/2015
 * File Name :- extractArch.c 
 
 * This file is part of Assignment 
 * This code is simple program for unzip the apk file and read folder file by file 
 **********************************************************/

#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include "junzip.h"
#include "type.h"

/*=====================Macro Definitions==================*/

#define BUF_LEN    1024

/*=====================Global Variables==================*/

char folder_name[BUF_LEN];
int  count_dot = 0;
	
/*=====================Function Definitions==================*/

void listdir(const char *name, int level)
{
    char full_path[BUF_LEN] = "";
    char full_File_path[BUF_LEN] = "";
    char path[BUF_LEN] = "";
    int len = 0;
    struct dirent *entry = NULL;
    DIR *dir = NULL;

    if (!(dir = opendir(name)))
    {
        printf("Not able to open dir %s",name );
    }

    if (!(entry = readdir(dir)))
    {
        return;
    }

    do
    {
        if (DT_DIR == entry->d_type)
        {
            len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
            path[len] = 0;
            if ((0 == strcmp(entry->d_name, ".")) || (0 == strcmp(entry->d_name, "..")))
            {
                continue;
            }
            strcpy(full_path, path);
            listdir(path, level + 1);
        }
        else
        {
            if (!strcmp(full_path,""))
            {
                snprintf(full_File_path, sizeof(full_File_path), "%s/%s", name, entry->d_name);
                type(full_File_path);
            }
            else
            {					
                snprintf(full_File_path, sizeof(full_File_path), "%s/%s", name, entry->d_name);
                type(full_File_path);
            }
        }
   } while ((entry = readdir(dir)));

    closedir(dir);
    return;
} /* end of listdir() */
/*-------------------------------------------------------*/

void trimapkname(char *s)
{
    int i = 0, j = 0;

    while ('\0' != s[i])
    {
        if (('/' == s[i]) || ('.' == s[i]))
        {
            i++;
            continue;
        }
        else
        {
            folder_name[j++] = s[i];
        }
        i++; 
    }
    return;
} /* end of trimapkname() */
/*-------------------------------------------------------*/

int main(int argc, char *argv[])
{
	 char a_ch_sysCmd[1024];
	int unZipStatus=0;
    if (2 > argc)
    {
        puts("Usage: extractArch file.zip or apk ");
        return -1;
    }
	
    trimapkname(argv[1]);
    unZipStatus = unzip_app(argc,argv,folder_name);
	
	/*
	Sometimes this https://github.com/jokkebk/JUnzip.git is not able to extract taet.apk  in this case i am unzipping apk file with system(unzip) utility
	we can use other third party unzip library 
	*/
		if(unZipStatus == 1)
		{
			snprintf(a_ch_sysCmd, 1024, "rm -rf %s >> /dev/null", folder_name );
			system(a_ch_sysCmd);
			snprintf(a_ch_sysCmd, 1024, "unzip %s  -d %s >> /dev/null"  , argv[1],folder_name );
			system(a_ch_sysCmd);
		}
    listdir(folder_name, 0);

    return 0;
} /* end of main() */
/*-------------------------------------------------------*/
