#!/bin/bash

#___________________________

#terminal notification
clear
echo "_________"
echo "make.sh"
echo "_________"

#build does not exist yet
mkdir -p ../build
cd ../build
#create makefile if it does not exist
if [ ! -f Makefile ]; then
	cmake ..
fi
#make
make
cd -
