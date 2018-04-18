#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void mycat() {

}


void mycp() {

}


void myls() {

}


void mycd() {

}

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

        printf("%s\n", input);

        if(strcmp(input, "exit") == 0)break;

        char **separate;
        char *word;

        int i;
        for(i=0;i<strlen(input);i++){
            word = (char *)malloc(1);
            word[0] = '\0';

            if(input[i] == ' '){
                separate = (char **)malloc(sizeof(separate) + sizeof(word) + 1);
            }
            else if(input[i] == '\0');
            else{
                char *tmp;
                tmp = (char *)malloc(sizeof(input[i]) + 1);
                tmp[0] = input[i];
                tmp[1] = '\0';
                strcat(word, tmp);
            }
        }

        free(input);
    }
}
