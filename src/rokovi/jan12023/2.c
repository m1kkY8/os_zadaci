#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ftw.h>
#include <time.h>
#include <string.h>
#include <strings.h>

#define cerr(cond, msg) do{ if(!(cond)){ perror(msg); exit(1);}} while(0)

#define SEC_IN_DAY 3600
#define WEEK 7 * SEC_IN_DAY

char* extension = NULL;
int size_sum = 0;

int filter_by_time(const char* fpath, const struct stat* sb, int typeflag, struct FTW *ftwbuf){ 

    if(typeflag == FTW_F){

        char* file_extension = strchr(fpath + ftwbuf->base, '.');

        time_t current_time = time(NULL);
        time_t time_since_mod = current_time - sb->st_mtime; 
        
        if(time_since_mod < WEEK){
            if(extension != NULL && !strcmp(file_extension, extension))
                size_sum += sb->st_size;
        }
    }
    return 0;
}
int main(int argc, char** argv){

    cerr(argc == 3, "args");

    extension = argv[2];

    struct stat file_info;
    cerr(stat(argv[1], &file_info) != -1,  "stat");
    cerr(S_ISDIR(file_info.st_mode), "not dir");

    cerr(nftw(argv[1], filter_by_time, 50, 0) != -1, "ntfw");

    printf("%d\n", size_sum);    

    return 0;
}
