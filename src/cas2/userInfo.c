#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pwd.h>

#include <stdio.h>
#include <stdlib.h>

#define check_error(cond, msg)\
	do{\
		if(!(cond)){\
			fprintf(stderr, "err %s\n", msg);\
			exit(1);\
		}\
	} while(0)

/*userinfo username*/
int main(int argc, char **argv){
	check_error(argc == 2, "userinfo username");

	struct passwd *userInfo = getpwnam(argv[1]);
	check_error(userInfo != NULL, "nema");

	fprintf(stdout, "Username: %s\n", userInfo->pw_name);
	fprintf(stdout, "UID: %d\n", userInfo->pw_uid);
	fprintf(stdout, "GID: %d\n", userInfo->pw_gid);
	fprintf(stdout, "Home dir: %s\n", userInfo->pw_dir);
	fprintf(stdout, "Shell: %s\n", userInfo->pw_shell);
}