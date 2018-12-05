#! /bin/bash

# Which run to use
runNum=$1
if [ -z "$runNum" ]; then
    echo " give Run Number to use"
    exit
fi

# which target
tar=$2
if [ -z "$tar" ]; then
    echo " give target d or h"
    exit
fi

if [ "$tar" = "d" ]; then
    targ=2
fi
if [ "$tar" = "h" ]; then
    targ=1
fi

# which particle in SHMS
part=$3
if [ -z "$part" ]; then
    echo " give SHMS particle type p or m "
    exit
fi

if [ "$part" = "p" ]; then
    pityp=1
fi
if [ "$part" = "m" ]; then
    pityp=2
fi

# Which kinematic group
kgrp=$4
if [ -z "$kgrp" ]; then
    echo " give kinematic group 1 - 4"
    exit
fi

# Which kinematic group
ptgrp=$5
if [ -z "$ptgrp" ]; then
    echo " give p_t setting 1 - 12"
    exit
fi

echo " Comparing spectrometer variables for  pi${part} in SHMS"

# Which scripts to run.
script="UTIL_SIDIS/scripts/SIDIS_spec_compare.C"

# Which commands to run.
runbc="root -l \"${script}(${runNum}, ${targ}, ${pityp}, ${kgrp}, ${ptgrp})\""
eval ${runbc}

echo "If the plots look good quit root otherwise call an expert"
