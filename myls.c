#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <pwd.h>


int main(int argc, char **argv) {

   struct stat path_stat; 
   char **buf2 = malloc(sizeof(argv));  
   int i; 
   char *arg0;
   char *arg1; 
   char *arg2; 


   char **input = malloc(sizeof(argv[i])); 
   if(argc == 2) {
      arg1 = malloc(sizeof(argv[2])); 
      arg1 = argv[1]; 
      char **buf = malloc(sizeof(arg1)); 
      buf[0] = "ls"; 
      buf[1] = arg1; 
      buf[2] = 0; 

      //checking to see if arg1 is a directory, if not display error
      stat(arg1, &path_stat); 

      if(S_ISDIR(path_stat.st_mode)) {
         execvp( buf[0], buf);
      }
      else {
         printf("Directory does not exist.\n"); 
      }
   }
  
   else if(argc == 3) {
      arg0 = malloc(sizeof("ls"));
      arg0 = "ls"; 
      arg1 = malloc(sizeof(argv[1]));
      arg1 = argv[1];
      arg2 = malloc(sizeof(argv[2])); 
      arg2 = argv[2]; 
    
      char **arg3 = (char**)malloc(strlen(arg0) + strlen(arg1) + strlen(arg2) + 1); 
      arg3[0] = (char*)malloc(strlen(arg0));
      arg3[1] = (char*)malloc(strlen(arg1)); 
      arg3[2] = (char*)malloc(strlen(arg2));
      arg3[3] = 0;  
      arg3[0] = arg0; 
      arg3[1] = arg1;  
      arg3[2] = arg2; 

      
      execvp(arg3[0], arg3);  
   }

  return 0; 
}

