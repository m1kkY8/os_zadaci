/*
umask - kreira fajl sa custom pravima pristupa
*/

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

/*./umask ime_fajla prava*/
int main(int argc, char **argv){
	check_error(argc == 3, "argumenti");

	int pravaPristupa = strtol(argv[2], NULL, 8);
	
	mode_t oldUmask = umask(0);
	int fd = open(argv[1], O_CREAT | O_EXCL, pravaPristupa);
	check_error(fd != -1, "open");

	close(fd);
	umask(oldUmask);
	exit(EXIT_SUCCESS);
}
