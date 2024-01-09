#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>

#define check_error(cond, msg)\
	do{\
		if(!(cond)){\
      		perror(msg);\
			exit(1);\
		}\
	} while(0)

int main(int argc, char **argv){
	
	check_error(execlp("nvim", NULL) != -1, "exec");
	// check_error(execlp("ls", "ls", "-lath", NULL) != -1, "exec");

	printf("amogus\n");

	exit(EXIT_SUCCESS);
}
