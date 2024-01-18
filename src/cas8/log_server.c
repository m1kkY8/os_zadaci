#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/mman.h>
#include <semaphore.h>

#include <string.h>
#include <strings.h>

#define check_error(cond,msg) do { if (!(cond)) { perror(msg); exit(EXIT_FAILURE);}} while (0)

#define MSG_LEN (256) 			/* maksimalna velicina poruke u bloku memorije */
#define LOCAL_SEM (0)			/* lokalni semafor - koristi se za sync tredova */
#define PROC_SEM (!LOCAL_SEM)	/* globalni semafor - koristi se za sync procesa */

typedef struct {
	char buf[MSG_LEN];
	sem_t safeToRead;
	sem_t safeToWrite;
} osMemoryBlock;

static char* osUsage = "./log_server pathFile";

void *osCreateMemoryBlock(const char* filePath, unsigned size) {
	
	int memFd = shm_open(filePath, O_RDWR | O_CREAT, 0600);
	check_error(memFd != -1, "shm_open failed");
	
	check_error(ftruncate(memFd, size) != -1, "ftruncate failed");
	
	void* addr;
	check_error((addr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, memFd, 0)) != MAP_FAILED, "mmap failed");
	
	close(memFd);
	
	return addr;
}

int main(int argc, char** argv) {
	
	check_error(argc == 2, osUsage);
	
	osMemoryBlock* pMsgBuf = osCreateMemoryBlock(argv[1], sizeof(osMemoryBlock));
	check_error(sem_init(&pMsgBuf->safeToRead, PROC_SEM, 0) != -1, "read sem init failed");
	check_error(sem_init(&pMsgBuf->safeToWrite, PROC_SEM, 1) != -1, "write sem init failed");
	
	char localBuf[MSG_LEN];
	
	do {
		scanf("%s", localBuf);
		
		check_error(sem_wait(&pMsgBuf->safeToWrite)!=-1, "sem wait failed");
		strcpy(pMsgBuf->buf, localBuf);
		check_error(sem_post(&pMsgBuf->safeToRead) != -1, "sem post failed");
		
	} while(strcasecmp(localBuf, "quit"));

    check_error(munmap(pMsgBuf, sizeof(osMemoryBlock)) != -1, "munmap failed");
	check_error(shm_unlink(argv[1]) != -1, "shm unlink failed");
	
	exit(EXIT_SUCCESS);
}

