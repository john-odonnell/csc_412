#!/bin/bash

# set up executable command and built-in parameters array
EXECUTABLE=$1
PARAMETERS=("one" "two" "three" "four")
EXEC="./$EXECUTABLE"

# loop through parameters array, add param to exec cmd
# and run the command for each addition
for i in {0..3}; do
    EXEC+=" "
    EXEC+="${PARAMETERS[$i]}"
    $EXEC
done
