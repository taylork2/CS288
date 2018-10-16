#!/bin/bash
seed=$1
depth(){
	x=$1
	# if (( $x != "" )); then
		lst=(`ls -d $x*/`)
		for y in ${lst[@]}; do
			echo "$y"
			depth "$y" 
		done
	# fi
}

depth $seed