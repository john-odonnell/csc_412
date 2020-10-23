// URI CSC 412 OPERATING SYSTEMS : VERSION 2 OF PROGRAM 4
// one process does all the work
// args : number of child processes n, path to data directory path

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

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

// child process responsibilities
// : build lists of files
// : output each list of files to a file
// : files are names <thisPID>.<destPID>, so 0.0, 0.1, ..., <n-1>.<n-1>
void childProcess(char** files, int start, int interval, int numProcesses, int proc){
    // allocate memory for lists of files for each process
    char*** sublists = malloc(numProcesses * sizeof(void*));
    int* filesPerProcess = calloc(1, numProcesses * sizeof(int));

    FILE *stream;
    int procId = 0;
    for (int i = start; i < start + interval; i++) {
        // scan file for process id
        // increment counter at index :id in filePerProcess counter
        stream = fopen(files[i], "r");
        if (stream == NULL) {
            printf("File %s could not be opened!\n", files[i]);
            exit(1);
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

    for (int i = 0; i < numProcesses; i++) {
        // establish filename <thisPID>.<destinationPID>
        char* filename = calloc(1, sizeof(char) * 10);
        char* destination = calloc(1, sizeof(char) * 10);
        sprintf(filename, "%d", proc);
        sprintf(destination, "%d", i);
        strcat(filename, ".");
        strcat(filename, destination);
        filename = realloc(filename, sizeof(char) * strlen(filename));
        // open file and write filenames to it
        stream = fopen(filename, "w");
        if (stream == NULL) {
            printf("File %s could not be opened!\n", filename);
            exit(1);
        }
        for (int j = 0; j < filesPerProcess[i]; j++) {
            fputs(sublists[i][j], stream);
            fputs("\n", stream);
        }
        fclose(stream);
        free(filename);
        free(destination);
    }

    free(ptrs);

    exit(0);
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

// loop through all data files of the form <n>.<processId>
// collect data into an array and sort
// ouput to file <processId>.processed
// exit
char* processing(int processId, int numProcesses) {
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
        char* filename = calloc(1, sizeof(char) * 10);
        char* destination = calloc(1, sizeof(char) * 10);
        sprintf(filename, "%d", i);
        sprintf(destination, "%d", processId);
        strcat(filename, ".");
        filename = realloc(filename, sizeof(char) * (strlen(filename) + strlen(destination) + 2));
        strcat(filename, destination);
        
        // open filestream
        stream = fopen(filename, "r");
        if (stream == NULL) {
            printf("File %s could not be opened!", filename);
            exit(1);
        }
        
        // read each line from the file, strip the newline and insert into the array
        char* placeholder = calloc(1, sizeof(char) * 1024);
        while(fgets(placeholder, 1024, stream)) {
            toOrder[idx] = calloc(1, sizeof(char) * 1024);
            strcpy(toOrder[idx], placeholder);
            toOrder[idx][strlen(toOrder[idx])-1] = '\0';
            toOrder[idx] = realloc(toOrder[idx], sizeof(char) * (strlen(toOrder[idx]) + 1));

            idx++;
        }

        fclose(stream);

        if (remove(filename) != 0) {
            printf("Unable to delete file %s\n", filename);
        }

        free(filename);
    }

    // order array
    qsort(toOrder, idx, sizeof(char*), cmpfunc);

    // output strings to <processId>.processed
    // create output filename
    char* outputFilename = calloc(1, sizeof(char) * 10);
    sprintf(outputFilename, "%d", processId);
    outputFilename = realloc(outputFilename, sizeof(char) * (strlen(outputFilename) + 10 + 1));
    strcat(outputFilename, ".processed");
    // loop through array of filenames
    int procId, line;
    FILE* to_file = fopen(outputFilename, "w");
    if (to_file == NULL) {
        printf("File %s could not be opened!", outputFilename);
    }
    for (int i = 0; i < idx; i++) {
        char* code = calloc(1, sizeof(char) * 4096);
        // open each file and read it's line of code
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
    
    free(toOrder);
    
    exit(0);
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


    //
    // call fork to spawn n child processes
    // call the child process function within each fork
    //
    //
    // pointer to an array of numProcesses pointers
    // these pointers will point to groups of arrays returned by child processes
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

        // call fork
        int p = fork();
        printf("child %d forked\n", i);

        // child process is passed
        // : total list of files
        // : index to start processing
        // : length of processing interval
        // : total number of child processes
        // : it's own PID
        // result
        // : fills n files, named <selfPID>.<targetPID>
        // : each holds the filenaes that Child <targetPID> is responsible for
        if (p == 0) {
            childProcess(files, start, j, numProcesses, i);
        } else if (p < 0) {
            printf("Error creating Child Process #%d. Exiting.\n", i);
            exit(1);
        }
    }

    // wait for child processes to complete
    pid_t termProcess;
    int statusVal;
    for (int i = 0; i < numProcesses; i++) {
        termProcess = waitpid(-1, &statusVal, 0);
        printf("Parent process got signal from child process %6d with status value %d --> %d\n", termProcess, statusVal, WEXITSTATUS(statusVal));
    }

    // create another batch of child processes to process data
    for (int i = 0; i < numProcesses; i++) {
        int p = fork();
        printf("subchild %d forked\n", i);

        if (p == 0) {
            processing(i, numProcesses);
        } else if (p < 0) {
            printf("Error while creating SubChild Process #%d. Exiting.\n", i);
            exit(1);
        }
    }

    // wait for subchild processes
    for (int i = 0; i < numProcesses; i++) {
        termProcess = waitpid(-1, &statusVal, 0);
        printf("Parent process got signal from subchild process %6d with status value %d --> %d\n", termProcess, statusVal, WEXITSTATUS(statusVal));
    }

    // process data
    FILE* in;
    FILE* out;
    out = fopen("src.c", "w");
    for (int i = 0; i < numProcesses; i++) {
        // create input filenames
        char* inputFilename = calloc(1, sizeof(char) * 10);
        sprintf(inputFilename, "%d", i);
        inputFilename = realloc(inputFilename, sizeof(char) * (strlen(inputFilename) + 10 + 1));
        strcat(inputFilename, ".processed");

        char* placeholder = calloc(1, sizeof(char) * 1024);
        
        in = fopen(inputFilename, "r");
        while (fgets(placeholder, 1024, in)) {
            fputs(placeholder, out);
        }

        free(placeholder);

        fclose(in);
        
        if (remove(inputFilename) != 0) {
            printf("Unable to remove file %s\n", inputFilename);
        }
        free(inputFilename);
    }
    fclose(out);

    // free original file list
    free(files);

    return 0;
}
