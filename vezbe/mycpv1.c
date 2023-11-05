#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#define BUFF_SIZE 2048

#define check_error(cond, msg)\
	do{\
		if(!(cond)){\
			fprintf(stderr, "err %s\n", msg);\
			exit(1);\
		}\
	} while(0)

/*mycp src dest*/
int main(int argc, char **argv){
	check_error(argc == 3, "mycp src dest");

	int srcfd = open(argv[1], O_RDONLY);
	check_error(srcfd != -1, "srcfd");

	int destfd = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	check_error(destfd != -1, "destfd");

	char *buf = malloc(BUFF_SIZE);
	check_error(buf != NULL, "buffer");

	int readBytes = 0;

	while(readBytes = read(srcfd, buf, BUFF_SIZE)){
		check_error(write(destfd, buf, readBytes) != -1, "write");
	}

	check_error(readBytes != -1, "read");

	close(srcfd);
	close(destfd);
	free(buf);
	return 0;
}