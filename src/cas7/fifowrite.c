#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define check_error(cond, msg) do{ if(!(cond)){ perror(msg); exit(1);}} while(0)

int main(int argc, char **argv){
    
    check_error(argc == 2,"");

    struct stat file_info;
    int file_info_return = stat(argv[1], &file_info);

    if(file_info_return == -1){
        if(errno != ENOENT)
            check_error(0,"");

        check_error(mkfifo(argv[1], 0777) != -1, "");
    }

    int fifo_fd = open(argv[1], O_WRONLY);
    check_error(fifo_fd != -1, "");

    srand(time(NULL));
    int x; 
    char response[30];

    do {
        x = rand();
        check_error(write(fifo_fd, &x, sizeof(int)) != -1, "");
        printf("Sent %d\n", x);

        printf("Send next (y|n):");
        scanf("%s", response);

    } while (strcmp(response, "no"));


    close(fifo_fd);
    exit(EXIT_SUCCESS);
}
