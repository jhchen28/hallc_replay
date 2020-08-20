#! /bin/bash

# Which run to use
runNum=$1
if [ -z "$runNum" ]; then
    echo " give Run Number to use"
    exit
fi

# which target
pol=$2
if [ -z "$pol" ]; then
    echo " give SHMS polarity type POS or NEG "
    exit
fi

if [ "$pol" = "POS" ]; then
    pol=1
fi
if [ "$pol" = "NEG" ]; then
    pol=2
fi
if [ "$pol" = "pos" ]; then
    pol=1
fi
if [ "$pol" = "neg" ]; then
    pol=2
fi

echo " counting for ${pol} "

# Which scripts to run.
script="UTIL_SIDIS/Bean_counter_all.C"

## Which commands to run.
runbc="root -l \"${script}(${runNum}, ${pol})\""
eval ${runbc}

echo "If the plots look good quit root otherwise call an expert"
