#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
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

   char *buf;  

   buf = pwd();

   printf("%s",buf);


  return 0; 
}
