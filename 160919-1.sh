#given d 
#traverse the document 
#using recursion

d=/home/Documents/CS288
lst='ls $d'

function traverse(){
	local unknown=$1 
	if[-d $unknown]; then
		lst=("ls $unknown"))
		for x in ${lst[@]}; do
			traverse $unknown/$x
			done
	else 
		echo $unknown 
	fi
}

#create directories 
b=3
d=3
for ((i=0))