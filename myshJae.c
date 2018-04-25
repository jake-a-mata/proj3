#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

char *PATH;
int contains(char *arg, char target, int start);
char* pwd() {
	
	char *ptr;
 	char currentDir[100];

 	if (getcwd(currentDir, sizeof(currentDir)) != NULL) {
		printf("Current working dir: %s\n", currentDir);
		ptr = (char*)malloc(sizeof(currentDir));
		ptr = currentDir;
		return ptr;
	} 
   
}

int main(int argc, char** argv) {
    PATH = pwd();
    char *input;
    while(1){
        input = (char *)malloc(1);
        input[0] = '\0';

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
                if(input[i-1] != ' ' || input[i+1] != ' '){
                    wordCount++;
                }
            }
            else{
                if(i == (strlen(input) - 1)){
                    wordCount++;
                }
            } 
        }

        printf("Word count = %d\n", wordCount); 
        separate = (char **)malloc(wordCount * sizeof(char));
        int p;
        for(p=0;p<wordCount;p++){
            separate[p] = (char *)malloc(1);
            separate[p][0] = '\0';
        }
       

        int index = 0;
        for(i=0;i<strlen(input);i++){ 

            if(input[i] == ' '){
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
                 
            }
            else{
                char *tmp;
                tmp = (char *)malloc(sizeof(input[i]) + 1);
                tmp[0] = input[i];
                tmp[1] = '\0';
                strcat(word, tmp);
                //free(tmp);
            }
        }
        //Now I have a char* of user input and a char **of the input separated by a space
        //Deal with pipe, redirects, and non-commands 
        for(i=0;i<wordCount;i++)printf("at %d = %s\n", i, separate[i]);
        /*    
		int readWords = 0;

        int pp[2];

        while(readWords < wordCount){
            int ispipe = -1;
            int redirect1 = -1;
            int redirect2 = -1;
            int redirect3 = -1;
            int arrow = -1;
            int i;

            arrow = contains(separate[readWords], '>', 0);

            pid_t pid;
            pid = fork();
            if(pid == 0){
                //child process
                if(ispipe != -1){
                    pipe(pp);
                    dup2(pp[1], 1);
                    dup2(pp[0], 0);
                    close(pp[0]);
                    close(pp[1]);
                }
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
            }
        }
        */
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
