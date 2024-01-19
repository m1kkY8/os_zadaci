#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pwd.h>

#define cerr(cond, msg) do{ if(!(cond)){ perror(msg); exit(1);}} while(0)

#define MAX_USER_LEN 256
#define MAX_FILE_LEN 256

int main(int argc, char** argv){
    cerr(argc >= 2, "args");
    
    struct stat file_info;
    struct passwd* user_info = NULL;
    char user_name[MAX_USER_LEN];
    int *sizes = malloc(sizeof(int) * (argc - 1));
    
    for(int i = 0; i < argc - 1; i++){
        if(stat(argv[i+1], &file_info) != 0){
            continue;
        } else {
            if((file_info.st_mode & S_IFMT) == S_IFREG){
                sizes[i] = file_info.st_size;
            } else 
                sizes[i] = -1;


        }
        
    }
    
    uid_t user_id = file_info.st_uid;
    cerr((user_info = getpwuid(user_id)) != NULL, "pwudi");
    strcpy(user_name, user_info->pw_name);

    printf("%s ", user_name);

    for(int i = 0; i < argc - 1; i++){
        printf("%d ",sizes[i]);
        
    }

    free(sizes);
    exit(0);
}
