#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#include <sys/mman.h>
#include <semaphore.h>
#include <string.h>
#include <strings.h>

#define check_error(cond,msg)\
  do {\
    if (!(cond)) {\
      perror(msg);\
      fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
      exit(EXIT_FAILURE);\
    }\
  } while (0)

#define MSG_LEN (256)
#define LOCAL_SEM (0)
#define PROC_SEM (!(LOCAL_SEM))

typedef struct {
	char buf[MSG_LEN];
	sem_t safeToRead;
	sem_t safeToWrite;
} osMemoryBlock;	

static char* osUsage = "./log_client pathFile";


void* osGetMemoryBlock(const char* filePath, unsigned* size) {
	
	int memFd = shm_open(filePath, O_RDWR, 0);
	check_error(memFd != -1, "shm open failed");
	
	struct stat fInfo;
	check_error(fstat(memFd, &fInfo) != -1, "stat failed");
	*size = fInfo.st_size;
	
	void *addr;
	check_error((addr = mmap(0, *size, PROT_READ | PROT_WRITE, MAP_SHARED, memFd, 0)) != MAP_FAILED, "mmap failed");
	
	close(memFd);
	
	return addr;
}

int main(int argc, char** argv) {
	
	check_error(argc == 2, osUsage);
	
	unsigned size;
	osMemoryBlock *pMsgBuf = osGetMemoryBlock(argv[1], &size);
	
	char localBuf[MSG_LEN];
	do {
		check_error(sem_wait(&pMsgBuf->safeToRead) != -1, "sem wait failed");
		strcpy(localBuf, pMsgBuf->buf);
		check_error(sem_post(&pMsgBuf->safeToWrite) != -1, "sem post failed");
		printf("Received message: '%s'\n", localBuf);
		
	} while(strcasecmp(localBuf, "quit"));
	check_error(munmap(pMsgBuf, size) != -1, "munmap failed");
	
	exit(EXIT_SUCCESS);
}

