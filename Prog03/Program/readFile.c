#include <stdlib.h>
#include <stdio.h>

#include "readFile.h"
#include "array2d.h"

// READ FILE INTO CUSTOM 2D ARRAY
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
