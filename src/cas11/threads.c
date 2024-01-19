#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>

#include <pthread.h>
#include <errno.h>

#define check_error(cond, msg) do{ if(!(cond)){ perror(msg); exit(1);}} while(0)

#define pt_error(pterr, msg) do {int pt_err = pterr; if(pt_err > 0){errno = pt_err; check_error(0 , msg);}} while(0) 

typedef struct{
    int idx;
} ThreadFunctionArg;

typedef struct {
} ThreadFunctionRet;

void* thread_function(void* arg){
    ThreadFunctionArg* farg = arg;
    printf("tID:%jd, thread no:%d\n", (intmax_t)pthread_self(), farg->idx);
    return NULL;
}

int main(int argc, char **argv){
    
    check_error(argc == 2, "args");
    
    int num_of_threads = atoi(argv[1]);
    pthread_t* thread_ids = malloc(num_of_threads * sizeof(pthread_t));
    check_error(thread_ids != NULL, "malloc");

    ThreadFunctionArg* data = malloc(num_of_threads * sizeof(ThreadFunctionArg));
    check_error(data != NULL, "malloc");

    for(int i = 0; i < num_of_threads; i++){

        data[i].idx = i;
        pt_error(pthread_create(&thread_ids[i], NULL, thread_function, &data[i]), "create");
    }
    
    for(int i = 0; i < num_of_threads; i++){
        pt_error(pthread_join(thread_ids[i], NULL), "join");
    }

    free(thread_ids);
    free(data);

    exit(EXIT_SUCCESS);

}
