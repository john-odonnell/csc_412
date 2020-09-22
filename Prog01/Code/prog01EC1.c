#include <stdio.h>

int main(int argc, char* argv[]) {
    // new char pointer points to first memlocation after the
    // ./ in the exec statement by incrementing two bytes
    char* execString = argv[0] + 2;

    // print argument data when run with 3 parameters (exec cmd, arg1, arg2)
    //
    // otherwise, print usage statement and return error code
    if (argc == 3) {
        printf("The executable %s was launched with two arguments:\n", execString);
        printf("    The first argument is: %s,\n", argv[1]);
        printf("    The second argument is: %s.\n", argv[2]);

        return 0;
    } else {
        // print usage reminder
	printf("usage:  %s <argument1> <argument2>\n", execString);
        
	// return error code for too few or too many args
	if (argc < 3) {
            return 1;
	} else {
            return 2;
	}
    }
}
