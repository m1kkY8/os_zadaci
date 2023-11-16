#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define check_error(cond, msg)\
	do{\
		if(!(cond)){\
      		perror(msg);\
			exit(1);\
		}\
	} while(0)

int main(int argc, char **argv){
	
	pid_t childPid = fork();
	check_error(childPid != -1, "fork");

	if(childPid == 0){
		check_error(execlp("/home/tox/dev/os/pipes", "pipes", NULL) != -1, "exec");
		
		exit(EXIT_SUCCESS);
	}

	int status = 0;
	check_error(wait(&childPid) != -1, "wait");

	if(WIFEXITED(status) && (WEXITSTATUS(status) == EXIT_SUCCESS)){
		printf("alal vera");
	} else {
		printf("kurac");
	}

	exit(EXIT_SUCCESS);
}
