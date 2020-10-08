#ifndef ARRAY2D_INCLUDED
#define ARRAY2D_INCLUDED

typedef struct Array2d {
    int width, height, size;
    void* arr;
} Array2d;

// constructor and destructor functions
extern Array2d* Array2d_new(int width, int height, int size);
extern void Array2d_init(Array2d* array2d, int width, int height, int size, void* arrary);
extern void Array2d_free(Array2d* array2d);

// get array details functions
extern int Array2d_height(Array2d* array2d);
extern int Array2d_width(Array2d* array2d);
extern int Array2d_size(Array2d* array2d);

// function for retreiving or changing array storage
extern void* Array2d_at(Array2d* array2d, int i, int j);

#endif
