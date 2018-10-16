#!/bin/bash/env bash

url="http://www.wsj.com/mdc/public/page/2_3021-activnyse-actives.html"
count=0
while (( $count<60 ))
do
	date=`date +%Y_%m_%d_%H_%M_%S`
	wget $url -O "nyse_$date.html"
	python extract.py "nyse_$date.html"
	((count++))
	sleep 60
done
