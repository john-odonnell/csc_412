#!/bin/bash

# build Version 1
gcc -Wall -o ../Version1/v1 ../Version1/prog04.c

# build Version 2
gcc -Wall -o ../Version2/v2 ../Version2/prog04.c

# build Version 3
gcc -Wall -o ../Version3/v3 ../Version3/prog04.c
gcc -Wall -o ../Version3/dist ../Version3/dist.c
gcc -Wall -o ../Version3/proc ../Version3/proc.c
