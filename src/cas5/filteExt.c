#define _XOPEN_SOURCE 700

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <ftw.h>

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

char *ext = NULL;

int processFile(const char* fpath, const struct stat* sb, int type, struct FTW* ftwbuf){

	if(type == FTW_F){
		char *extension = strrchr(fpath + ftwbuf->base, '.');
		if(extension != NULL){
			if(strcmp(extension, ext) == 0){
				printf("%s\n", fpath + ftwbuf->base);
			}
		}
	}

	return 0;
}

/*filterext path ext*/
int main(int argc, char **argv){
	check_error(argc == 3, "argumenti");
	
	ext = argv[2];

	struct stat fileInfo;
	check_error(stat(argv[1], &fileInfo) != -1, "stat");
	check_error(S_ISDIR(fileInfo.st_mode), "...");

	check_error(nftw(argv[1], processFile, 50, 0) != -1, "...");

	exit(EXIT_SUCCESS);
}
