#!/bin/bash

# args
# path to data file directory
# path to output file to produce

if [[ $# -ne 2 ]]; then
    echo "usage: ${0} <data_dir> <out_path>"
else
    # pull arguments
    DATA_DIR=${1}
    OUT_FILE=${2}

    NUM_PROCS=0

    # add trailing slash to data dir if necessary
    if [[ ${DATA_DIR:(-1)} != "/" ]]; then
        DATA_DIR="${DATA_DIR}/"
    fi

    # for each file in the data dir
    # if the file is a .txt file
    # read the process id and compare against NUM_PROCS
    for file in "$DATA_DIR"*; do
        if [[ ${file##*.} == "text" ]]; then
            proc=$(cat "$file" | awk '{print $1}')
            if [[ $proc -ge $NUM_PROCS ]]; then
                NUM_PROCS=$(($proc + 1))
            fi
        fi
    done

    # set dir variable
    DIR="$(pwd)"

    last_slash=0
    dir_len=${#DIR}
    for ((i = 0 ; i < ${dir_len} ; i++)); do
        if [[ ${DIR:$((i)):6} == "Prog04" ]]; then
            last_slash=$((i + 6))
        fi
    done

    PROG04_DIR=${DIR:0:${last_slash}}

    "${PROG04_DIR}/Source/Version 3/v3" ${NUM_PROCS} "${DATA_DIR}" ${OUT_FILE}
fi
