#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

#include "fileOps.h"
#include "array2d.h"

// READ FILE INTO CUSTOM 2D ARRAY
//
// RETURN VALUES
// Array2d* if the given filename is valid
// NULL     otherwise
Array2d* readFile(char* filename) {
    // open filestream with the filename
    FILE *stream = fopen(filename, "r");
    
    if (stream == NULL) {
        return NULL;
    }

    // pull the width and height specs from the file
    int w, h;
    fscanf(stream, "%d %d\n", &w, &h);

    Array2d* arr = Array2d_new(w, h, sizeof(char));
    if (arr == NULL) {
        return NULL;
    }
    
    // pull the rest of file and arrange it in a 2d array
    char placeholder;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            placeholder = getc(stream);
            char *arrIdx = (char*)Array2d_at(arr, i, j);
            *arrIdx = placeholder;
        }
        if (i != h-1) {
            // burn newline
            getc(stream);
        }
    }
    
    // close filestream
    fclose(stream);

    // return pointer to array struct
    return arr;
}

// RECURSE THROUGH DIRECTORY
// opens a directory, and fills an array with filenames of all .img files
// RETURN VALUES
// NULL   if the directory is invalid, or if the directory contains no .img files
// CHAR** if the directory contains at least on e .img file
char** recurseDir(char* path, int* totalFiles) {
    // open and validate directory stream
    DIR* dir = opendir(path);
    if (dir == NULL) {
        printf("Invalid path: %s\n", path);
        return NULL;
    }
    
    struct dirent* entry;
    *totalFiles = 0;

    // loop to determine length of return array
    while ((entry = readdir(dir)) != NULL) {
        // pull name and file type from struct
        const char* name = entry->d_name;
        unsigned char type = entry->d_type;
        // isolate file extension
        char* thisExt = calloc(1, sizeof(char) * 5);
        int nameLen = strlen(name);
        memcpy(thisExt, &name[nameLen-4], 4);

        // if the entry
        // : is a normal type file
        // : is not the '.' or '..' directories
        // : has the file extension .img
        // increment the file counter
        if ((name[0] != '.') && (type == DT_REG) && (!strcmp(".img", thisExt))) {
            *totalFiles = *totalFiles + 1;
        }

        free(thisExt);
    }

    closedir(dir);

    char** files = (char**)calloc(1, ((*totalFiles * 2) + 1)* sizeof(char*));

    int i = 0;
    dir = opendir(path);
    while ((entry = readdir(dir)) != NULL) {
        // pull name and file type from struct
        char* name = entry->d_name;
        unsigned char type = entry->d_type;
        // isolate file extension
        char* thisExt = calloc(1, sizeof(char) * 5);
        int nameLen = strlen(name);
        memcpy(thisExt, &name[nameLen-4], 4);

        // if the entry
        // : is a normal file type
        // : is not the '.' or '..' directories
        // : has the file extension .img
        // allocate a new filenmae string and add it to the array of files
        // cope the filename into the allocation
        // increment the file counter
        if ((name[0] != '.') && (type == DT_REG) && (!strcmp(".img", thisExt))) {
            files[i] = (char*)malloc(strlen(name)+1);
            strcpy(files[i], name);
            i++;
        }

        free(thisExt);
    }

    closedir(dir);

    return files;
}
