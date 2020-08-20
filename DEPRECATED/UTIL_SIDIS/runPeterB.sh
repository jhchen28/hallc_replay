#! /bin/bash

# Which run to use
runNum=$1
if [ -z "$runNum" ]; then
    echo " give Run Number to use"
    exit
fi

    targ=1

# Which scripts to run.
script="UTIL_SIDIS/PeterB.C"

## Which commands to run.
runbc="root -l \"${script}(${runNum}, ${targ})\""
eval ${runbc}


