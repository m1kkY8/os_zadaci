#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <grp.h>

#include <stdio.h>
#include <stdlib.h>

#define check_error(cond, msg)\
	do{\
		if(!(cond)){\
			fprintf(stderr, "err %s\n", msg);\
			exit(1);\
		}\
	} while(0)

/*group gname*/
int main(int argc, char **argv){
	check_error(argc == 2, "argumenti");

	struct group *grinfo = getgrnam(argv[1]);

	fprintf(stdout, "%s\n", grinfo->gr_name);
	fprintf(stdout, "%d\n", grinfo->gr_gid);
	
	for(int i = 0; grinfo->gr_mem[i] != NULL; i++){
		fprintf(stdout, "%s\n", grinfo->gr_mem[i]);
	}

}