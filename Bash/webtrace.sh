#!/bin/sh

echo "" > change.txt
web=$1
wait=$2
while [ 1 ];
	do
	lynx -dump $web > old.txt
	sleep $wait
	lynx -dump $web > new.txt
	change=$(diff old.txt new.txt)
	if [ "$change" ];
		then
		echo $change >> change.txt
		echo "Nastapila zmiana"
	fi
done



