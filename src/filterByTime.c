#define _XOPEN_SOURCE 700

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <ftw.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define SECHOUR (3600)
#define SECDAY ((24)*(SECHOUR))

#define check_error(cond, msg)\
	do{\
		if(!(cond)){\
      		perror(msg);\
			exit(1);\
		}\
	} while(0)


time_t now;

int processFile(const char *fpath, const struct stat *sb, int typeFlag, struct FTW *ftwbuf){

	if(typeFlag == FTW_F){

		time_t timedif = (now - sb->st_mtime) / (SECDAY);
		if(timedif < 7)
		printf("%s/&s\n",fpath ,fpath + ftwbuf->base);
	}


	return 0;
}

/* filter path*/
int main(int argc, char **argv){
	check_error(argc == 2, "argumenti");

	struct stat fileInfo;
	check_error(stat(argv[1], &fileInfo) != -1, "stat");
	check_error(S_ISDIR(fileInfo.st_mode), "...");

	check_error(time(&now) != -1, "");
	check_error(nftw(argv[1], processFile, 50, 0) != -1, "...");

	exit(EXIT_SUCCESS);
}
