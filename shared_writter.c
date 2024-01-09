#define _XOPEN_SOURCE 700

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#define check_error(cond, msg) do{ if(!(cond)){ perror(msg); exit(1);}} while(0)

void* create_mem_block(char* fpath, unsigned size){
    int mem_fd = shm_open(fpath, O_RDWR | O_CREAT, 0600);
    check_error(mem_fd != -1, "");

    check_error(ftruncate(mem_fd, size) != -1 , "");

    void* address = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, 0);
    check_error(address != MAP_FAILED, "");

    close(mem_fd);
    return address;
}

int main(int argc, char **argv){
    
    check_error(argc == 2, "");
    
    int n = 10;
    int *array = create_mem_block(argv[1],n * sizeof(int));

    for(int i = 0; i < n; i++){
        array[i] = i * i;
    }

    check_error(munmap(array, n * sizeof(int)) != -1, "");



    exit(EXIT_SUCCESS);
}
