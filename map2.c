#include <stdio.h>
#include <stdlib.h>

struct node *head=NULL;
// struct node *current = NULL;

struct node{
	int val;
	struct node * next;
};

void insert(val){
	struct node *link = (struct node*) malloc(sizeof(struct node));
	link->val = val;
	link->next = head;
	head = link;
}

int main(int argc, char *argv[]) {	
	makeDivider(1);
	printf(" FFFFFFFF");
	makeDivider(3);

	int k;
	//store the strings into a linked list 
	int length=0;
	for (k=0;k<argc;k++) {
		int offset=0;
		while ( *(argv[k] + offset) != '\0'){
			if (offset == 1000){
				//just in case this runs forever
				break;
			}
			insert(*(argv[k]+offset));
			offset++;
			length++;
		}
		insert(*(argv[k]+offset));	
		length++;
	}

	//Fill the extra space with --
	int newLine=0;
	printf("\n\t|");
	if (length%4!=0){
		for (k=(length%4);k<4; k++){
			newLine+=1;
			printf("%s", "  --  |");
		}	
	}

	int offset=length - length%4 -1;
	struct node *ptr = head;
	while(ptr != NULL){
		if (newLine % 4 == 0){
			newLine=0;
			printf(" %X\n\t|", argv+offset);
			offset-=4;
		}
		if (ptr->val == '\0'){
			printf("  00  |");
		} else {
			printf("  %X  |", ptr->val);
		}
		ptr=ptr->next;
		newLine++;
	}
	printf("address");

	makeDivider(3);
	offset=argc-1;
	for (k=0;k<argc;k++){	
		printf("\nargv+%d\t|  %X  |", offset, *(argv+offset));
		offset--;
	}

	// 			newLine=0;
	// 			printf("|\n\t");
	// 		}
	// 		printf("|  %X  ", *(argv[k] + offset));
	// 		offset++;
	// 		newLine++;
	// 	}
	// 	printf("|  %X   ", *(argv[k] + offset));

	// printf("%d\n", argc);
	// printf("%p\n", &argc);
	// printf("%p\n", argv);
	// printf("%p\n", &argv);
	// int i;
	// for (i=0;i<argc;i++){
	// 	printf("argv[%d]=%p\n", i, argv[i]);
	// 	printf("ref: argv[%d]=%p\n", i, &argv[i]);
	// }

	makeDivider(3);
	printf(" 00000000\n");

	printf("%X", *argv);
	printf("%X", argv);
	return 0;
}

makeDivider(numDividers){
	int i;
	for ( i=0; i<numDividers; i++){
		printf("\n\t+------+------+------+------+");
	}
}
