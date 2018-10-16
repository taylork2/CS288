#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void split_line(char **fields, char* line);

int main(){
	char *line;
	char *fields;
	line="this,is,something,cool";
	 
   	split_line(&fields, line);
  	return(0);
}

void split_line(char **fields, char *line){
	int i=0;
	char *token, *delim;
	delim=",";

	token = strtok(line, delim);
	printf("%s\n", line);

	while(token!=NULL){
		fields[i]=(char*)malloc(strlen(token));
		strcpy(fields[i], token);
		printf("%s\n", fields[i]);
		token=strtok(NULL, delim);

		i++;
	}
}