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
// 
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
// 
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
// 
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
// 
// RETURN VOID
void fibOut(int* arr, int f1, int f2, int n) {
    printf("%d terms of the Fibonacci series with F1=%d and F2=%d:\n    ", n, f1, f2);
    for (int i = 0; i < n; i++) {
        printf("F%d=%d", i+1, arr[i]);

        if (i == n-1) { printf("\n");  }
        else          { printf(", "); }
    }

    return;
}

// PRINT ALL FIBONACCI SEQUENCES IN A GIVEN SET
// the function is passed an array of pointers to fibo seqs
// 
// RETURN VOID
void allFibOut(int** fibSeqs, int* fibLens, int seqs) {
    for (int i = 0; i < seqs; i++) {
        fibOut(fibSeqs[i], fibSeqs[i][0], fibSeqs[i][1], fibLens[i]);
    }
    return;
}

// FIND UNIQUE VALUES IN A FIBONACCI SEQUENCE
// adds values not currently in the unique set to the set
// 
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
// 
// RETURN VOID
void uniqueOut(int* unique, int len) {
    printf("{");
    for (int i = 0; i < len; i++) {
        printf("%d", unique[i]);
        if (i != len-1) {
            printf(", ");
        }
    }
    printf("}\n");
}

// FREE FIBONACCI SEQS
// frees the individual allocations of each fibonacci array
// arguments include the array of sequence pointers and the length of the array
//
// RETURN VOID
void freeFibs(int** fibSeqs, int seqs) {
    for (int i = 0; i < seqs; i++) {
        free(fibSeqs[i]);
    }
    return;
}

// MAIN FUNCTION
// 
// allocates memory for
//      : an array of unique values
//      : an array of pointers to distinct fibonacci sequences
//      : an array of lengths for each fibonacci sequence
// for each set of 3 fibonacci descriptors
//      : call input validation
//      : allocate memory for this fibonacci sequence
//      : call fibonacci generator
//      : add pointer to memory to array of fib seqs
//      : find all presently unique values in the sequence and add them to a growing set
// once all fibonacci sets have been validated and generated
//      : output every fibonacci sequence
//      : call output function for unique values array
//      : free all allocated memory
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
    int** fibSeqs = malloc(sizeof(void*)*seqs);
    int*  fibLens = malloc(sizeof(int)*seqs);

    for (int i = 0; i < seqs; i++) {
        // convert arguments to integers with built in input validation
        int f1 = convert(argv[(i*3)+1]);
        int f2 = convert(argv[(i*3)+2]);
        int  n = convert(argv[(i*3)+3]);
        fibLens[i] = n;
        
        // usage reminders and error codes for failed validations
        if (f1 == -1 || f2 == -1 || n == -1 || f1 == 0 || f2 == 0 || n == 0 || f2 <= f1) {
            printf("usage:\t%s [F1 F2 n]+, with F2>F1>0 and n>0\n", argv[0]);
            
            // free allocated memory
            freeFibs(fibSeqs, i);
            free(fibSeqs);
            free(fibLens);
            free(unique);
            return 1;
        }

        // allocate memory on heap for fibonacci series and fill
        int* fibSeries = malloc(sizeof(int) * n);
        fibGen(fibSeries, f1, f2, n);
        fibSeqs[i] = fibSeries;

        // add unique values to the unique value array
        uniqueValues += findUnique(fibSeries, n, unique, uniqueValues);
    }
    
    allFibOut(fibSeqs, fibLens, seqs);

    // reallocate memory for total number of unique values and output
    unique = realloc(unique, sizeof(int)*uniqueValues);
    printf("The values encountered in the sequences are\n    ");
    uniqueOut(unique, uniqueValues);
    
    // free allocated memory space
    freeFibs(fibSeqs, seqs);
    free(unique);
    free(fibSeqs);
    free(fibLens);

    return 0;
}
