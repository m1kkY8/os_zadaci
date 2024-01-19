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

void* thread_function(void* arg){
    (void)arg;
    sleep(2);
    printf("deatached thread\n");
    return NULL;
}

int main(int argc, char **argv){
    
    pthread_t background_thread;
    pt_error(pthread_create(&background_thread, NULL, thread_function, NULL), "create");
    pt_error(pthread_detach(background_thread), "detach");

    sleep(5);

    exit(EXIT_SUCCESS);

}
