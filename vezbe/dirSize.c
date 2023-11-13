#define _XOPEN_SOURCE 700

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <dirent.h>

#include <string.h>

#define check_error(cond,userMsg)\
	do {\
		if (!(cond)) {\
			perror(userMsg);\
			exit(EXIT_FAILURE); \
		}\
	} while(0)


void dirSize(char* path, unsigned *size){
	
	struct stat fileInfo;
	check_error(lstat(path, &fileInfo) != -1, "mf");

	*size += fileInfo.st_size;

	if(!(S_ISDIR(fileInfo.st_mode))){
		return;
	}

	DIR *dir = opendir(path);
	check_error(dir != NULL, "...");

	struct dirent* dirEntry = NULL;
	errno = 0;

	while((dirEntry = readdir(dir)) != NULL){

		char *fPath = malloc(strlen(path) + strlen(dirEntry->d_name) + 2);
		check_error(fPath != NULL, "malloc");

		strcpy(fPath, path);
		strcat(fPath, "/");
		strcat(fPath, dirEntry->d_name);

		if (!strcmp(dirEntry->d_name, ".") || !strcmp(dirEntry->d_name, "..")) {

			check_error(stat(fPath, &fileInfo) != -1, "...");
			*size += fileInfo.st_size;
			
			free(fPath);
			errno = 0;
			continue;
		}

	
		dirSize(fPath, size);
		free(fPath);
		errno = 0;
	}

	check_error(errno != EBADF, "readdir");
	check_error(closedir(dir) != -1 , "closedir");

}


int main(int argc, char** argv) {
	
	check_error(argc == 2, "...");
	
	struct stat fInfo;
	check_error(stat(argv[1], &fInfo) != -1, "stat main");
	check_error(S_ISDIR(fInfo.st_mode), "nije dir");
	
	unsigned size = 0;
	dirSize(argv[1], &size);
	
	printf("Dir size: %uMB\n", size / 1024 / 1024);
	
	exit(EXIT_SUCCESS);
}
