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
			fprintf(stderr, "err %s\n", msg);\
			exit(1);\
		}\
	} while(0)

/*mymkdir path mode*/
int main(int argc, char **argv){
	check_error(argc == 3, "argumenti");

	int mode = strtol(argv[2], NULL, 8);
	int val = mkdir(argv[1], mode);
	
	check_error(val != -1, "neuspesno kreiranje");

	return 0;
}