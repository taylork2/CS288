#!/bin/bash
#insert sorts list of command line parameters in ascending order

re='^-?[0-9]+$'
array=( "$@" )
for ((idx=0 ; idx<${#array[@]} ; idx++ ))
do
	if ! [[ ${array[idx]} =~ $re ]]; then
		echo "Please enter integers only. ${array[idx]} is not an integer."
		array=( )
		break
	else
		x=${array[idx]}
		j=$((idx-1))
		while (( j>=0 && ${array[j]}>$x ))
		do
			array[$((j+1))]=${array[j]}
			j=$((j-1))
		done 
		array[((j+1))]=$x
	fi
done
echo ${array[@]}
