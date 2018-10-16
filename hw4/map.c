#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {	

	
	printf("%p\n", argv);
	printf("%p\n", &argv);
	int i;
	for (i=0;i<argc;i++){
		printf("argv[%d]=%p\n", i, argv[i]);
		printf("ref: argv[%d]=%p\n", i, &argv[i]);
	}

	printf("%p\n", &argc);
	makeDivider(1);
	printf(" FFFFFFFF");
	makeDivider(3);

	printf("\nargv\t|");
	char buffer[8];
	buffer = argv;
	for (i=0;i<8;i+=2){
		printf("  %c%c  |", buffer[i], buffer[i+1]);
	}
	printf("\nargc\t|  00  |  00  |  00  |  0%d  | %p", argc, &argc);
	makeDivider(3);
	printf(" 00000000\n");
	return 0;
}

makeDivider(numDividers){
	int i;
	for ( i=0; i<numDividers; i++){
		printf("\n\t+------+------+------+------+");
	}
}

