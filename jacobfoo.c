#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
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

int main(int argc, char *argv[]){

  pid_t pid;
  int status;
  int sum = 0;
  int ret;
  errno = 0;
  // function is named mycd which will handle a 2d array argument.
  // look at hw for 2d array. then compare the strings with whatever. 
  // when i have an error i got to find the error number and return a 
  // string with the error message. if succesful, dont return anything
  // if -1 check errno return string in it
  char *cmd = "uhh";
  char *buf =(char *) malloc(sizeof(pwd()));
  buf = pwd();
  int length = 0;
  char src[100];
  for(int i = 1; i<argc; i++){
    strcat(src, argv[i]);
    strcat(src, " ");
  }
  printf("arg1 = %s\n", argv[0]);
  printf("arg2 = %s\n", argv[1]);
  printf("arg3 = %s\n", argv[2]);
  printf("tryng to find that last variable: "); printf("%s", argv[1]); 
  printf("\nstrcat result: %s\n", src);
    if(strcmp(src, "cd . ") == 0){
      printf("I am in cd . ");
      printf("before: %s\n", buf);
      chdir(buf);
      printf("after: %s\n", buf);
    } 
    else if(strcmp(src, "cd .. ") == 0){
      printf("I am in cd ..");
      printf("before: %s\n", buf);
      chdir("..");
      buf = pwd();
      printf("after: %s\n", buf);
    }
    else{
      printf("before: %s\n", buf);
      chdir(argv[1]);
//      int retval = chdir(argv[2]);
//      if(retval < 0){ fprintf(stderr, "directory not found.\n";);
      buf = pwd();
      printf("after: %s\n", buf);
      char *args[] = {"touch", "test.txt"};
      execvp(args[0], args);
    }
}
