#!/bin/bash

# if the correct amount of arguments is given, then
# the enter the for loop
# 
# otherwise, print usage statement
if [[ ( $# -gt 4 ) && ( $# -lt 8 ) ]];
then
    # for each arg after the exec cmd
    # run the executable with each successive pair of args
    for ((i=2 ; i<$# ; i++)); do
        INC=$(($i + 1))
        ./$1 ${!i} ${!INC}
    done
else
    echo -e "usage:\t./script01EC4.sh arg1 arg2 arg3 arg4 [arg5] [arg6]"
fi

