#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

main(){

char *path, *argv[20], buf[80], n, *p;
int m, status, inword, continu;
int i, j;
int left_fd[2];
int right_fd[2];
int start[25] = { 0 };
int pipes, count;
while(1) {
	pipes = 0;
	inword = 0;
	p = buf;
	m = 0;
	continu=0;
	printf( "\nshhh> ");
	count = 0;
	while ( ( n = getchar() ) != '\n'  || continu ) {
		if ( n ==  ' ' ) {
	    		if ( inword ) {
				inword = 0;
				*p++ = 0;
		    	}
		}
		else if ( n == '\n' ) continu = 0;
		else if ( n == '\\'&& !inword ) continu = 1;
		else {
	    		if ( !inword ) {
				inword = 1;
				argv[m++] = p;
				count++;
				*p++ = n;
	     		}else
				*p++ = n;
		}
	}
	*p++ = 0;
	argv[m] = 0;
	if ( strcmp(argv[0],"exit") == 0 )
        {
            exit (0);
        }

	char *outFile[1];
	char *inFile[1];
	pipes=0;
	int in;
	int out;
	int inFlag = 0;
	int outFlag = 0;
	j = 1;
	for(i = 0; i < count; i++){
		if(argv[i] == NULL){
			break;
		}else if(strcmp(argv[i], "|") ==0){
			argv[i] = 0;
			pipes++;
			start[j] = i + 1;
			j++;
		}else if(strcmp(argv[i], ">") == 0){
			outFlag = 1;
			argv[i] = 0;
			outFile[0] = argv[i+1];
		}else if(strcmp(argv[i], "<") == 0){
			inFlag = 1;
			argv[i] = 0;
			inFile[0] = argv[i+1];
		}
	}
	for(j=0; j <= pipes; j++)
	{
		if( pipes > 0 && j != pipes)
		{
			pipe(right_fd);
		}
		int  PID;
		PID = fork();

		if(PID < 0) //Error
		{
			printf("error.");
			return -1;
		}	

		else if(PID > 0)  //parent
		{ 
			if(j > 0)
			{
				close(left_fd[0]);
				close(left_fd[1]);
			}
			left_fd[0]=right_fd[0];
			left_fd[1]=right_fd[1];
			wait(&status);
		}

		else if(PID==0) //inside child
		{
			if(j == 0 && inFlag == 1)
			{
				in = open(inFile[0], O_RDONLY);
				close(0);
                                dup(in);
                                close(in);
			}

			if(pipes > 0)
			{
                     		if(j==0) //first child
				{
					close(1);
					dup(right_fd[1]);
					close(right_fd[1]);
					close(right_fd[0]);
				}
				
				else if(j==pipes) //last child
				{
					close(0);
					dup(left_fd[0]);
					close(left_fd[0]);
					close(left_fd[1]);
				}
				
				else  //middle child
				{
					close(1);
                       	        	dup(right_fd[1]);
					close(0);
					dup(left_fd[0]);
                       	        	close(right_fd[1]);
                                	close(right_fd[0]);
                                	close(left_fd[0]);
                                	close(left_fd[1]);
				}
			} //end of if pipes > 0 statement

			if(outFlag == 1 && j == pipes)
			{
                                 out = creat(outFile[0], 00777);
                                 close(1);
                                 dup(out);
                                 close(out);
			}
			execvp(argv[start[j]], &argv[start[j]]);
		} //end of if pid == 0 statement
	}
	wait(&status);
}
}
