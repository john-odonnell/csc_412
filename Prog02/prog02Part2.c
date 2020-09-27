// Fibonacci Sequence
// USAGE    ./executable F1 F2 n
// generates the first n Fib numbers given F1 and F2

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

// VALIDATE INPUT
// checks if each character in a given string is a valid digit
// RETURN VALUES
// : TRUE  if all characters are a valid digit
// : FALSE if any character fails the digit test
bool validate(char arg[]) {
    for (int i = 0; arg[i] != 0; i++) {
        if (!isdigit(arg[i])) {
            return false;
        }
    }
    return true;
}

// CONVERT INPUT FROM CHAR[] TO INT
// converts on successful validation call
// RETURN VALUES
// : N >= 0 if the input is successful validated and converted
// : -1     otherwise
int convert(char arg[]) {
    if (validate(arg)) {
        return atoi(arg);
    } else {
        return -1;
    }
}

// GENERATE FIBONACCI SEQUECE
// Fills an array of length n given the initial conditions f1 and f2
// RETURN VOID
void fibGen(int* arr, int f1, int f2, int n) {
    arr[0] = f1;
    arr[1] = f2;

    for (int i = 2; i < n; i++) {
        arr[i] = arr[i-1] + arr[i-2];
    }

    return;
}

// OUTPUT FIBONACCI SEQUENCE
// adheres to assignment section 2.1 output specifications
// RETURN VOID
void fibOut(int* arr, int f1, int f2, int n) {
    printf("%d terms of the Fibonacci series with F1=%d and F2=%d:\n    ", n, f1, f2);
    for (int i = 0; i < n; i++) {
        printf("F%d=%d", i+1, arr[i]);

        if (i == n-1) { printf("\n");  }
        else          { printf(",\t"); }
    }

    return;
}

// FIND UNIQUE VALUES IN A FIBONACCI SEQUENCE
// adds values not currently in the unique set to the set
// RETURN VALUES
// : N, the amount of unique values added
int findUnique(int* fib, int fibLen, int* unique, int uniqueLen) {
    int added = 0;
    for (int i = 0; i < fibLen; i++) {
        bool found = false;
        int j = 0;
        while (!found && j < uniqueLen) {
            if (fib[i] == unique[j]) {
                found = true;
            } else {
                j++;
            }
        }
        if (!found) {
            unique[uniqueLen] = fib[i];
            uniqueLen++;
            added++;
        }
    }
    return added;
}

// UNIQUE VALUE ARRAY OUTPUT
// outputs all the unqiue values found in the many generated fibonacci sequences
// RETURN VOID
void uniqueOut(int* unique, int len) {
    printf("The values encountered in the sequences are\n");
    printf("    {");
    for (int i = 0; i < len; i++) {
        printf("%d", unique[i]);
        if (i != len-1) {
            printf(", ");
        }
    }
    printf("}\n");
}

// MAIN FUNCTION
// takes input - sets of 3 ints describing different Fib seqs
// calls validation on each set
// calls fibonacci generator and output function for each
// 
// RETURN VALUES
// 0 if all operations terminate successfully
// 1 if input is invalid, either in form or type
int main(int argc, char* argv[]) {
    // return error and print usage for:
    //  too few arguments
    //  argument counts indivisible by 3
    if (argc < 4 || (argc-1)%3 != 0) {
        printf("usage:\t%s [F1 F2 n]+, with F2>F1>0 and n>0\n", argv[0]);
        return 1;
    }
   
    // find maximum unque values and allocate maximum required memory
    int maxUnique = 0;
    for (int i = 3; i < argc; i+=3) {
        maxUnique += convert(argv[i]);
    }
    int* unique = malloc(sizeof(int)*maxUnique);
    int  uniqueValues = 0;
 
    // parse arguments and generate fibonacci sequences
    int  seqs = argc / 3;
    for (int i = 0; i < seqs; i++) {
        // convert arguments to integers with built in input validation
        int f1 = convert(argv[(i*3)+1]);
        int f2 = convert(argv[(i*3)+2]);
        int  n = convert(argv[(i*3)+3]);

        
        // usage reminders and error codes for failed validations
        if (f1 == -1 || f2 == -1 || n == -1 || f2 <= f1) {
            printf("usage:\t%s [F1 F2 n]+, with F2>F1>0 and n>0\n", argv[0]);
            return 1;
        }

        // allocate memory on heap for fibonacci series and fill
        int* fibSeries = malloc(sizeof(int) * n);
        fibGen(fibSeries, f1, f2, n);
    
        // call output function
        fibOut(fibSeries, f1, f2, n);

        // add unique values to the unique value array
        uniqueValues += findUnique(fibSeries, n, unique, uniqueValues);
        
        // free allocated memory for this fibonacci sequence
        free(fibSeries);
    }
    
    // reallocate memory for total number of unique values and output
    unique = realloc(unique, sizeof(int)*uniqueValues);
    uniqueOut(unique, uniqueValues);
    free(unique);

    return 0;
}
