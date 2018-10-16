#!/bin/bash
#print files/directories >avg file size 

main()
{
	dir=( "$(ls $1)" )
	# echo ${dir[@]}
	average $1
	filter $1

}

average()
{
	total="$(du -sm $1 | cut -f 1)"
	length=${#dir[@]}
	avg=$(($total / $length))
}

filter(){
	echo "$1/$x"
	for x in ${dir[@]}
	do
		size=$(du -ms "$1/$x" | cut -f 1)
		# name=$(du -ms "$1/$x" | cut -f 2)
		if [ $size -gt $avg ]; then
			# echo $x
		fi
	done
}

if [ $# != 1 ]
then 
	echo "Please provide directory to reverse as first argument."
else 
	main $1	
fi