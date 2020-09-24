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
void fibOut(int* arr, int fi, int f2, int n) {
    for (int i = 0; i < n; i++) {
        printf("F%d=%d", i+1, arr[i]);

        if (i == n-1) { printf("\n");  }
        else          { printf(",\t"); }
    }

    return;
}

// MAIN FUNCTION
// takes input
// calls validation
// calls fibonacci generator
// calls output function
// RETURN VALUES
// 0 if all operations terminate successfully
// 1 if an input argument is not a valid integer
// 2 if the given F2 is not greater than the given F1
int main(int argc, char* argv[]) {
    // convert and validate input
    int f1 = convert(argv[1]);
    int f2 = convert(argv[2]);
    int  n = convert(argv[3]);
    
    // usage reminders and error codes for failed validations
    if (f1 == -1 || f2 == -1 || n == -1) {
        printf("usage:\t./fibo F1 F2 n, with F2>F1>0 and n>0.\n");
        return 1;
    }
    if (f2 <= f1) {
        printf("error:\tThe second argument must be strictly larger than the first\n");
        return 2;
    }

    printf("%d terms of the Fibonacci series with F1=%d and F2=%d:\n", n, f1, f2);
    
    // allocate memory on heap for fibonacci series and fill
    int* fibSeries = malloc(sizeof(int) * n);
    fibGen(fibSeries, f1, f2, n);
    
    // call output function
    fibOut(fibSeries, f1, f2, n);

    return 0;
}
