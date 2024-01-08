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

    int fifo_fd = open(argv[1], O_RDONLY);
    check_error(fifo_fd != -1, "");

    int x; 
    int read_bytes;

    while((read_bytes = read(fifo_fd, &x, sizeof(int))) > 0 ){
        printf("Recieved %d\n", x);
    }

    check_error(read_bytes != -1, "");

    close(fifo_fd);
    exit(EXIT_SUCCESS);
}
