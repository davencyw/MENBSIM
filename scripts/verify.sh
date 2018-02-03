#!/bin/sh

#########################
EXECUTABLE="../build/bin/menbsim_verification"

#########################

#variables and parameters
OUTFOLDER="../data/out"
INFOLDER="../data/in/data.ascii"
#INFOLDER="../data/in/testforce.ascii"

SOLVERTYPE=1
SOFTENING=0.01
NUMSTEPS=10

OMPNUMTHREADS=1



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
						--nooutput
