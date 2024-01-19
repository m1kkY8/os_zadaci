#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#define check_error(cond, msg) do{ if(!(cond)){ perror(msg); exit(1);}} while(0)

#define pthread_error(pt_er, msg) do{ int per = pt_er;if(per > 0){errno = per; check_error(0, msg);}} while(0) 

#define NO_OF_THREADS 26

typedef struct{
    int t_id;
} LocalThread;

typedef struct{
    int total_sum;
    pthread_mutex_t lock;
} SafeSum;

SafeSum total_sum;

int letters[NO_OF_THREADS];
char* filepath = NULL;

void* count_letters(void* arg){
    
    LocalThread* t = arg;
    int local_sum = 0;
    FILE* f = fopen(filepath, "r");
    check_error(f != NULL, "fopen");
    
    char c;
    while((c = fgetc(f)) != EOF){
        if(isalpha(c) && c == 'a' + t->t_id){
            local_sum++;
        }
    }
    letters[t->t_id] = local_sum;
        
    pthread_error(pthread_mutex_lock(&total_sum.lock), "msg");
    total_sum.total_sum += local_sum;
    pthread_error(pthread_mutex_unlock(&total_sum.lock), "msg");

    fclose(f);

    return NULL;
}

int main(int argc, char** argv){
    
    check_error(argc == 2, "args");
    total_sum.total_sum = 0; 
    
    pthread_error(pthread_mutex_init(&total_sum.lock, NULL), "mut init");

    filepath = argv[1];
    
    pthread_t* thread_ids = malloc(NO_OF_THREADS * sizeof(pthread_t));
    check_error(thread_ids != NULL, "");

    LocalThread* thread_letter = malloc(NO_OF_THREADS * sizeof(LocalThread));
    check_error(thread_letter != NULL, "");


    for(int i = 0; i < NO_OF_THREADS; i++){
        thread_letter[i].t_id = i;
        pthread_error(pthread_create(&thread_ids[i], NULL, count_letters, &thread_letter[i]), "create");
    }

    for(int i = 0; i < NO_OF_THREADS; i++){
        pthread_error(pthread_join(thread_ids[i], NULL), "join");
    }
    
    int k;
    int max = -1;
    for(int i = 0; i < NO_OF_THREADS; i++){
        if(letters[i] > max){
            max = letters[i];
            k = i;
        }
    }
   
    pthread_error(pthread_mutex_destroy(&total_sum.lock), "mutex del");
    printf("%d %c %d", total_sum.total_sum, 'a' + k, letters[k]);

    exit(0);

}

