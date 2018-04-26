#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <error.h>

char *PATH; 
int contains(char *arg, char target, int start);
void cisshPipe(char **command1, char **command2);
void cd(char *src);

char* pwd() {
	
	char *ptr;
 	char currentDir[100];

 	if (getcwd(currentDir, sizeof(currentDir)) != NULL) {
		//printf("Current working dir: %s\n", currentDir);
		ptr = (char*)malloc(sizeof(currentDir));
		ptr = currentDir;
		return ptr;
	} 
   
}

int main(int argc, char** argv) {
    
    char *input;
    while(1){
        PATH = malloc(strlen(pwd()));
        PATH = pwd();
        input = (char *)malloc(1);
        input[0] = '\0';

        printf("%s$ ", PATH);
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

        if(strcmp(input, "exit") == 0 || strcmp(input, "Exit") == 0)break;

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

        int pp[2];
        int start = 0, current = 0;
        i=0;
        int pipePos = -1;

        start = readWords;
        //printf("flags - %d,%d,%d", readWords, start, current);
        for(i=readWords;i<wordCount;i++){
            if(strcmp(separate[i], "|") == 0){
                pipePos = i;
                break;
            }
        }

        if(pipePos != -1){
            //printf("pipe at: %d\n", pipePos);

            char **torun;
            size_t size;
            for(i=0;i<pipePos;i++){
                size += sizeof(separate[i]);
            }
            torun = (char **)malloc(size + 1);
            for(i=0;i<pipePos;i++){
                torun[i] = malloc(strlen(separate[i]));
                torun[i] = separate[i];
            }
            torun[pipePos] = malloc(1);
            torun[pipePos] = 0;
            for(i=0;i<=pipePos;i++)printf("torun: %d = %s\n", i, torun[i]);
            
            char **torunt;
            size = 0;
            for(i=pipePos + 1;i<wordCount;i++){
                size += sizeof(separate[i]);
            }
            torunt = (char **)malloc(size + 1);
            for(i=pipePos + 1;i<wordCount;i++){
                torunt[i] = malloc(strlen(separate[i]) + 1);
                //printf("%d \n",(int)strlen(separate[i]));
                //torunt[i][0] = '\0';
                torunt[i] = separate[i];
            }
            torunt[wordCount] = malloc(1);
            torunt[wordCount] = 0;
            for(i=pipePos+1;i<=wordCount;i++)printf("torun2: %d = %s\n", i, torunt[i]);
       		printf("sending to func\n");
            cisshPipe(torun, torunt);	 
            /*int pp[2];
            if(pipe(pp) < 0)continue;

            pid_t pid, pid2;
            pid = fork();
            if(pid == 0){
                //child process 
                close(pp[0]);
                dup2(pp[1], STDOUT_FILENO);
                close(pp[1]);
                int result = execvp(torun[0], torun); 
            }
            else{
                pid2=fork();
                if(pid2 == 0){ 
                    close(pp[0]);
                    dup2(pp[0], STDIN_FILENO);
                    close(pp[1]);
                    //dup2(stdout, 1);
                    printf("try me\n");
                    int result = execvp(torunt[0], torunt);
                }
                else{
                    wait(NULL);
                    wait(NULL);
                }
            }

            close(pp[0]);
            close(pp[1]);
*/
        }
        else{
            if(strcmp(separate[0], "cd") == 0){
                cd(separate[1]);
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

int contains(char *arg, char target, int start){
    int ret = -1, i=0;
    for(i=start;i<strlen(arg);i++) 
    if(arg[i] == target){
        ret = i;
        break;
    }
    return ret;
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
