#!/bin/sh

while [ 1 ];
	do
	dana1=$(awk '$1=="wlp4s0:" {print $10}' /proc/net/dev)
	sleep 1
	dana2=$(awk '$1=="wlp4s0:" {print $10}' /proc/net/dev)
	dana=$(($dana2-$dana1))
	format=B
	echo $dana
	licznik=$(($dana/1000))
	echo $licznik
	if [ $dana -gt 1000000 ];
		then dana=$(($dana/1000000))
		format=MB
	elif [ $dana -gt 1000 ];
		then dana=$(($dana/1000))
		format=KB
	fi
	out="";
	while [ $licznik -gt 0 ];
		do ((licznik--))
		out="${out}#";
	done
	tput setaf 1
	echo -n $out;
	tput setaf 2 
	echo " $dana $format/s"
done

