#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <error.h>
#include <fcntl.h>

char *PATH; 

void cisshPipe(char **command1, char **command2);
void cd(char *src);

char* pwd(int type) {
	
	char *ptr;
 	char currentDir[100];

 	if (getcwd(currentDir, sizeof(currentDir)) != NULL) {
        if(type == 0)printf("%s%s$%s ", "\x1B[34m", currentDir, "\x1B[37m");
        else printf("Current working dir: %s\n", currentDir);
		ptr = (char*)malloc(sizeof(currentDir));
		ptr = currentDir;
		return ptr;
	} 
   
}

int main(int argc, char** argv) {
    //char *path, *argv[20], buf[80], n, *p;
    int m, status, inword, continu;
    int i, j;
    int left_fd[2];
    int right_fd[2];
    int starts[25] = { 0 };
    int pipes, count;
    char *input;
    while(1){
        /*
        PATH = (char *)malloc(strlen(pwd()) + 1);
        PATH = pwd();
        */

        input = (char *)malloc(1);
        input[0] = '\0';

        pwd(0);
        
        while(1){
            char c;
            scanf("%c", &c);
            if(c == '\n'){
                break;
            }
            else{
                char *word = (char *)malloc(sizeof(c) + 1);
                word[0] = c;
                word[1] = '\0';
                strcat(input, word);
            }
        }

        if(strcmp(input, "exit") == 0 || strcmp(input, "Exit") == 0){
            exit(1);
        }

        char **separate;
        char *word;

        word = (char *)malloc(1);
        word[0] = '\0';

        int i, wordCount = 0;
        
        for(i=0;i<strlen(input);i++){ 
            if(input[i] == ' '){
                wordCount++;
            }
            else if(input[i] == '|' || input[i] == '>'){
                if(input[i-1] != ' ' && input[i+1] != ' '){
                    wordCount+=2;
                }
                else if(input[i-1] != ' ' || input[i+1] != ' '){
                    wordCount++;
                }
            }
            else{
                if(i == (strlen(input) - 1)){
                    wordCount++;
                }
            } 
        }

        separate = (char **)malloc(wordCount * sizeof(char));
        int p;
        for(p=0;p<wordCount;p++){
            separate[p] = (char *)malloc(1);
            separate[p][0] = '\0';
        }
       

        int index = 0;
        for(i=0;i<strlen(input);i++){ 

            if(input[i] == ' '){
                if(strcmp(word, "\0") != 0){
                    char **tmp = (char **)malloc(sizeof(separate));
                    
                    int p;
                    for(p=0;p<index;p++){
                        tmp[p] = (char *)malloc(sizeof(separate[p]));
                        tmp[p] = separate[p];
                    }

                    separate = (char **)malloc((index + 1) * sizeof(word));

                    for(p=0;p<index;p++){
                        separate[p] = tmp[p];
                    }

                    separate[index] = (char *)malloc(sizeof(word) + 1);
                    separate[index] = word;
                    separate[index][strlen(word)] = '\0';

                    index++;
                    word = (char *)malloc(1);
                    word[0] = '\0';
                }
            }
            else if(i == (strlen(input) - 1)){
                char *tmp;
                tmp = (char *)malloc(sizeof(input[i]) + 1);
                tmp[0] = input[i];
                tmp[1] = '\0';
                strcat(word, tmp);

                char **tmp2 = separate;

                separate = (char **)malloc((wordCount + 1) * sizeof(word));

                int p;
                for(p=0;p<wordCount;p++)separate[p] = tmp2[p];

                separate[index] = (char *)malloc(sizeof(word) + 1);
                separate[index] = word;
                separate[index][strlen(word)] = '\0';

                index++;
                word = (char *)malloc(1);
                word[0] = '\0';
                
            }
            else if(input[i] == '|' || input[i] == '>'){
                if(i > 0 && input[i - 1] != ' '){
                    char **tmp = (char **)malloc(sizeof(separate));
                    
                    int p;
                    for(p=0;p<index;p++){
                        tmp[p] = (char *)malloc(sizeof(separate[p]));
                        tmp[p] = separate[p];
                    }

                    separate = (char **)malloc((index + 1) * sizeof(word));

                    for(p=0;p<index;p++){
                        separate[p] = tmp[p];
                    }

                    separate[index] = (char *)malloc(sizeof(word) + 1);
                    separate[index] = word;
                    separate[index][strlen(word)] = '\0';

                    index++;
                    word = (char *)malloc(1);
                    word[0] = '\0';
                }
                //printf("special chars\n");
                char *tword = (char *)malloc(2);
                tword[0] = input[i];
                tword[1] = '\0';
                char **tmp = (char **)malloc(sizeof(separate));
                
                int p;
                for(p=0;p<index;p++){
                    tmp[p] = (char *)malloc(sizeof(separate[p]));
                    tmp[p] = separate[p];
                }

                separate = (char **)malloc((index + 1) * sizeof(tword));

                for(p=0;p<index;p++){
                    separate[p] = tmp[p];
                }

                separate[index] = (char *)malloc(sizeof(tword));
                separate[index] = tword;

                index++;
                tword = (char *)malloc(1);
                tword[0] = '\0'; 
                word = (char *)malloc(1);
                word[0] = '\0';
            }
            else{
                char *tmp;
                tmp = (char *)malloc(sizeof(input[i]) + 1);
                tmp[0] = input[i];
                tmp[1] = '\0';
                strcat(word, tmp);
            }
        }
        //Now I have a char* of user input and a char **of the input separated by a space
        //Deal with pipe, redirects, and non-commands 
        //for(i=0;i<wordCount;i++)printf("at %d = %s\n", i, separate[i]);
            
		int readWords = 0;
        char *outFile[1];
        char *inFile[1];
        int in;
        int out;
        int inFlag = 0;
        int outFlag = 0;
        int j=1;
		int pipes = 1;
        int status;

        for(i=0;i < wordCount; i++){
			if(strcmp(separate[i], "|") ==0){
				separate[i] = 0;
				pipes++;
				starts[j] = i + 1;
				j++;
			}
			else if(strcmp(separate[i], ">") == 0){
				outFlag = 1;
				separate[i] = 0;
				outFile[0] = separate[i+1];
			}
			else if(strcmp(separate[i], "<") == 0){
				inFlag = 1;
				separate[i] = 0;
				inFile[0] = separate[i+1];
        	}
        }

        if(pipes > 1){
            for(j=0; j < pipes; j++)
            {
                if( pipes > 0 && j != pipes)
                {
                    pipe(right_fd);
                }
                int PID;
                PID = fork();

                if(PID < 0) //Error
                {
                    printf("Error: Could not fork\n");
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
                    //printf("%s\n", separate[starts[j]]);
                    if(strcmp(separate[starts[j]], "myls") == 0)execvp("./myls", &separate[starts[j]]);
                    if(strcmp(separate[starts[j]], "mycat") == 0)execvp("./mycat", &separate[starts[j]]);
                    if(strcmp(separate[starts[j]], "mycp") == 0)execvp("./mycp", &separate[starts[j]]);
                    else execvp(separate[starts[j]], &separate[starts[j]]);
                } //end of if pid == 0 statement
            }
            wait(&status);
        }
        else{ 
            if(strcmp(separate[0], "cd") == 0 || strcmp(separate[0], "mycd") == 0){
                cd(separate[1]);
            }
            else if(strcmp(separate[0], "pwd") == 0 || strcmp(separate[0], "mypwd") == 0){
                pwd(-1);
            }
            else if(strcmp(separate[0], "myls") == 0){
                pid_t pid;
				pid = fork();
				if(pid == 0){
					//child process
                    if(inFlag == 1)
                    {
                         in = open(inFile[0], O_RDONLY);
                         close(0);
                         dup(in);
                         close(in);
                    }
                    if(outFlag == 1)
                    {
                        out = creat(outFile[0], 00777);
                        close(1);
                        dup(out);
                        close(out);
                    }
					int result = execvp("./myls", separate);
					if(result == -1){
						printf("Error: %s\n", strerror(errno));
					}
				}
                wait(NULL);
            }
            else if(strcmp(separate[0], "mycat") == 0){
                 pid_t pid;
				pid = fork();
				if(pid == 0){
					//child process
					if(inFlag == 1)
                    {
                         in = open(inFile[0], O_RDONLY);
                         close(0);
                         dup(in);
                         close(in);
                    }
                    if(outFlag == 1)
                    {
                        out = creat(outFile[0], 00777);
                        close(1);
                        dup(out);
                        close(out);
                    } 
                    int result = execvp("./mycat", separate);
					if(result == -1){
						printf("Error: %s\n", strerror(errno));
					}
				}
                wait(NULL);
            }
            else if(strcmp(separate[0], "mycp") == 0){
                pid_t pid;
				pid = fork();
				if(pid == 0){
					//child process
					if(inFlag == 1)
                    {
                         in = open(inFile[0], O_RDONLY);
                         close(0);
                         dup(in);
                         close(in);
                    }
                    if(outFlag == 1)
                    {
                        out = creat(outFile[0], 00777);
                        close(1);
                        dup(out);
                        close(out);
                    }
                    int result = execvp("./mycp", separate);
					if(result == -1){
						printf("Error: %s\n", strerror(errno));
					}	
				}
                wait(NULL);
            }
            else{
				pid_t pid;
				pid = fork();
				if(pid == 0){
					//child process
					int result = execvp(separate[0], separate);
					if(result == -1){
						printf("Error: %s\n", strerror(errno));
					}
					else{
						
					}
				}
				else if(pid < 0){
					//error could not make child
					printf("Error: Unable to create child process\n");
				}
				else{
					//parent process
                    wait(NULL);
				}	
            }
        }
    }
}

void cisshPipe(char **command1, char **command2)
{
    int fd[2];
    pid_t childPid;
    if (pipe(fd) != 0)
        //error("failed to create pipe");

    if ((childPid = fork()) == -1)
        //error("failed to fork");

    if (childPid == 0)
    {
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        execvp(command1[0], command1);
        //error("failed to exec command 1");
    }
    else
    {
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);
        execvp(command2[0], command2);
        //error("failed to exec command 2");
    }
}

void cd(char *src){
      int retval = chdir(src);
      if(retval < 0){ fprintf(stderr, "directory not found.\n");}
}
