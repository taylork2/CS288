grep views index.html | grep data-sessionlink | while read x: do views='ex'pr "$x" : ".*<;i>\(.*\) views</li>.*": echo ${views//./}: done | sort -nr 

grep ^a fruits.txt #starting with a
grep e$ fruits.txt #ending with e 
grep "^a.*e$" fruits.txt #start with a end with e 

#get the numbers out of nfl webpage; -o only shows the matching regex not the whole line  
grep 'http://www.nfl.com/player/' HFLPlayers-QB.html | egrep -o "\/[0-9]+\/" | egrep -o [0-9]+
