#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define check_error(cond, msg)\
	do{\
		if(!(cond)){\
			fprintf(stderr, "err %s\n", msg);\
			exit(1);\
		}\
	} while(0)

/*myrm tip[-d -f] path */
int main(int argc, char **argv){
	check_error(argc == 3, "myrm tip[-d -f] path");

	if(strcmp(argv[1], "-f") == 0){

		check_error(unlink(argv[2]) != -1, "unlink");
	} else if(strcmp(argv[1], "-d") == 0){

		check_error(rmdir(argv[2]) != -1, "rmdir");
	} else {
		check_error(0, "losa opcija");
	}
}