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
	
	int x = 10;
	int y = 5;

	pid_t childPid = fork();
	check_error(childPid != -1, "fork");

	if(childPid > 0){
		printf("Parent\n");
		//parrent

		x += 20;
		y -= 10;

		printf("%d, %d\n", x, y);

	} else {
		//child
		printf("Shild\n");

		x += 3;
		y -= 2;

		printf("%d, %d\n", x, y);


		exit(EXIT_SUCCESS);
	}

	int status = 0;
	check_error(waitpid(childPid, &status, 0) != -1, "wait");

	if(WIFEXITED(status)){
		if(WEXITSTATUS(status) == EXIT_SUCCESS){
			printf("Uspeh\n");
		} else {
			printf("jok\n");
		}
	} else {
		printf("jok\n");
	}

	exit(EXIT_SUCCESS);
}
