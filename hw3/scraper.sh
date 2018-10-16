#!/bin/bash
#Scraping the youtube homepage for views, users, durations, titles. and ids
#then output and store in csv file 

# wget -O index.html youtube.com

raw=`egrep "aria-describedby" index.html`
raw_views=(`echo $raw | egrep -o 'yt-lockup-meta-info"><li>[0-9,]+ views' | cut -d'>' -f3`)
views=(${raw_views[@]//"views"/})
views=(${views[@]//,/})
raw_durs=(`echo $raw | egrep -o 'Duration: [0-9:]+'`)
durs=(${raw_durs[@]//"Duration:"/})

IFS=$'\n'
titles=(`echo $raw | egrep -o '[^=]*" aria-describedby' | cut -d'"' -f2`)
comma=MY_COMMA
titles=("${titles[@]//,/$comma}\n")
users=(`echo $raw | egrep -o 'data-ytid="[^<]+' | cut -d'>' -f2`)
users=("${users[@]//,/$comma}\n")
ids=(`echo $raw | egrep -o 'data-ytid="[^"]+' | cut -d'"' -f2`)
# echo -e "User,Views,Time(duration),ID,Title" | paste -s >> file.csv
for ((i=0; i<${#views[@]}; i++)); do
	echo -e "${views[i]},${users[i]},${ids[i]},${titles[i]},${durs[i]}" | paste -s >> file.csv
done

paste file.csv

# echo "Total Users: ${#users[@]}"
# echo "Views: ${#views[@]}"
# echo "Durations: ${#durs[@]}"
# echo "Titles: ${#titles[@]}"
# echo "IDs: ${#ids[@]}"