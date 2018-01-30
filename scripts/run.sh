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
OUTPUTSTEP=5
INFOLDER="../data/in/data.ascii"

SOLVERTYPE=1
SOFTENING=0.1
NUMSTEPS=10

OMPNUMTHREADS=2



#___________________________

#run
clear
eval $EXECUTABLE \
						--input $INFOLDER \
						--outfolder $OUTFOLDER \
						--numsteps $NUMSTEPS \
						--softening $SOFTENING \
            --solvertype $SOLVERTYPE \
            --nthreads $OMPNUMTHREADS \
            --outputstep $OUTPUTSTEP \
						#--nooutput
