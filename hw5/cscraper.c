#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 100

struct clip *build_a_lst(char * fn);
struct clip *append(struct clip *hp,char **five);
int find_length(struct clip *hp);
void print_a_lst(struct clip *cp);
void split_line(char **fields,char *line);

struct clip {
  int views;
  char *user;
  char *id;
  char *title;
  char *dur;
  struct clip *next;
} *head;

int main(int argc, char **argv) {
  int n;
  head = build_a_lst(*(argv+1));
  n = find_length(head);
  printf("%d clips\n",n);
  print_a_lst(head);    /* prints the table */
  return 0;
}

struct clip *build_a_lst(char *fn) {
  FILE *fp;
  struct clip *hp;
  char *fields[4];
  char line[LINE_LENGTH];
  int cnt=0;
  hp=NULL;

  // open fn
  // while no more lines
  // read a line
  // split the line into four substrings/int and store them in a struct
  // append - add the struct at the end of the list
  // return the head pointer holding the list
  fp = fopen(fn, "r");
  if (fp==NULL){
    printf("File does not exist.\n");
    exit(0); 
  }
  while (fgets(line, LINE_LENGTH, fp) != NULL) {
    cnt++;
    split_line(fields, line);

    hp=append(hp, fields);
  }

  fclose(fp);

  return hp;
}

/* fields will have four values stored upon return */
void split_line(char **fields,char *line) {
  int i=0;
  const char *token, *delim;
  delim = ",\n";
  /* 
     call strtok(line, delim);
     repeat until strtok returns NULL using strtok(NULL, delim);
 */
  fields[i]=strtok(line, delim);
  printf("%s\n",fields[0]);
  while(fields[++i]=strtok(NULL, delim)){}
}

/* set four values into a clip, insert a clip at the of the list */
struct clip *append(struct clip *hp,char **five) {
  struct clip *cp,*tp;

  /* 
     malloc tp
     set views using atoi(*five)
     malloc for four strings.
     strcpy four strings to tp
     insert tp at the end of the list pointed by hp
     use cp to traverse the list
 */
  tp=(struct clip*) malloc(sizeof(struct clip));
  
  tp->views = atoi(strdup(five[0]));
  tp->user = (char *) malloc(strlen(five[1])+1);
  strcpy(tp->user, five[1]);
  tp->id = (char *) malloc(strlen(five[2])+1);
  strcpy(tp->id, five[2]);
  tp->title = (char *) malloc(strlen(five[3])+1);
  strcpy(tp->title, five[3]);
  tp->dur = (char *) malloc(strlen(five[1])+1);
  strcpy(tp->dur, five[4]);
  hp->next=tp;

  return hp;
}

void print_a_lst(struct clip *cp) {
  /* 
     use a while loop and the statement below to print the list
     printf("%d,%s,%s,%s,%s\n",cp->views,cp->user,cp->id,cp->title,cp->time);
  */
  while (cp != NULL){
      printf("%d,%s,%s,%s,%s\n",cp->views,cp->user,cp->id,cp->title,cp->dur);
      cp = cp->next;
  }
}

int find_length(struct clip *hp){
  int count=0;
  while (hp != NULL){
    count++;
    hp = hp->next;
  }
  return count;
}

/* end */