#!/bin/bash

function myfunc2() {
	echo dummy 
}

function myfunc(){
	local x=$1 y=$2
	echo $x $y
	myfunc2
}

function main(){
	local p=$1 q=$2 r=$3 s=$4
	echo $p $q $r $s 
	myfunc $q $r
}

main $1 $2 $3 $4 