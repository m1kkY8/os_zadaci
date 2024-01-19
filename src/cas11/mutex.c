#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <errno.h>
#include <pthread.h>

#include <stdatomic.h>

#define check_error(cond,msg)\
  do {\
    if (!(cond)) {\
      perror(msg);\
      fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
      exit(EXIT_FAILURE);\
    }\
  } while (0)

#define pt_err(pthreadErr, userMsg) \
	do { \
		int _pthreadErr = pthreadErr; \
		if (_pthreadErr > 0) { \
			 errno = _pthreadErr; \
			 check_error(false, userMsg); \
		 }\
	} while (0)

typedef struct{
    int sum;
    pthread_mutex_t lock;
} SafeInt;

SafeInt global_sum;

void* thread_function(void* arg){
    
    char* file_path = arg;
    FILE* f = fopen(file_path, "r");
    check_error(f != NULL, "fopen");

    int sum = 0;
    int current = 0;

    while(fscanf(f, "%d", &current) == 1){
        sum += current;
    } 
    check_error(ferror(f) == 0, "read");

    pt_err(pthread_mutex_lock(&global_sum.lock), "lock");
    global_sum.sum += sum;
    pt_err(pthread_mutex_unlock(&global_sum.lock), "lock");
    
    fclose(f);

    return NULL;
}

int main(int argc, char** argv){

    check_error(argc >= 2, "args");
    global_sum.sum = 0;
    
    pt_err(pthread_mutex_init(&global_sum.lock, NULL), "mutex init");

    int no_of_threads = argc - 1;
    pthread_t* thread_ids = malloc(no_of_threads * sizeof(pthread_t));
    check_error(thread_ids != NULL, "");

    for(int i = 0; i < no_of_threads; i++){
        pt_err(pthread_create(&thread_ids[i], NULL, thread_function, argv[i+1]), "create");
    }

    for(int i = 0; i < no_of_threads; i++){
        pt_err(pthread_join(thread_ids[i], NULL), "nesto");
    }

    printf("Sum: %d", global_sum.sum);

    free(thread_ids);

    pt_err(pthread_mutex_destroy(&global_sum.lock), "mutex destroy");
    
    exit(0);
}
