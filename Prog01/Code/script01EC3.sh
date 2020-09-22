#!/bin/bash

# if the correct amount of arguments is given,
# build and execute the command using the args
# 
# otherwise, echo usage statement
if [ $# -eq 3  ];
then
    gcc -Wall -o ${1} prog01.c
    ./$1 $2 $3
else
    echo -e "usage:  ./script01EC3.sh ./myProg arg1 arg2"
fi
