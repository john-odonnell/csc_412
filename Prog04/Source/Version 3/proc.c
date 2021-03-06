#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 * Comparison Function used for QSort
 *
 * Compares two elements, which are filepaths. Opens each file and saved their
 * respective line numbers. The elements will be sorted based on these line numbers.
 *
 * @param a A first given list element
 * @param b A second fiven list element
 *
 * @return an integer value based on the difference between elements
 */
int cmpfunc(const void* a, const void* b) {
    FILE* stream;
    int aProc, bProc;
    int aLine, bLine;
    
    stream = fopen(*(char**)a, "r");
    if (stream == NULL) {
        printf("File %s could not be opened!\n", *(char**)a);
    }
    fscanf(stream, "%d %d ", &aProc, &aLine);
    fclose(stream);

    stream = fopen(*(char**)b, "r");
    if (stream == NULL) {
        printf("File %s could not be opened!\n", *(char**)b);
    }
    fscanf(stream, "%d %d ", &bProc, &bLine);
    fclose(stream);

    return (aLine - bLine);
}

/**
 * Processing Process
 *
 * This process is responsible for ordering the lines of code that this process
 * is responsible for. A given process of ID <id> opens all files created by
 * the distribution processes of form <n>.<id>, which all contain filepaths.
 * Each filepath is inserted into an array, and the array is then ordered by qsort
 * based on line number. These lines of code are then inserted in their sorted
 * order into a file <id>.processed.
 *
 * @param processId ID for this process
 * @param numProcesses Total number of processes
 * @param execPath Path to this process' executable file
 *
 * @return 0 if successful, 1 if failure
 */
int main(int argc, char* argv[]) {
    // pull args
    int processId = atoi(argv[0]);
    int numProcesses = atoi(argv[1]);
    char* execPath = argv[2];
    
    // order the files by index
    // store their outputs sequentially in a string
    // return the long string
    
    // total file count for this process
    int idx = 0;

    // allocate array for ordered files
    char** toOrder = malloc(1024 * sizeof(char*));

    // insert all files into single array
    FILE* stream;
    for (int i = 0; i < numProcesses; i++) {
        // establish filename <sourcePID>.<thisPID>
        char* filename = calloc(1, sizeof(char) * 1024);
        char* source = calloc(1, sizeof(char) * 10);
        char* destination = calloc(1, sizeof(char) * 10);
        sprintf(source, "%d", i);
        sprintf(destination, "%d", processId);
        strcat(filename, execPath);
        strcat(filename, source);
        strcat(filename, ".");
        filename = realloc(filename, sizeof(char) * (strlen(filename) + strlen(source) + strlen(destination) + 2));
        strcat(filename, destination);
        

        // open filestream
        stream = fopen(filename, "r");
        if (stream == NULL) {
            printf("File %s could not be opened!", filename);
            exit(1);
        }

        // read each line from the file, strip the newline and insert into the array
        char* placeholder = calloc(1, sizeof(char) * 1024);
        while (fgets(placeholder, 1024, stream)) {
            toOrder[idx] = calloc(1, sizeof(char) * 1024);
            strcpy(toOrder[idx], placeholder);
            toOrder[idx][strlen(toOrder[idx])-1] = '\0';
            toOrder[idx] = realloc(toOrder[idx], sizeof(char) * (strlen(toOrder[idx]) + 1));

            idx++;
        }

        fclose(stream);
        free(placeholder);

        if (remove(filename) != 0) {
            printf("Unable to delete file %s\n", filename);
        }

        free(destination);
        free(filename);
    }

    // order array
    qsort(toOrder, idx, sizeof(char*), cmpfunc);

    // output strings to <processId>.processed
    // create output filename
    char* outputFilename = calloc(1, sizeof(char) * 1024);
    char* thisProcess = calloc(1, sizeof(char) * 10);
    sprintf(thisProcess, "%d", processId);
    strcat(outputFilename, execPath);
    strcat(outputFilename, thisProcess);
    outputFilename = realloc(outputFilename, sizeof(char) * (strlen(outputFilename) + 10 + 1));
    strcat(outputFilename, ".processed");
    // loop through array of filenames
    int procId, line;
    FILE* to_file = fopen(outputFilename, "w");
    if (to_file == NULL) {
        printf("File %s could not be opened!\n", outputFilename);
    }
    for (int i = 0; i < idx; i++) {
        char* code = calloc(1, sizeof(char) * 4096);
        // open each file and read its line of code
        stream = fopen(toOrder[i], "r");
        fscanf(stream, "%d %d", &procId, &line);
        fgets(code, 4096, stream);
        fclose(stream);
        // reallocate code string
        code = realloc(code, sizeof(char) * (strlen(code) + 1));

        // output line of code
        fputs(code, to_file);
        free(code);
    }
    fclose(to_file);

    free(outputFilename);

    for (int i = 0; i < idx; i++) {
        free(toOrder[i]);
    }
    free(toOrder);

    exit(0);
}
