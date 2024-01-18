#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include <sys/mman.h>
#include <semaphore.h>

#define check_error(cond, msg) do{ if(!(cond)){ perror(msg); exit(1);}} while(0)

#define MAX_LEN (1024)

typedef struct{
    sem_t ready;
    sem_t sorted;
    double data[MAX_LEN];
    unsigned num_of_elements;
} SharedArray;

void* get_memory_block(const char* filepath, int* block_size){
    
    int block_fd = shm_open(filepath, O_RDWR, 0);
    check_error(block_fd != -1, "shm");

    struct stat file_info;
    check_error(fstat(block_fd, &file_info) != -1, "stat");
    *block_size = file_info.st_size;

    void* addr;
    check_error(MAP_FAILED != (addr = mmap(NULL, *block_size, PROT_READ | PROT_WRITE, MAP_SHARED, block_fd, 0)), "mmap");
    close(block_fd);
    return addr;
}

int main(int argc, char **argv){
        
    check_error(argc == 2, "");
    int block_size;
    SharedArray* data = get_memory_block(argv[1], &block_size);
    check_error(sem_wait(&data->ready) != -1, "wait");

    for(int i = 0; i < data->num_of_elements; i++){
        for(int j = 0; j < data->num_of_elements; j++){
            if(data->data[i] > data->data[j]){
                
                double tmp = data->data[i];
                data->data[i] = data->data[j];
                data->data[j] = tmp;
            } 
        }
    }

    check_error(sem_post(&data->sorted) != -1, "post");
    check_error(munmap(data, block_size) != -1, "unmap");

    exit(EXIT_SUCCESS);
}
