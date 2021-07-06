#!/bin/sh

for file in /proc/*
	do
	if [ -e $file/status ];
		then
		tput setaf 1
		echo  $(awk '$1=="Pid:" {print $0}' $file/status)
		tput sgr0
		echo $(awk '$1=="PPid:" {print $0}' $file/status)
		echo $(awk '$1=="State:" {print $0}' $file/status)
		echo $(awk '$1=="Name:" {print $0}' $file/status)
		echo -n "Files: "
		echo $(ls -l $file/fd | wc -l)
		fi
done


