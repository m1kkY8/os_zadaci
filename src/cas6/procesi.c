#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>

#define check_error(cond, msg) \
	do                         \
	{                          \
		if (!(cond))           \
		{                      \
			perror(msg);       \
			exit(1);           \
		}                      \
	} while (0)

int main(int argc, char **argv)
{

	pid_t childPID = fork();

	check_error(childPID != -1, "...");

	if (childPID > 0)
	{
		// parent

		printf("Parrent\nMy ID: %d\nChild ID: %d\n", (int)getpid(), (int)childPID);
	}
	else
	{
		// child
		printf("Child\nMy ID %d\nParrent ID: %d\n", (int)getpid(), (int)getppid());
		exit(EXIT_SUCCESS);
	}

	printf("ovo je parrent\n");

	int status = 0;
	check_error(wait(&status) != -1, "...");

	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == EXIT_SUCCESS)
		{
			printf("uspeh");
		}
		else
		{
			printf("jok");
		}
	}
	else
	{
		printf("jok");
	}

	exit(EXIT_SUCCESS);
}
