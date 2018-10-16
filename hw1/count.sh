#!/bin/bash
#counter for bash commands 
alpha=( {a..z} )
for letter in ${alpha[@]}
do
	echo -n "$letter "
	ls /bin/$letter* 2>/dev/null | wc -w 
done