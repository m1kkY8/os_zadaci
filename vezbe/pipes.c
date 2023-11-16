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

//macros for closing fd's for one way pipe
#define RD_END (0)
#define WR_END (1)
#define BUFF_SIZE (1024)

int main(int argc, char **argv){
	//communication pipes
	int childToParent[2];
	int parentToChild[2];

	//creating pipse
	check_error(pipe(childToParent) != -1,"pipe");
	check_error(pipe(parentToChild) != -1, "pipe");

	//create child process
	pid_t childPid = fork();
	check_error(childPid != -1, "fork");

	if(childPid > 0){
		//parrent

		//closing pipes to make them oneway
		close(parentToChild[RD_END]);
		close(childToParent[WR_END]);

		//send message to child
		char buffer[BUFF_SIZE];
		sprintf(buffer, "%s", "ADOLF 1");

		check_error(write(parentToChild[WR_END], buffer, strlen(buffer)) != -1, "write parrent"); 

		//recive message from child
		char message[BUFF_SIZE];
		int readBytes = read(childToParent[RD_END], message, BUFF_SIZE);
		check_error(readBytes != -1, "read parent");
		message[readBytes] = 0;

		printf("Child message: %s\n", message);

		//close reaminging fd's
		close(parentToChild[WR_END]);
		close(childToParent[RD_END]);
	} else {
		//child

		//same as parent
		close(childToParent[RD_END]);
		close(parentToChild[WR_END]);

		//recive message from parrent
		char message[BUFF_SIZE];

		int readBytes = read(parentToChild[RD_END], message, BUFF_SIZE);
		check_error(readBytes != -1, "read");
		message[readBytes] = 0;

		char buffer[BUFF_SIZE];
		sprintf(buffer, "Parrent message: %s", message);
		printf("%s\n", buffer);

		//send message to prent
		sprintf(buffer, "%s", "ADOLF 2");
		check_error(write(childToParent[WR_END], buffer, strlen(buffer)) != -1, "write child");

		//same as in parrent
		close(childToParent[WR_END]);
		close(parentToChild[RD_END]);

		exit(EXIT_SUCCESS);
	}

	check_error(wait(NULL) != -1, "wait");

	exit(EXIT_SUCCESS);
}
