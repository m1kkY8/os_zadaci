#include <sys/types.h>
#include <sys/stat.h>
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

	char* message = "amogus";

	int stdinFlags = fcntl(STDIN_FILENO, F_GETFL);
	check_error(stdinFlags != -1, "...");

	int accessMode = stdinFlags & O_ACCMODE;
	if(accessMode == O_WRONLY || accessMode == O_RDWR){
		check_error(write(STDIN_FILENO,message, strlen(message)) != -1, "...");
	}

	int fd = open("1.txt", O_CREAT | O_WRONLY, 0666);
	check_error(fd != -1, "...");

	check_error(write(fd, message, strlen(message)) != -1, "...");	
	int fileFlags = fcntl(fd, F_GETFL);
	check_error(fileFlags != -1, "...");

	//MOZE
	fileFlags = fileFlags | O_APPEND;
	
	// NE MOZE
	// fileFlags = fileFlags & ~O_WRONLY;
	// fileFlags = fileFlags | O_RDONLY;

	check_error(fcntl(fd, F_SETFL, fileFlags) != -1, "...");	
	check_error(write(fd, message, strlen(message)) != -1, "...");

	close(fd);
	exit(EXIT_SUCCESS);
}
