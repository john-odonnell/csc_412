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
    printf("||| RECURSE DIRECTORY |||\n");
    
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
    printf("total files: %d\n", *totalFiles);

    closedir(dir);

    char** files = (char**)calloc(1, ((*totalFiles * 2) + 1) * sizeof(char*));

    // add all files in the directory to a list of filenames
    int i = 0;
    dir = opendir(path);
    while((entry = readdir(dir)) != NULL) {
        char* name = entry->d_name;
        unsigned char type = entry->d_type;
        printf("file %d: %s\n", i, name);

        if ((name[0] != '.') && (type == DT_REG)) {
            files[i] = (char*)malloc(strlen(path) + strlen(name) + 1);
            printf("\tarr[%d] memloc: %p\n", i, files + i);
            printf("\t%p allocated\n", (void*)files[i]);
            strcat(files[i], path);
            strcat(files[i], name);
            printf("\t%s assigned\n", files[i]);
            i++;
        }
    }

    closedir(dir);

    // return the list of filenames
    return files;
}

void childProcess(char** files, int start, int interval, char**** lists, int** lengths, int numProcesses, int proc){
    printf("||| CHILD PROCESS %d |||\n", proc);
    
    // allocate memory for lists of files for each process
    char*** sublists = malloc(numProcesses * sizeof(void*));
    int* filesPerProcess = calloc(1, numProcesses * sizeof(int));
    printf("\tsublists ptr : %p\n", (void*)sublists);
    printf("\tfileCnt pntr : %p\n", (void*)filesPerProcess);

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

        printf("file: %s\n\tproc: %d\n", files[i], procId);
    }

    for (int i = 0; i < numProcesses; i++) {
        // allocate lists based on filePerProcess counter
        sublists[i] = malloc(sizeof(void*) * filesPerProcess[i]);
        printf("sublist %d created\n", i);
    }

    char*** ptrs = malloc(sizeof(char*) * numProcesses);
    for (int i = 0; i < numProcesses; i++) {
        ptrs[i] = &sublists[i][0];
        printf("PTR%d : %p\n", i, (void*)ptrs[i]);
    }
    for (int i = start; i < start + interval; i++) {
        // insert each file into the process files list
        stream = fopen(files[i], "r");
        fscanf(stream, "%d ", &procId);
        fclose(stream);
        // *ptrs[procId] = (char*)malloc(sizeof(char) * (strlen(files[i]) + 1));
        // strcpy(*ptrs[procId], files[i]);
        *ptrs[procId] = files[i];
        printf("PTR%d : %p\tVALUE : %s\n", procId, ptrs[procId], *ptrs[procId]);
        ptrs[procId] = ptrs[procId] + 1;
    }

    printf("\n\nCONTENTS OF SUBLIST\n");
    for (int i = 0; i < numProcesses; i++) {
        printf("PROCESS %d\n", i);
        for (int j = 0; j < filesPerProcess[i]; j++) {
            printf("\tFILE: %s\n", sublists[i][j]);
        }
    }
    printf("\n\n");

    // assign pointer to process file list into parent array
    lists[proc] = sublists;
    printf("SUBLIST POINTER IN LISTS: %p\n", lists[proc]);
    lengths[proc] = filesPerProcess;
    printf("SUBLIST POINTER IN LENGTH: %p\n", lengths[proc]);
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

// TODO PROCESSING FUNCTION
char* processing(char*** lists, int** lengths, int processId, int numProcesses) {
    printf("||| PROCESSING PROCESS %d |||\n", processId);
    // order the files by index
    // store their outputs sequentially in a string
    // return the long string
    
    // find total file count for this process
    int fileCount = 0;
    for (int i = 0; i < numProcesses; i++) {
        fileCount += lengths[i][processId];
    }
    printf("\tprocess filecount : %d\n", fileCount);

    // allocate array for ordered files
    char** toOrder = malloc(fileCount * sizeof(char*));
    printf("new array allocated to sort files by line number\n");

    // insert all files into single array
    int idx = 0;
    for (int i = 0; i < numProcesses; i++) {
        int len = lengths[i][processId];
        for (int j = 0; j < len; j++) {
            toOrder[idx] = lists[i][j];
            printf("%s inserted\n", toOrder[idx]);
            idx++;
        }
    }

    // order array
    qsort(toOrder, fileCount, sizeof(char*), cmpfunc);
    printf("sorted by line number\n");

    // concatenate strings
    char* output = malloc(sizeof(char));
    output[0] = '\0';
    FILE* stream;
    int procId, line;
    for (int i = 0; i < fileCount; i++) {
        char* code = malloc(sizeof(char) * 100);

        stream = fopen(toOrder[i], "r");
        fscanf(stream, "%d %d ", &procId, &line);
        fgets(code, 100, stream);
        printf("line:%d\t%s\n", line, code);
        printf("strlen code : %lu\n", strlen(code));

        output = realloc(output, sizeof(char) * (strlen(output) + strlen(code)));
        strcat(output, code);
        printf("new output : %s\n", output);
    }

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

    printf("||| MAIN FUNCTION |||\n");
    printf("numProc : %d\n", numProcesses);
    printf("path    : %s\n", path);

    // fill an array with all the files in the directory
    int numFiles = 0;
    char** files = recurseDirectory(path, &numFiles);
    printf("||| MAIN FUNCTION |||\n");
    printf("numFile : %d\n", numFiles);

    int interval = numFiles / numProcesses;
    int remainder = numFiles % numProcesses;
    printf("interval : %d\n", interval);
    printf("remainder: %d\n", remainder);

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
        printf("call Child Process %d\n", i);
        printf("\tstart idx : %d\n", start);
        printf("\tinterval  : %d\n", j);
        printf("\tfiles ptr : %p\n", (void*)files);
        printf("\tlists ptr : %p\n", (void*)lists);
        printf("\tlens ptr  : %p\n", (void*)lengths);
        childProcess(files, start, j, lists, lengths, numProcesses, i);
        printf("||| MAIN FUNCTION |||\n");
    }

    printf("\n\n\nCONTENTS OF LISTS\n");
    for (int i = 0; i < numProcesses; i++) {
        for (int j = 0; j < numProcesses; j++) {
            printf("PROCESS %d\n", j);
            for (int k = 0; k < lengths[i][j]; k++) {
                printf("\t\tFILE: %s\n", lists[i][j][k]);
            }
        }
    }
    printf("\n\n\n");


    char* totalOutput = malloc(sizeof(char));
    totalOutput[0] = '\0';
    for (int i = 0; i < numProcesses; i++) {
        char*** thisProcLists = malloc(numProcesses * sizeof(void*));
        
        printf("Lists for Processing process:\n");
        for (int j = 0; j < numProcesses; j++) {
            thisProcLists[j] = lists[j][i];
            printf("\t%p\n", (void*)thisProcLists[j]);
        }
        printf("New pointer allocated for use in Process%d\n", i);
        

        // TODO STORE OUTPUT
        char* procOutput = processing(thisProcLists, lengths, i, numProcesses);
        printf("PROCESS %d\n", i);
        printf("%s\n", procOutput);

        totalOutput = realloc(totalOutput, sizeof(char) * (strlen(totalOutput) + strlen(procOutput) + 1));
        strcat(totalOutput, procOutput);

        // free
        for (int j = 0; j < numProcesses; j++) {
            free(thisProcLists[j]);
        }
        free(thisProcLists);
    }

    // TODO CONCATENATE AND OUTPUT SRC FILE
    FILE* stream = fopen("src.c", "w");
    printf("\n\n||| OUTPUT |||\n");
    printf("%s\n", totalOutput);
    fputs(totalOutput, stream);
    
    // TODO FREES
    // free lists
    free(lengths);
    free(lists);
    free(files);

    return 0;
}