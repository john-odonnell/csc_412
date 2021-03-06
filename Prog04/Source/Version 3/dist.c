#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Distribution Process
 *
 * The distribution process opens a file, which contians filenames assigned
 * to the process by the Server. The process reads each filepath, opens the file,
 * and determines which processing process the file belongs to. The distribution
 * process writes each filename to a group of output files, <thisProc>.<processingProc>,
 * where <thisProc> signifies which distribution process handled the process, and
 * <processingProc> is the ID of the process which will process the file.
 *
 * @param files Path to file, consisting of a list of files
 * @param numProcesses Total number of distribution processes
 * @param proc This distribution process' ID
 * @param execPath Path to this module's executable
 *
 * @return 0 on success, 1 on failure
 */
int main(int argc, char* argv[]) {
    // pull arguments

    // the server will build files of
    // only the filenames a certain process will have
    // to care about
    char* files = argv[0];
        
    // int start = atoi(argv[2]);
    // int interval = atoi(argv[3]);
    int numProcesses = atoi(argv[1]);
    int proc = atoi(argv[2]);

    char* execPath = argv[3];
    
    // allocate memory for lists of files for each process
    char*** sublists = malloc(numProcesses * sizeof(void*));
    int* filesPerProcess = calloc(1, numProcesses * sizeof(int));

    // open file holding files to distribute
    FILE* filesToDist = fopen(files, "r");
    FILE* file;
    int procId = 0;

    char* filename = calloc(1, sizeof(char) * 1024);
    while(fgets(filename, 1024, filesToDist)) {
        filename[strlen(filename)-1] = '\0';
        // scan file for process id
        // increment file counter in counter array
        file = fopen(filename, "r");
        if (file == NULL) {
            printf("File %s could not be opened!\n", filename);
            exit(1);
        }
        fscanf(file, "%d ", &procId);

        if (procId < 0 || procId >= numProcesses) {
            printf("Invalid Process ID %d\n", procId);
        }

        filesPerProcess[procId] = filesPerProcess[procId] + 1;
        
        // close this file
        fclose(file);
    }
    // close file of files
    fclose(filesToDist);

    // allocate lists based on file counter array
    for (int i = 0; i < numProcesses; i++) {
        sublists[i] = malloc(sizeof(void*) * filesPerProcess[i]);
    }

    char*** ptrs = malloc(sizeof(char*) * numProcesses);
    for (int i = 0; i < numProcesses; i++) {
        ptrs[i] = &sublists[i][0];
    }

    // reopen files to distribute file
    filesToDist = fopen(files, "r");
    if (filesToDist == NULL) {
        printf("File %s could not be opened!\n", files);
    }
    while (fgets(filename, 1024, filesToDist)) {
        filename[strlen(filename)-1]='\0';

        file = fopen(filename, "r");
        if (file == NULL) {
            printf("File %s could not be opened!\n", filename);
        }
        fscanf(file, "%d ", &procId);
        fclose(file);
        *ptrs[procId] = calloc(1, sizeof(char) * (strlen(filename) + 1));
        strcpy(*ptrs[procId], filename);
        ptrs[procId] = ptrs[procId] + 1;
    }

    fclose(filesToDist);
    free(filename);

    if (remove(files) != 0) {
        printf("Unable to delete file %s\n", files);
    }

    for (int i = 0; i < numProcesses; i++) {
        // establish filename <thisPID>.<destinationPID>
        char* thisOut = calloc(1, sizeof(char) * 1024);
        char* thisProc = calloc(1, sizeof(char) * 10);
        char* destination = calloc(1, sizeof(char) * 10);
        
        sprintf(thisProc, "%d", proc);
        sprintf(destination, "%d", i);
        strcpy(thisOut, execPath);
        strcat(thisOut, thisProc);
        thisOut = realloc(thisOut, sizeof(char) * (strlen(thisOut) + strlen(thisProc) + strlen(destination) + 2));
        strcat(thisOut, ".");
        strcat(thisOut, destination);
        // open file and write filenames to it
        file = fopen(thisOut, "w");
        if (file == NULL) {
            printf("File %s could not be opened!\n", thisOut);
            exit(1);
        }
        for (int j = 0; j < filesPerProcess[i]; j++) {
            fputs(sublists[i][j], file);
            fputs("\n", file);
        }
        fclose(file);
        free(thisOut);
        free(thisProc);
        free(destination);
    }

    // free allocated memory
    free(ptrs);
    free(filesPerProcess);
    for (int i = 0; i < numProcesses; i++) {
        free(sublists[i]);
    }
    free(sublists);

    exit(0);
}

