#!/bin/sh

for file in *
do
	mv -- "$file" "$(echo $file | tr [:upper:] [:lower:])" 2>/dev/null
done;
