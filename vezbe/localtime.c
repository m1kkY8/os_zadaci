#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

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
	
	time_t now;
	check_error(time(&now) != -1, ">");
	
	struct tm *vreme = localtime(&now);
	check_error(vreme != NULL, ">");
	
	vreme->tm_year++;

	time_t newTime = mktime(vreme);
	printf("%s\n", ctime(&newTime));

	exit(EXIT_SUCCESS);
}
