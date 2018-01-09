#!/bin/bash
injectfolder=../include/*

#get declaration and  vars
startsym="/*__DECLARATION__"
endsym=" */"
declarationtext=$(awk '{printf "%s\\n", $0}' ../declaration.txt)
maxlines=95

editedfiles=0

#loop through all files
for file in $(find $injectfolder -name '*.hh'); do
	#if there is a declaration, remove it
	numline=1
	declarationexists=false
	firstline=$(awk -v n=$numline 'NR == n' $file)
	nextline=firstline
	if [[ "$firstline" == "$startsym" ]]; then
		declarationexists=true
		#count lines until endsym is reached
		while [[ "$nextline" != "$endsym" ]] &&  [[ "$numline" -lt "$maxlines" ]]; do
		numline=$((numline + 1))
		nextline=$(sed -n "$numline p" $file)
		done

	fi

	#warn and exit on fail
	if [[ $numline == 95 ]]; then
		echo "FAILED ON FILE "
		echo "$file"
		echo "\n exiting"
		exit 1
	fi

	#remove first n lines
	if [[ "$declarationexists" == true ]]; then
		sed -i -e "1,$numline d" $file
	fi

	#inject new declaration
	sed -i "1s/^/$declarationtext/" $file	
	editedfiles=$((editedfiles + 1))

done

echo "edited $editedfiles files" 
