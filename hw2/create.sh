#!/bin/bash 

d=$1
b=$2
dir=$3
first=$4 

create(){
	if [[ $first == "breadth" ]]
	then 
		breadth
	elif [[ $first == "depth" ]]
	then 
		depth 1 1
	else 
		echo "4th arg must be 'depth' or 'breadth.'"
	fi
}

breadth(){
	mkdir "$dir-breadth"
	open=("$dir-breadth")
	closed=()
	for ((i=1 ; i<=$d ; i++))
	do 
		temp_open=()
		for x in ${open[@]}
		do 
			for (( k=1 ; k<=$b ; k++ ))
			do 
				mkdir $x/$k
				echo "$x/$k"
				temp_open+=($x/$k)
			done
			closed+=($x)
		done
		open=(${temp_open[@]})
	done
}

depth(){
	declare -i dp=$1
	declare -i bd=$2
	declare -i i=1

	if (( $dp <= ($d+1) ))
		then
		if (( $dp == 1 && $bd == 1 ))
			then
			mkdir "$dir-depth"
			cd "$dir-depth"
		else
			mkdir $bd
			cd $bd
			pwd
		fi

		let dp++

		while [ $i -le $b ]
		do
			depth $dp $i
			let i++
		done
		cd ..
	fi
}

if [ $# != 4 ]; then
	echo "Please provide 4 arguments. 1.depth 2.breadth 3.directory 4.'breath' or 'depth' choice"
else
	create
fi