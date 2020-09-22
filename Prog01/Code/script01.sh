#!/bin/bash

# set up executable command and built-in parameters array
EXEC="./${1}"
PARAMS=("one" "two" "three" "four")

# build the executable
gcc -Wall -o ${1} prog01.c

# loop through parameters array, add param to exec cmd
# and run the command for each addition
for i in {0..3}; do
    EXEC+=" "
    EXEC+="${PARAMS[$i]}"
    $EXEC
done
