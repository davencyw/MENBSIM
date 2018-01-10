#!/bin/bash

#variables and parameters
OUTFOLDER="../data/out/"
INFOLDER="../data/in/data.ascii"

SOFTENING=0.1
NUMSTEPS=3
TIMESTEP=1


#___________________________

#run
clear
../build/bin/menbsim \
						--input $INFOLDER \
						-d $TIMESTEP \
						--numsteps $NUMSTEPS \
						--softening $SOFTENING
