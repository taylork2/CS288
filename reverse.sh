#!/bin/bash
#reverse contents of directory passed as parameter
main()
{
	array=($(ls $1))
	reverse
}

reverse()
{
	for ((i=${#array[@]}-1 ; i>=0 ; i-- ))
	do
		echo -n "${array[i]} "
	done
	echo
}

if [ $# != 1 ]
then 
	echo "Please provide directory to reverse as first argument."
else 
	main $1	
fi