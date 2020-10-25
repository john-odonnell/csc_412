// URI CSC 412 OPERATING SYSTEMS : VERSION 3 OF PROGRAM 4
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

/**
 * Recurse a Directory
 *
 * Recurses through a directory. For each item, the path to the file is added
 * to an array if the file is of the correct type. For each, a pointer to a
 * file count tracker is incremented.
 *
 * @param path Path to a directory
 * @param totalFiles Pointer to a value which tracks directory members
 *
 * @return An array of strings, allocated on the heap
 */
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

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("usage: ./a.out <n> <path> <out>\n");
        printf(" <n>    : number of child processes\n");
        printf(" <path> : path to directory of files\n");
        printf(" <out>  : path to output file\n");
        return 1;
    }

    // program arguments
    // numProcesses : the number of child processes that the parent will spawn
    // path : the path in the filesystem to a directory of files to be processed
    char* execPath = argv[0];
    execPath[strlen(execPath)-2] = '\0';
    printf("EXEC PATH: %s\n", execPath);
    int numProcesses;
    sscanf(argv[1], "%d", &numProcesses);
    char* path = argv[2];
    char* outFile = argv[3];

    // fill an array with all the files in the directory
    int numFiles = 0;
    char** files = recurseDirectory(path, &numFiles);

    int interval = numFiles / numProcesses;
    int remainder = numFiles % numProcesses;

    char* procString = calloc(1, sizeof(char) * 1024);

    //
    // call fork to spawn n child processes
    // call the child process function within each fork
    //
    //
    // pointer to an array of numProcesses pointers
    // these pointers will point to groups of arrays returned by child processes
    FILE* dist_stream;
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

        char* filesDiv = calloc(1, sizeof(char) * 1024);
        char* processStr = calloc(1, sizeof(char) * 10);
        strcpy(filesDiv, execPath);
        sprintf(processStr, "%d", i);
        strcat(filesDiv, processStr);
        strcat(filesDiv, ".dist");
        filesDiv = realloc(filesDiv, sizeof(char) * (strlen(filesDiv) + 1));
        printf("File Div: %s\n", filesDiv);

        dist_stream = fopen(filesDiv, "w");
        for (int k = start; k < start + j; k++) {
            fputs(files[k], dist_stream);
            fputs("\n", dist_stream);
        }
        fclose(dist_stream);

        sprintf(procString, "%d", i);

        // call fork
        int p = fork();

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
            // childProcess(filesDiv, numProcesses, i);
            // arguments need to be strings
            char* toExec = calloc(1, sizeof(char) * 1024);
            strcpy(toExec, execPath);
            strcat(toExec, "dist");
            execl(toExec, filesDiv, argv[1], procString, execPath, NULL);
            free(toExec);
        } else if (p < 0) {
            printf("Error creating Child Process #%d. Exiting.\n", i);
            exit(1);
        }

        free(filesDiv);
    }

    // wait for child processes to complete
    // pid_t termProcess;
    int statusVal;
    for (int i = 0; i < numProcesses; i++) {
        waitpid(-1, &statusVal, 0);
        // termProcess = waitpid(-1, &statusVal, 0);
        // printf("Parent process got signal from child process %6d with status value %d --> %d\n", termProcess, statusVal, WEXITSTATUS(statusVal));
    }

    // create another batch of child processes to process data
    for (int i = 0; i < numProcesses; i++) {
        sprintf(procString, "%d", i);

        int p = fork();

        if (p == 0) {
            // processing(i, numProcesses); 
            char* toExec = calloc(1, sizeof(char) * 1024);
            strcpy(toExec, execPath);
            strcat(toExec, "proc");
            execl(toExec, procString, argv[1], execPath, NULL);
            free(toExec);
        } else if (p < 0) {
            printf("Error while creating SubChild Process #%d. Exiting.\n", i);
            exit(1);
        }
    }

    // wait for subchild processes
    for (int i = 0; i < numProcesses; i++) {
        waitpid(-1, &statusVal, 0);
        // termProcess = waitpid(-1, &statusVal, 0);
        // printf("Parent process got signal from subchild process %6d with status value %d --> %d\n", termProcess, statusVal, WEXITSTATUS(statusVal));
    }

    // process data
    FILE* in;
    FILE* out;
    out = fopen(outFile, "w");
    for (int i = 0; i < numProcesses; i++) {
        // create input filenames
        char* inputFilename = calloc(1, sizeof(char) * 1024);
        char* processNum = calloc(1, sizeof(char) * 10);
        sprintf(processNum, "%d", i);
        strcat(inputFilename, execPath);
        strcat(inputFilename, processNum);
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
    for (int i = 0; i < numFiles; i++) {
        free(files[i]);
    }
    free(files);

    return 0;
}
