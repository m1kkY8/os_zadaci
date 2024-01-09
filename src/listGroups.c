#define _XOPEN_SOURCE 700

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

void print_groups(struct group *grinfo){

	fprintf(stdout, "\n");
	fprintf(stdout, "%s\n", grinfo->gr_name);
	fprintf(stdout, "%d\n", grinfo->gr_gid);
	
	for(int i = 0; grinfo->gr_mem[i] != NULL; i++){
		fprintf(stdout, "%s\n", grinfo->gr_mem[i]);
	}

}

int main(int argc, char **argv){
	
	check_error(argc == 1, "argumenti");
	setgrent();

	struct group *grp = NULL;
	while((grp = getgrent()) != NULL){
		print_groups(grp);
	}

	endgrent();
	return 0;
}