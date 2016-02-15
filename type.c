/*********************************************************
 * Author :- Amit Garg
  Date :- 10/28/2015
 * 
 * This file is part of Assignment 
 * This code is simple program for unix simplified FILE utility for getting shared object architecture information
 * I am using libmagic utility which i from ftp://ftp.astron.com/pub/file/file-5.25.tar.gz
 i compiled this file utility and customize it and found that from libmagic we can get file information.
 
 *  **********************************************************/

#include <unistd.h>
#include <stdlib.h>
#include "type.h"

int type(const char * filename)
{
    const char *archType = NULL;
    char line[128];
    char a_ch_sysCmd[1024];
    struct magic_set *magic = NULL;

    FILE *file = NULL;
    magic = magic_open(0);
    magic_load(magic, NULL);
    archType = magic_file(magic, filename);
    //printf("%s %s\n",filename,archType);
 
    if (strstr(archType,"shared object, ARM") != NULL)
    {
        if (NULL != strstr(archType,"32-bit"))
        {
            snprintf(a_ch_sysCmd, 1024, "readelf -a -w %s | grep -i Tag_CPU_arch | awk '{print $2}' > armeabitype ", filename);
            system(a_ch_sysCmd);
            file = fopen ( "armeabitype", "r");
            if (NULL !=file)
            {
                /* Reading first line as we are writing only  line  for clarification of armeabi and armeabi-v7a in this case we have to check Cpu architecture so readelf can check 
					I am trying to read elf header but it eill take time so i used system() */
                fgets(line, sizeof line, file); /* read a line */
                fclose(file);
                system("rm -rf armeabitype ");
            }
            else
            {
                printf("File does not exist!!"); /* why not exist  */
            }

            if (NULL != strstr(line,"v7"))
            {
                printf("%s archtype -armeabi-v7a\n",filename);
            }
            else if (NULL != strstr(line,"v5TE"))
            {
                printf("%s archtype -armeabi\n",filename);
            }
        }	
        else if (NULL != strstr(archType,"64-bit"))
        {
            printf("%s archtype -arm64-v8a\n",filename);
        }
    }
    else if (NULL != strstr(archType,"shared object, Intel"))
    {
        if (NULL != strstr(archType,"32-bit"))
        {
            printf("%s archtype -x86\n",filename);
        }
    }
    else if (NULL != strstr(archType,"shared object, x86-64"))
    {
        printf("%s archtype -x86_64\n",filename);
    }
	else if (NULL != strstr(archType,"shared object, MIPS"))
    {
        if (NULL != strstr(archType,"32-bit"))
        {
            printf("%s archtype -mips\n",filename);
        }
		else      
        printf("%s archtype -mips64\n",filename);
    }

    magic_close(magic);

    return 0;
} /* end of type() */ 
/*-------------------------------------------------------*/
