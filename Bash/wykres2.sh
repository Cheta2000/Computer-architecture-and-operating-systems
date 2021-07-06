#!/bin/sh

while [ 1 ];
        do
        dana=$(awk '{print $1}' /proc/loadavg)
        licznik=$(echo "($dana*100)/1" | bc)
	out="";
        while [ $licznik -gt 0 ];
	        do
		licznik=$(($licznik-1))
        	out="${out}#";
        done
        tput setaf 1
        echo -n $out;
        tput setaf 2
	echo " $dana";
        sleep 1;
done

~                                                                                                                                                                                                           
~                 
