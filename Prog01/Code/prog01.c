#include <stdio.h>

int main(int argc, char* argv[]) {
    // prints a description of the command and arguments given
    // only if there are 2 arguments
    // format: ./exec arg1 arg2
    //
    // otherwise, print usage statement and return error code
    if (argc == 3) {
        printf("The executable %s was launched with two arguments:\n", argv[0]);
        printf("    The first argument is: %s,\n", argv[1]);
        printf("    The second argument is: %s.\n", argv[2]);

        return 0;
    } else {
	// usage reminder
        printf("usage:  %s <argument1> <argument2>\n", argv[0]);
        
	if (argc < 3) {
            // return 1 if too few args are given
	    return 1;
	} else {
	    // return 2 if too many args are given
            return 2;
	}
    }
}
