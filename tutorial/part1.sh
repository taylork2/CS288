#!/bin/bash

#with two parameters will automatically put space in between
echo Hello      World  
#this will put spaces 
echo "Hello     World"
#* will be the name of the program 
echo Hello * World 
#searches for packages with "hello"
echo `hello` world 

#======================
MY_MESSAGE="Hello World!"
echo $MY_MESSAGE

x="hello"
expr $x + 1 #will not work b/c expr expects number
y="1"
expr $y + 1 #this will work though

echo What is your name? 
read MY_NAME #reads in from standard input 
echo "Hello $MY_NAME"

#no error output if var not set 
#this script will create new shell
#want to create variable then run script use:
MYVAR=hello
export MYVAR 
#doesn't really matter since we are ./ (dot sourcing)

