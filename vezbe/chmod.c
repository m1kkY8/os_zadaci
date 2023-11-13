/* chmod - menja prava pristupa */

#include <sys/types.h>
#include <sys/stat.h>
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

/* ./chmod fajl prava */
int main(int argc, char **argv){
	check_error(argc == 3, "argumenti");

	int pravaPristupa = strtol(argv[2], NULL, 8);
	int fd = open(argv[1], O_CREAT, pravaPristupa);
	check_error(fd != -1, "open");
	close(fd);

	check_error(chmod(argv[1], pravaPristupa) != -1, "chmod");

	exit(EXIT_SUCCESS);
}
