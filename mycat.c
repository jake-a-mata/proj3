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
	
 	//printf("%d",argc);	
//	printf("%s\n",argv[1]);

//	if (argv[1][0] == '<'){
//		printf("carrot detected");	
//	}
//	printf("%s\n",argv[2]);
//	printf("%s\n",argv[3]);
//	printf("%s\n",argv[4]);

	fd = fopen(argv[1],"r+");
	
	if(fd<0){
		printf("file cannot be opened");
	}

        c = fgetc(fd);
	while(c != EOF){
		printf("%c",c);
		c = fgetc(fd);
	}


	return 0;
}
