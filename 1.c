#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <wait.h>

#define CMD_LEN 256

#define check_error(cond, msg) do{ if(!(cond)){ perror(msg); exit(1);}} while(0)

int main(int argc, char **argv){
    
    char* commands = malloc(argc * CMD_LEN);
    check_error(commands != NULL, "nesto");
    

    char user_input[CMD_LEN];

    do{
        fgets(user_input, CMD_LEN, stdin);

        if(!strcasecmp(user_input, "quit\n"))
            break;

        int exit_code = system(user_input);

        if(exit_code == 0)
            printf("gud\n");
        else
            printf("nogud\n");
    } while (strcasecmp(user_input, "quit"));

    printf("adios\n");

    exit(EXIT_SUCCESS);
}
