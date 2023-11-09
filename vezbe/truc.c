#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <utime.h>

#include <stdio.h>
#include <stdlib.h>

#define check_error(cond, msg)\
	do{\
		if(!(cond)){\
      		perror(msg);\
			exit(1);\
		}\
	} while(0)

int main(int argc, char **argv){
	check_error(argc == 2, "argumenti");

	struct stat fileInfo;
	check_error(stat(argv[1], &fileInfo) != -1, "stat");
	
	FILE* f = fopen(argv[1], "w");
	fclose(f);

	struct utimbuf oldTime;
	oldTime.actime = fileInfo.st_atime;
	oldTime.modtime = fileInfo.st_mtime;

	check_error(utime(argv[1], &oldTime) != -1, "...");

	exit(0);
}
