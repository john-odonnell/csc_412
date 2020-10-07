#include <stdlib.h>
#include <stdio.h>

#include "array2d.h"

Array2d Array2d_new(int width, int height, int size) {
    Array2d array2d;
    // printf("~~Struct Size~~\nint: %ld\nvd*: %ld\ntot: %ld\n", sizeof(int), sizeof(void*), sizeof(*array2d));

    array2d = malloc(sizeof(*array2d));

    array2d->width = width;
    array2d->height = height;
    array2d->size = size;

    // printf("~~Array Creation~~\nHgt: %d\nWdt: %d\nSiz: %d\n", array2d->height, array2d->width, array2d->size);

    void *arr = calloc(1, width*height*size);
    array2d->arr = arr;

    return array2d;
}

void Array2d_free(Array2d *arr) {
    free((*arr)->arr);
    free(*arr);
    return;
}

int Array2d_width(Array2d arr) {
    return arr->height;
}

int Array2d_height(Array2d arr) {
    return arr->width;
}

int Array2d_size(Array2d arr) {
    return arr->size;
}

void *Array2d_at(Array2d arr, int i, int j) {
    return arr->arr + (i * arr->width * arr->size) + (j * arr->size);
}
