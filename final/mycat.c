#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
 	
	int i;
	char c;
 	FILE* fd;	
	char *buf;

	if(argc < 2){
		printf("please enter at least 1 argument to read");
	}
	
 	
    char *input[3];
    input[0] = "cat";
    input[1] = argv[1];
    input[2] = 0;
    execvp(input[0], input);
   /** 
    fd = fopen(argv[1],"r+");
	
	if(fd<0){
		printf("file cannot be opened");
	}

        c = fgetc(fd);
	while(c != EOF){
		printf("%c",c);
		c = fgetc(fd);
	}

*/
	return 0;
}
