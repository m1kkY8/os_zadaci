#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#define check_error(cond, msg)\
	do{\
		if(!(cond)){\
			fprintf(stderr, "err %s\n", msg);\
			exit(1);\
		}\
	} while(0)

int my_fopen(const char *putanja, const char* mode){
	int flag = 0;

	switch (mode[0]) {
	case 'r':
		flag |= mode[1] == '+' ? O_RDWR : O_RDONLY;
		break;
	case 'w':
		flag |= mode[1] == '+' ? O_RDWR: O_WRONLY;
		flag |= O_CREAT;
		flag |= O_TRUNC;
		break;
	default:
		break;
	}

	mode_t prava_pristupa = 0644;

	int fd = open(putanja, flag, prava_pristupa);
	return fd;

}

int main(int argc, char **argv){
	check_error(argc == 3, "argumenti");

	int fd = my_fopen(argv[1], argv[2]);
	check_error(fd != -1, "fajl nije otvoren");

	close(fd);
	return 0;
}