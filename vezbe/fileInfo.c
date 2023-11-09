#define _XOPEN_SOURCE 700

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 1024

#define check_error(cond, msg)\
	do{\
		if(!(cond)){\
			perror(msg);\
			exit(1);\
		}\
	} while(0)


void fileinfo(char **s, const char *path);
char *getUserName(int userId);
char *getGroupName(gid_t groupId);

/*./fileInfo path*/
int main(int argc, char **argv){

	check_error(argc == 2,"argumenti");

	char *s = NULL;

	fileinfo(&s, argv[1]);
	printf("%s", s);
	
}

void fileinfo(char **s, const char *path){
	
	errno = EINVAL;
	check_error(*s == NULL, "init");
	errno = 0;

	//dodela podataka u strukturu
	struct stat info;
	check_error(stat(path, &info) != -1, "stat");

	//alociranje buffera
	char* buffer = malloc(MAX_SIZE * sizeof(char));
	check_error(buffer != NULL, "buffer malloc");
	*s = buffer;

	//prava pristupa
	char pravaPristupa[11];
	strcpy(pravaPristupa, "-rwxrwxrwx");

	switch (info.st_mode & S_IFMT)
	{
	case __S_IFREG:
		pravaPristupa[0] = '-';
		break;
	case __S_IFDIR:
		pravaPristupa[0] = 'd';
		break;
	case __S_IFCHR:
		pravaPristupa[0] = 'c';
		break;
	case __S_IFBLK:
		pravaPristupa[0] = 'b';
		break;
	case __S_IFLNK:
		pravaPristupa[0] = 'l';
		break;
	case __S_IFIFO:
		pravaPristupa[0] = 'p';
		break;
	case __S_IFSOCK:
		pravaPristupa[0] = 's';
		break;
	}

	if(!(info.st_mode & S_IRUSR)){
		pravaPristupa[1] = '-';
	}
	if(!(info.st_mode & S_IWUSR)){
		pravaPristupa[2] = '-';
	}
	if(!(info.st_mode & S_IXUSR)){
		pravaPristupa[3] = '-';
	}

	if(!(info.st_mode & S_IRGRP)){
		pravaPristupa[4] = '-';
	}
	if(!(info.st_mode & S_IWGRP)){
		pravaPristupa[5] = '-';
	}
	if(!(info.st_mode & S_IXGRP)){
		pravaPristupa[6] = '-';
	}

	if(!(info.st_mode & S_IROTH)){
		pravaPristupa[7] = '-';
	}
	if(!(info.st_mode & S_IWOTH)){
		pravaPristupa[8] = '-';
	}
	if(!(info.st_mode & S_IXOTH)){
		pravaPristupa[9] = '-';
	}

	//formiranje stringa
	int bytes = sprintf(buffer, "%s ", pravaPristupa);
	
	char *userName = getUserName(info.st_uid);
	char *groupName =  getGroupName(info.st_gid);
	char *vreme = ctime(&(info.st_mtim));

	bytes += sprintf(buffer + bytes, "%s ", userName);
	bytes += sprintf(buffer + bytes, "%s ", groupName);
	bytes += sprintf(buffer + bytes, "%d ", (int)info.st_size);
	bytes += sprintf(buffer + bytes, "%s", vreme);

	buffer[--bytes] = 0;

	bytes += sprintf(buffer + bytes, " %s ", path);

	free(userName);
	free(groupName);

}

char *getUserName(int userId){

	struct passwd *userInfo;
	userInfo = getpwuid(userId);

	check_error(userInfo != NULL, "userInfo");

	char *name = malloc(strlen(userInfo->pw_name) + 1);
	check_error(name != NULL, "");

	strcpy(name, userInfo->pw_name );

	return name;
}

char *getGroupName(gid_t grpId){

	struct group *groupInfo;
	groupInfo = getgrgid(grpId);
	check_error(groupInfo != NULL, "ginfo");

	char *name = malloc(strlen(groupInfo->gr_name) + 1);
	check_error(name != NULL, "");

	strcpy(name, groupInfo->gr_name);
	
	return name;
}