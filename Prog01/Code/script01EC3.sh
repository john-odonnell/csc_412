#!/bin/bash

# if the correct amount of arguments is given,
# execute the command using the args
# 
# otherwise, echo usage statement
if [ $# -eq 3  ];
then
	./$1 $2 $3
else
	echo -e "usage:\t./script01EC3.sh executable arg1 arg2"
fi
