#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv) {

	if(argc <= 1) {
		printf("There is not enough arguments.\n");
		exit(0);
	}

	execvp(argv[1], &argv[1]);
	perror(argv[1]);
	exit(0);
}
