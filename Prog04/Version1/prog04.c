// URI CSC 412 OPERATING SYSTEMS : VERSION 1 OF PROGRAM 4
// one process does all the work
// args : number of child processes n, path to data directory path

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

// recurse through a directory
// add all regular type files to an array
// return fully populated array
char** recurseDirectory(char* path, int* totalFiles) {
    // open and validate directory stream
    DIR* dir = opendir(path);
    if (dir == NULL) {
        printf("Invalid path: %s\n", path);
        return NULL;
    }

    struct dirent* entry;
    *totalFiles = 0;

    // count the total number of files in the directory
    while ((entry = readdir(dir)) != NULL) {
        const char* name = entry->d_name;
        unsigned char type = entry->d_type;

        if ((name[0] != '.') && (type == DT_REG)) {
            *totalFiles = *totalFiles + 1;
        }
    }

    closedir(dir);

    char** files = (char**)calloc(1, ((*totalFiles * 2) + 1) * sizeof(char*));

    // add all files in the directory to a list of filenames
    int i = 0;
    dir = opendir(path);
    while((entry = readdir(dir)) != NULL) {
        char* name = entry->d_name;
        unsigned char type = entry->d_type;

        if ((name[0] != '.') && (type == DT_REG)) {
            files[i] = (char*)calloc(1, strlen(path) + strlen(name) + 1);
            strcat(files[i], path);
            strcat(files[i], name);
            i++;
        }
    }

    closedir(dir);

    // return the list of filenames
    return files;
}

void childProcess(char** files, int start, int interval, char**** lists, int** lengths, int numProcesses, int proc){
    // allocate memory for lists of files for each process
    char*** sublists = malloc(numProcesses * sizeof(void*));
    int* filesPerProcess = calloc(1, numProcesses * sizeof(int));

    // TODO CHILD PROCESS LOGIC
    FILE *stream;
    int procId = 0;
    for (int i = start; i < start + interval; i++) {
        // scan file for process id
        // increment counter at index :id in filePerProcess counter
        stream = fopen(files[i], "r");
        if (stream == NULL) {
            printf("File %s could not be opened!\n", files[i]);
            return;
        }
        fscanf(stream, "%d ", &procId);
        filesPerProcess[procId] = filesPerProcess[procId] + 1;
        fclose(stream);
    }

    for (int i = 0; i < numProcesses; i++) {
        // allocate lists based on filePerProcess counter
        sublists[i] = malloc(sizeof(void*) * filesPerProcess[i]);
    }

    char*** ptrs = malloc(sizeof(char*) * numProcesses);
    for (int i = 0; i < numProcesses; i++) {
        ptrs[i] = &sublists[i][0];
    }
    for (int i = start; i < start + interval; i++) {
        // insert each file into the process files list
        stream = fopen(files[i], "r");
        fscanf(stream, "%d ", &procId);
        fclose(stream);
        *ptrs[procId] = files[i];
        ptrs[procId] = ptrs[procId] + 1;
    }

    free(ptrs);

    // assign pointer to process file list into parent array
    lists[proc] = sublists;
    lengths[proc] = filesPerProcess;
}

// comparison function for qsort
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

char* processing(char*** lists, int** lengths, int processId, int numProcesses) {
    // order the files by index
    // store their outputs sequentially in a string
    // return the long string
    
    // find total file count for this process
    int fileCount = 0;
    for (int i = 0; i < numProcesses; i++) {
        fileCount += lengths[i][processId];
    }

    // allocate array for ordered files
    char** toOrder = malloc(fileCount * sizeof(char*));

    // insert all files into single array
    int idx = 0;
    for (int i = 0; i < numProcesses; i++) {
        int len = lengths[i][processId];
        for (int j = 0; j < len; j++) {
            toOrder[idx] = lists[i][j];
            idx++;
        }
    }

    // order array
    qsort(toOrder, fileCount, sizeof(char*), cmpfunc);

    // concatenate strings
    char* output = malloc(sizeof(char));
    output[0] = '\0';
    FILE* stream;
    int procId, line;
    for (int i = 0; i < fileCount; i++) {
        char* code = calloc(1, sizeof(char) * 100);

        stream = fopen(toOrder[i], "r");
        fscanf(stream, "%d %d", &procId, &line);
        fgets(code, 100, stream);
        fclose(stream);

        output = realloc(output, sizeof(char) * (strlen(output) + strlen(code) + 1));
        strcat(output, code);
        free(code);
    }

    free(toOrder);

    return output;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("usage: ./a.out <n> <path>\n");
        printf(" <n>    : number of child processes\n");
        printf(" <path> : path to directory of files\n");
        return 1;
    }

    // program arguments
    // numProcesses : the number of child processes that the parent will spawn
    // path : the path in the filesystem to a directory of files to be processed
    int numProcesses;
    sscanf(argv[1], "%d", &numProcesses);
    char* path = argv[2];

    // fill an array with all the files in the directory
    int numFiles = 0;
    char** files = recurseDirectory(path, &numFiles);

    int interval = numFiles / numProcesses;
    int remainder = numFiles % numProcesses;

    // pointer to an array of numProcesses pointers
    // these pointers will point to groups of arrays returned by child processes
    char**** lists = malloc(numProcesses * sizeof(void*));
    int**   lengths = malloc(numProcesses * sizeof(void*));

    for (int i = 0; i < numProcesses; i++) {

        // define index of file to start processing with
        int start = i * interval;
        // define interval length, accounting for the last child
        // process being responsible for the remainder of integer division
        int j;
        if (i == numProcesses - 1) {
            j = interval + remainder;
        } else {
            j = interval;
        }

        // child process is passed
        // : total list of files
        // : index to start processing
        // : length of processing interval
        // : pointer to index in lists group
        // result
        // : creates a list of pointers to arrays
        // : : each array contains the files in the process's section
        // : : of the list that belongs to Process :IDX
        // : a pointer to the list is inserted into the process'
        // : location in the lists group
        childProcess(files, start, j, lists, lengths, numProcesses, i);
    }

    char* totalOutput = malloc(sizeof(char));
    totalOutput[0] = '\0';
    for (int i = 0; i < numProcesses; i++) {
        char*** thisProcLists = malloc(numProcesses * sizeof(void*));
        
        for (int j = 0; j < numProcesses; j++) {
            thisProcLists[j] = lists[j][i];
        }
        
        char* procOutput = processing(thisProcLists, lengths, i, numProcesses);

        totalOutput = realloc(totalOutput, sizeof(char) * (strlen(totalOutput) + strlen(procOutput) + 1));
        strcat(totalOutput, procOutput);

        free(thisProcLists);
        free(procOutput);
    }

    FILE* stream = fopen("src.c", "w");
    fputs(totalOutput, stream);
    fclose(stream);
    
    for (int i = 0; i < numProcesses; i++) {
        for (int j = 0; j < numProcesses; j++) {
            for (int k = 0; k < lengths[i][j]; k++) {
                free(lists[i][j][k]);
            }
            free(lists[i][j]);
        }
        free(lists[i]);
    }

    free(lists);

    for (int i = 0; i < numProcesses; i++) {
        free(lengths[i]);
    }

    free(lengths);

    free(files);
    free(totalOutput);

    return 0;
}
