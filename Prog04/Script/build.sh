#!/bin/bash

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

# build Version 1
gcc -Wall -o "${PROG04_DIR}/Source/Version 1/v1" "${PROG04_DIR}/Source/Version 1/prog04.c"

# build Version 2
gcc -Wall -o "${PROG04_DIR}/Source/Version 2/v2" "${PROG04_DIR}/Source/Version 2/prog04.c"

# build Version 3
gcc -Wall -o "${PROG04_DIR}/Source/Version 3/v3" "${PROG04_DIR}/Source/Version 3/prog04.c"
gcc -Wall -o "${PROG04_DIR}/Source/Version 3/dist" "${PROG04_DIR}/Source/Version 3/dist.c"
gcc -Wall -o "${PROG04_DIR}/Source/Version 3/proc" "${PROG04_DIR}/Source/Version 3/proc.c"
