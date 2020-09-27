#!/bin/bash

# check number of arguments
if [[ $# -ne 2 ]]; then
    echo "usage:  ${0} <path> <extension>"
else
    DIRPATH=${1}
    if [[ $DIRPATH[-1] != "/" ]]; then
        DIRPATH="${DIRPATH}/"
    fi

    # print preliminary statement
    echo "Looking for files with extension .${2} in folder"
    echo "    ${1}:"

    # find all files in the given directory with the given extension
    FILES=()
    for file in $DIRPATH*; do
        if [[ ${file##*.} == ${2} ]]; then
            FILES+=(${file:${#DIRPATH}})
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
