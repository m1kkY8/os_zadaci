#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 2048

#define check_error(cond, msg)\
	do{\
		if(!(cond)){\
			fprintf(stderr, "err %s\n", msg);\
			exit(1);\
		}\
	} while(0)

/*mycat path*/
int main(int argc, char **argv){

	check_error(argc == 2, "argumenti");

	int fd = open(argv[1], O_RDONLY);
	check_error(fd != -1, "open");

	char buf[BUFFER_SIZE];
	int readBytes = 0;

	while(readBytes = read(fd, buf, BUFFER_SIZE)){
		check_error(write(1, buf, readBytes) != -1, "write");
	}

	check_error(readBytes != -1, "read");

	close(fd);
	return 0;
}