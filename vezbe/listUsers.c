#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>

#include <stdio.h>
#include <stdlib.h>

#define _XOPEN_SOURCE 700

#define check_error(cond, msg)\
	do{\
		if(!(cond)){\
			fprintf(stderr, "err %s\n", msg);\
			exit(1);\
		}\
	} while(0)

void print_users(struct passwd* userInfo){

	fprintf(stdout, "\n");
	fprintf(stdout, "Username: %s\n", userInfo->pw_name);
	fprintf(stdout, "UID: %d\n", userInfo->pw_uid);
	fprintf(stdout, "GID: %d\n", userInfo->pw_gid);
	fprintf(stdout, "Home dir: %s\n", userInfo->pw_dir);
	fprintf(stdout, "Shell: %s\n", userInfo->pw_shell);
}

int main(int argc, char **argv){
	check_error(argc == 1, "argumenti");

	setpwent();

	struct passwd *user = NULL;

	while((user = getpwent()) != NULL){
		print_users(user);
	}

	endpwent();

	return 0;
}