#!/bin/sh

#########################
DEFAULT="../build/bin/menbsim"
PROFILING="../build/bin/menbsim_profiling"

if [ "$1" = "profiling" ]; then
  EXECUTABLE=$PROFILING
else
  EXECUTABLE=$DEFAULT
fi
#########################

#variables and parameters
OUTFOLDER="../data/out"
INFOLDER="../data/in/data.ascii"

SOFTENING=0.1
NUMSTEPS=24

SOLVERTYPE=1

#___________________________

#run
clear
eval $EXECUTABLE \
						--input $INFOLDER \
						--outfolder $OUTFOLDER \
						--numsteps $NUMSTEPS \
						--softening $SOFTENING \
            --solvertype $SOLVERTYPE \
						--nooutput
