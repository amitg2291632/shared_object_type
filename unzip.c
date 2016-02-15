 /*
Open source code taken from  https://github.com/jokkebk/JUnzip.git
I have done some customization  according to my requirement 
 Simple demo for JUnzip library. Needs zlib. Should compile with
 something like gcc junzip_demo.c junzip.c -lz -o junzip.exe
 */

#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>
#include <string.h>
#include "junzip.h"


#include <sys/stat.h>
#include <sys/types.h>

char newfolder[1024];

int check_if_not_unzip_properly=0;

// Replace this with a function that creates the directory if necessary
// and returns 1 if you want directory support
int makeDirectory(char *dir) {
  //  printf("mkdir(%s)\n", dir);
    //return 0;
    // For MinGW (requires io.h)
    mkdir(dir, 0700);
    return 1;
}

void writeFile(char *filename, void *data, long bytes) {
    FILE *out;
    int i;

    // simplistic directory creation support
    for(i=0; filename[i]; i++) {
        if(filename[i] != '/')
            continue;

        filename[i] = '\0'; // terminate string at this point

        if(!makeDirectory(filename)) {
            fprintf(stderr, "Couldn't create subdirectory %s!\n", filename);
            return;
        }

        filename[i] = '/'; // Put the separator back
    }

    if(!i || filename[i-1] == '/')
        return; // empty filename or directory entry

    out = fopen(filename, "wb");

    if(out != NULL) {
        fwrite(data, 1, bytes, out); // best effort is enough here
        fclose(out);
    } else {
        fprintf(stderr, "Couldn't open %s for writing!\n", filename);
    }
}

int processFile(FILE *zip) {
    JZFileHeader header;
    char filename[1024];
    unsigned char *data;

    if(jzReadLocalFileHeader(zip, &header, filename, sizeof(filename), newfolder)) {
        printf("Couldn't read local file header!");
        return -1;
    }

    if((data = (unsigned char *)malloc(header.uncompressedSize)) == NULL) {
        printf("Couldn't allocate memory!");
        return -1;
    }

    //printf("%s, %d / %d bytes at offset %08X\n", filename,header.compressedSize, header.uncompressedSize, header.offset);
	
	if (strstr(filename,".so") != NULL)
	{
	    if(header.compressedSize == 0)
			check_if_not_unzip_properly=1;
		}

    if(jzReadData(zip, &header, data) != Z_OK) {
        printf("Couldn't read file data!");
        free(data);
        return -1;
    }

    writeFile(filename, data, header.uncompressedSize);
    free(data);

    return 0;
}

int recordCallback(FILE *zip, int idx, JZFileHeader *header, char *filename) {
    long offset;

    offset = ftell(zip); // store current position

    if(fseek(zip, header->offset, SEEK_SET)) {
        printf("Cannot seek in zip file!");
        return 0; // abort
    }

    processFile(zip); // alters file offset

    fseek(zip, offset, SEEK_SET); // return to position

    return 1; // continue
}

int unzip_app(int argc, char *argv[], char *foldername) {
    FILE *zip;
    JZEndRecord endRecord;

	strcpy(newfolder,foldername);
    if(argc < 2) {
        puts("Usage: junzip_demo file.zip");
        return -1;
    }

    if(!(zip = fopen(argv[1], "rb"))) {
        printf("Couldn't open \"%s\"!", argv[1]);
        return -1;
    }

    if(jzReadEndRecord(zip, &endRecord)) {
        printf("Couldn't read ZIP file end record.");
        goto endClose;
    }

    if(jzReadCentralDirectory(zip, &endRecord, recordCallback)) {
        printf("Couldn't read ZIP file central record.");
        goto endClose;
    }

    //Alternative method to go through zip after opening it:
    //while(!processFile(zip)) {}

 	endClose:
    fclose(zip);

    return check_if_not_unzip_properly;
}
