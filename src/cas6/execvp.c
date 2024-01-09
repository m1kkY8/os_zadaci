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
	
	check_error(argc > 1, "");

	char** arguments = malloc(argc * sizeof(char*));
	check_error(arguments != NULL, "");

	for(int i = 1; i < argc; i++){
		arguments[i-1] = malloc(strlen(argv[i]) + 1);
		check_error(arguments[i-1] != NULL, "");
		strcpy(arguments[i-1], argv[i]);
	}
	arguments[argc - 1] = NULL;

	pid_t childPid = fork();
	check_error(childPid != -1, "");

	if(childPid == 0){

		check_error(execvp(arguments[0], arguments) != -1, "");

		exit(EXIT_SUCCESS);
	}

	int status = 0;
	check_error(wait(&status) != -1, "");
	
	if(WIFEXITED(status) && (WEXITSTATUS(status) == EXIT_SUCCESS)){
		printf("alal vera\n");
	} else {
		printf("kurcina\n");
	}
	

	exit(EXIT_SUCCESS);
}
