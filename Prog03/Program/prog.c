#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>

#include "array2d.h"
#include "fileOps.h"

// PRINT 2D ARRAY
// used in testing to output a 2d array
// used to manually validate input gathering function
// RETURN VOID
void printArr(Array2d* arr) {
    int w = Array2d_width(arr);
    int h = Array2d_height(arr);

    printf("~~Array~~\nw:%d h:%d\n", w, h);

    char *placehold;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            placehold = Array2d_at(arr, i, j);
            printf("%c ", *placehold);
        }
        printf("\n");
    }
}

// PATTERN CHECK
// given a certain (i,j), determine if the pattern given matches
// a portion of the image the same size as the pattern where
// the top left corner is the (i,j) idx
// RETURN VALUES
// : TRUE  if the pattern matches at (i,j)
// : FALSE if the pattern does not match
bool patternCheck(Array2d* pat, Array2d* img, int i, int j) {
    int pWidth = Array2d_width(pat);
    int pHeight = Array2d_height(pat);
    int iWidth = Array2d_width(img);
    int iHeight = Array2d_height(img);

    // if (i,j) does not accomodate the space the size of the pattern, return false
    if (j + pWidth > iWidth || i + pHeight > iHeight) {
        return false;
    }

    // loop through the pattern and the image
    // if any of the entries are not the same, return false
    for (int m = 0; m < pHeight; m++) {
        for (int n = 0; n < pWidth; n++) {
            if (*(char*)Array2d_at(pat, m, n) != *(char*)Array2d_at(img, i+m, j+n)) {
                return false;
            }
        }
    }

    // if all the tests pass, return true
    return true;
}

// FIND MATCHES
// returns a list of (i,j) pairs representing points in the image where the pattern matchs
// RETURN VALUES
// NULL if the image does not contain any pattern matches
// INT* if the image contains at least one pattern match
int* findMatches(Array2d* pat, Array2d* img) {
    int iWidth = Array2d_width(img);
    int iHeight = Array2d_height(img);
    
    // allocate enough spaces for as many pattern matches as possible
    int totalMatches = 0;
    int* matches = malloc(sizeof(int) * ((iWidth *  iHeight * 2) + 1));

    // loop through the image
    // check each (i,j) index with patternCheck func
    for (int i = 0; i < iHeight; i++) {
        for (int j = 0; j < iWidth; j++) {
            if (patternCheck(pat, img, i, j)) {
                totalMatches++;
                matches[(totalMatches * 2) - 1] = i;
                matches[totalMatches * 2] = j;
                printf("totMat: %d\n", totalMatches);
            }
        }
    }
    
    // insert match counter and reallocate the matches array
    matches[0] = totalMatches;
    matches = realloc(matches, sizeof(int) * ((totalMatches * 2) + 1));

    return matches;
}

// PRINT MATCHES ARRAY
// mainly used in testing to output the array of pattern matches
// can be repurposed to write to a file
// RETURN VOID
void printMatches(int* matches, char* filename, char* outputFile, int pass) {
    int len = (matches[0] * 2) + 1;

    FILE* stream;

    if (pass == 0) {
        stream = fopen(outputFile, "w");
    } else {
        stream = fopen(outputFile, "a");
    }

    printf("~~PRINT PROCESS~~\n");
    printf("out file: %s\n", outputFile);
    printf("cur file: %s\n", filename);
    
    fprintf(stream, "%s\n    ", filename);

    for (int i = 0; i < len; i++) {
        fprintf(stream, "%d", matches[i]);
        if (i != len-1) {
            fprintf(stream, " ");
        }
    }
    fprintf(stream, "\n");

    fclose(stream);

    return;
}


// MAIN FUNCTION
// INPUT FORMAT: ./<exec> <pat> <dir> <out>
// : <exec> is the compiled executable
// : <pat>  is the path to a .pat file describing the desired pattern
// : <dir>  is the path to a directory to search through for .img files
// : <out>  is the path to an output directory
// FUNCTION FLOW
int main(int argc, char* argv[]) {
    if (argc < 3) {
    printf("INPUT FORMAT: ./<exec> <pat> <dir> <out>\n");
    printf("\t<exec> is the compiled executable\n");
    printf("\t<pat>  is the path to a .pat file describing the desired pattern\n");
    printf("\t<dir>  is the path to a directory to search through for .img files\n");
    printf("\t<out>  is the path to an output directory\n");
    return 1;
    }

    // isolate pattern filename
    int idxFinalSlash = 0;
    int idxFileExtension = 0;
    for (int i = 0; i < strlen(argv[1]); i++) {
        if (argv[1][i] == '/') {
            idxFinalSlash = i;
        } else if (argv[1][i] == '.') {
            idxFileExtension = i;
        }
    }
    int idxPatternFilename = idxFinalSlash + 1;
    char* patternFilename = calloc(1, sizeof(char) * (strlen(argv[1]) - idxFinalSlash));
    strcpy(patternFilename, argv[1] + idxPatternFilename);
    patternFilename[idxFileExtension - idxPatternFilename] = '\0';
    printf("pattern filename: %s\n", patternFilename);
    
    // set output filename
    char* outputFilenameAddition = "_Matches.txt";
    char* outputFilePath = calloc(1, sizeof(char*) * (strlen(argv[3]) + strlen(patternFilename) + 9));
    
    memcpy(outputFilePath, argv[3], strlen(argv[3]));
    printf("out dir: %s\n", argv[3]);
    if (argv[3][strlen(argv[3])-1] != '/') {
        strcat(outputFilePath, "/");
    }
    strcat(outputFilePath, patternFilename);
    strcat(outputFilePath, outputFilenameAddition);
    outputFilePath = realloc(outputFilePath, sizeof(char)*(strlen(outputFilePath) + 1));

    // printf("%s\n", outputFilePath);
    printf("out file: %s\n", outputFilePath);



    // get a list of filenames in the <dir> directory of the .img type
    int totalFiles = 0;
    printf("txt dir: %s\n", argv[2]);
    if (argv[2][strlen(argv[2])-1] != '/') {
        strcat(argv[2], "/");
    } 
    printf("txt dir: %s\n", argv[2]);
    char** files = recurseDir(argv[2], &totalFiles);

    
    // read pattern into 2d arr
    Array2d* pat = readFile(argv[1]);
    for (int i = 0; i < totalFiles; i++) {
        char* filepath = calloc(1, sizeof(char) * (strlen(argv[2]) + 1));
        strcpy(filepath, argv[2]);
        filepath = realloc(filepath, sizeof(char) * (strlen(argv[2]) + 1 + strlen(files[i])));
        strcat(filepath, files[i]);
        printf("filepath: %s\n", filepath);

        Array2d* img = readFile(filepath);
        int* matches = findMatches(pat, img);
        
        printMatches(matches, files[i], outputFilePath, i);

        free(filepath);
        Array2d_free(img);
        free(matches);
    }

    // read image into 2d arr
    // Array2d* img = readFile(argv[2]);
    
    // printf("Pat Pointer: %p\n", pat);
    // printArr(pat);
    // printf("Img Pointer: %p\n", img);
    // printArr(img);

    // int* matches = findMatches(pat, img);
    // printMatches(matches);
    // free(matches);

    for (int i = 0; i < (totalFiles * 2) + 1; i++) {
        free(files[i]);
    }

    free(patternFilename);
    free(outputFilePath);
    free(files);
    Array2d_free(pat);
    // Array2d_free(img);
    // printf("Memory cleared.\n");

    // recurseDir(argv[3]);

    return 0;
}
