#include <stdlib.h>
#include <stdio.h>

#include "array2d.h"

// ARRAY CONSTRUCTOR
// allocated memory for struct and array, then calls array initializer
Array2d* Array2d_new(int width, int height, int size) {
    if (width <= 0 || height <= 0 || size <= 0) {
        return NULL;
    }

    // allocate memory for 2d array stuct
    Array2d* array2d = malloc(sizeof(*array2d));
    // pass:
    // 1. pointer to struct
    // 2. width, height, and unit size
    // 3. a new allocation of h*w*s bytes
    // to the array initializer
    Array2d_init(array2d, width, height, size, malloc(width * height * size));

    return array2d;
}

// ARRAY INITIALIZATION FUNCTION
// assigns values in the array struct
void Array2d_init(Array2d* array2d, int width, int height, int size, void* array) {
    array2d->width = width;
    array2d->height = height;
    array2d->size = size;
    array2d->arr = array;
    return;
}

// ARRAY DECONSTRUCTOR
// free memory allocated for:
// 1. array struct memory
// 2. array struct
void Array2d_free(Array2d* arr) {
    free(arr->arr);
    free(arr);
    return;
}

// GET WIDTH
int Array2d_width(Array2d* arr) {
    return arr->width;
}

// GET HEIGHT
int Array2d_height(Array2d* arr) {
    return arr->height;
}

// GET UNIT SIZE
int Array2d_size(Array2d* arr) {
    return arr->size;
}

// AT
// returns a pointer to the [i,j] index within the array
// i indicates row, and j indicates column
// changing/retriving a value in the array will follow the process:
// 1. call AT function
// 2. assign value/dereference pointer
void* Array2d_at(Array2d* arr, int i, int j) {
    int width = arr->width;
    int height = arr->height;
    int size = arr->size;
    if (i >= height || j >= width) {
        return NULL;
    }
    return (void*)( (char*)arr->arr + (i * width * size) + (j * size) );
}
