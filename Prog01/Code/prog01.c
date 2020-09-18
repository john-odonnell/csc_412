#include <stdio.h>

int main(int argc, char* argv[]) {
    // prints a description of the command and arguments given
    // only if there are 3 arguments (exec cmd, arg1, arg2)
    //
    // otherwise, print usage statement and return error code
    if (argc == 3) {
        printf("The executable %s was launched with two arguments:\n", argv[0]);
        printf("\tThe first argument is: %s\n", argv[1]);
        printf("The second argument is: %s\n", argv[2]);

        return 0;
    } else {
        printf("usage:\t%s <argument1> <argument2>\n", argv[0]);
        return 1;
    }
}
