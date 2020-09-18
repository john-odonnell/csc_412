#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    // pull the call to the executable from the arguments array
    char* execString = argv[0];
    
    // find the index of the last slash in the file path
    int idxLastSlash;
    for (int i = 0; i < strlen(execString); i++) {
	if (execString[i] == '/') {
		idxLastSlash = i;
	}
    }

    // create a char pointer to the array idx after the final slash
    char* execFile = (void*)execString + idxLastSlash + 1;

    // print argument data if there are 3 given (exec cmd, arg1, arg2)
    //
    // otherwise, print usage statement and return error code
    if (argc == 3) {
        printf("The executable %s was launched with two arguments:\n", execFile);
        printf("\tThe first argument is: %s\n", argv[1]);
        printf("The second argument is: %s\n", argv[2]);

        return 0;
    } else {
        printf("usage:\t%s <argument1> <argument2>\n", execFile);
        return 1;
    }
}
