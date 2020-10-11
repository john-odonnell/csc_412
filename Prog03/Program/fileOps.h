#ifndef FILEOPS_INCLUDED
#define FILEOPS_INCLUDED

#include "array2d.h"

extern Array2d* readFile(char* filename);
extern char** recurseDir(char* path, int* totalFiles);

#endif
