#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void removeChar(char *str, char c) {



}


int main(int argc, char **argv) {
int j;

	if(argc <= 1) {
		printf("There is not enough arguments.\n");
		exit(0);
	}

	
    /*char** new_argv = (char**)malloc(sizeof(argv));
    	for(int i = 1; i < argc; ++i)
    	{
        	new_argv[i-1] = (char*)malloc(strlen(argv[i]));
    		new_argv[i-1] = argv[i];
	}

	new_argv[argc] = NULL;

	for(int i = 0; i < argc -1; ++i)
    	{
			if(i == 0) {
                for(j = 2; j < (int)strlen(new_argv[i]); j++) {
                    new_argv[i][j-2] = new_argv[i][j];
                }
            new_argv[i][(int)strlen(new_argv[i]) - 2] = '\0';
            //new_argv[i][(int)strlen(new_argv[i])] = '\0';
            }
            //printf("%s\n", new_argv[i]);
        }

    	// free memory
//    	for(int i = 0; i < argc - 1; ++i)
//    	{
  //      	free(new_argv[i]);
    //8	}
    //	free(new_argv);
//printf("%s\n", new_argv[0]);
new_argv[3] = '\0';
*/
    char *input[argc+1];
    input[0] = "cp";
    input[1] = argv[1];
    input[2] = argv[2];
    if(argc == 4)input[3] = argv[3];
    input[argc] = 0;

	execvp(input[0], input);
	perror(argv[1]);
	exit(0);
}
