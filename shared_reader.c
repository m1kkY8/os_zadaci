#define _XOPEN_SOURCE 700

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#define check_error(cond, msg) do{ if(!(cond)){ perror(msg); exit(1);}} while(0)

void* get_memory_block(const char* fpath, unsigned *size){
    
    int mem_fd = shm_open(fpath, O_RDWR, 0600);
    check_error(mem_fd != -1, "");

    struct stat file_info;
    check_error(fstat(mem_fd, &file_info) != -1, "");

    *size = file_info.st_size;

    void* address = mmap(NULL, *size, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, 0);
    check_error(address != MAP_FAILED, "");

    close(mem_fd);
    return address;

}

int main(int argc, char **argv){
    
    check_error(argc == 2, "");
    
    unsigned size = 0;
    int* array = get_memory_block(argv[1], &size);
    int n = size / sizeof(int);

    for(int i = 0; i < n; i++)
        printf("%d ", array[i]);

    check_error(munmap(array, size), "");
    check_error(shm_unlink(argv[1]) != -1, "");


    exit(EXIT_SUCCESS);
}
