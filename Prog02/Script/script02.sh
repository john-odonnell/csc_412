#!/bin/bash

# bash script for finding all the files of a given type within a given directory
# usage:  ./script02.sh <directory> <extension>
# <extension>   correct format: txt
#             incorrect format: .txt

# check number of arguments
# if the number of arguments is not 2, print usage statement
# otherwise, search directory for files of given type
if [[ $# -ne 2 ]]; then
    echo "usage:  ${0} <path> <extension>"
else
    # if the input directory has no trailing /, include it
    # the trailing / is required in file discovery loop
    DIRPATH=${1}
    if [[ $DIRPATH[-1] != "/" ]]; then
        DIRPATH="${DIRPATH}/"
    fi

    # print preliminary statement
    echo "Looking for files with extension .${2} in folder"
    echo "    ${1}:"

    # file discovery loop
    # find all files in the given directory with the given extension
    FILES=()
    for file in $DIRPATH*; do
        if [[ ${file##*.} == ${2} ]]; then
            FILES+=("${file:${#DIRPATH}}")
        fi
    done

    # print results
    # print number of files
    NUM_FILES=${#FILES[@]}
    if [[ NUM_FILES -eq 1 ]]; then
        echo "1 file found:"
    else
        echo "${NUM_FILES} files found:"
    fi
    # print each file in the files list
    for file in "${FILES[@]}"; do
        echo "    ${file}"
    done
fi
