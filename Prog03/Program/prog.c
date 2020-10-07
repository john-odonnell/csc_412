#include <stdlib.h>
#include <stdio.h>

#include "array2d.h"

Array2d imageInput(char* filename) {
    // open filestream with the filename
    FILE *image = fopen(filename, "r");
    
    // pull the width and height specs from the file
    int w, h;
    char placeholder;

    fscanf(image, "%d %d\n", &w, &h);
    // printf("%d, %d\n", w, h);

    // pull the rest of file and arrange it in a 2d array
    Array2d arr = Array2d_new(w, h, sizeof(char));
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            placeholder = getc(image);
            // printf("%c\t", placeholder);
            char *arrIdx;
            arrIdx = (char*)Array2d_at(arr, i, j);
            // printf("%p\t", (void*)arrIdx);
            *arrIdx = placeholder;
        }
        getc(image);
    }
    
    fclose(image);
    return arr;
}

void printArr(Array2d arr) {
    int w = arr->width;
    int h = arr->height;

    char *placehold;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            placehold = Array2d_at(arr, i, j);
            printf("%c ", *placehold);
        }
        printf("\n");
    }
}

int main(int argc, char* argv[]) {
    
    // Array2d arr = Array2d_new(5, 5, sizeof(int));


    Array2d arr = imageInput(argv[1]);
    printf("Arr Pointer: %p\n", arr);
    printArr(arr);

    Array2d_free(&arr);

    return 0;
}
