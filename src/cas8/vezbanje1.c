#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <wait.h>

#define RD 0
#define WR 1
#define CMD_LEN 256

#define check_error(cond, msg) do{ if(!(cond)){ perror(msg); exit(1);}} while(0)

int main(int argc, char **argv){
    
    int parent_child[2];
    int child_parent[2];

    check_error(pipe(parent_child) != -1, "pipe");
    check_error(pipe(child_parent) != -1, "pipe");

    pid_t child_pid = fork();
    check_error(child_pid != -1, "fork");

    if(child_pid == 0){
    
        close(parent_child[WR]);
        close(child_parent[RD]);

        while(1){
            char command[CMD_LEN];
            int read_bytes = 0;

            check_error((read_bytes = read(parent_child[RD], command, CMD_LEN)) != -1, "read");
            command[read_bytes] = '\0';

            check_error(freopen("/dev/null", "w", stdout) != NULL, "");
            check_error(freopen("/dev/null", "w", stderr) != NULL, "");
            
            int exit_code = system(command);

            check_error(write(child_parent[WR], &exit_code, sizeof(exit_code)), "cwrite");
        }
        close(parent_child[RD]);
        close(child_parent[WR]);
    
        exit(0);

    } else { //prnt
        
        char command[CMD_LEN];

        close(parent_child[RD]);
        close(child_parent[WR]);

        do{
            fgets(command, CMD_LEN, stdin);

            if(!strcasecmp(command, "quit\n")){
                kill(child_pid, SIGKILL);
                break;
            }

            check_error(write(parent_child[WR], command, strlen(command)) != -1, "write");

            int exit_code;
            check_error(read(child_parent[RD], &exit_code, sizeof(exit_code)) != -1 , "nesto");
            
            if(WIFEXITED(exit_code)){
                if(WEXITSTATUS(exit_code) == EXIT_SUCCESS)
                    printf("Success\n");
                else
                    printf("Fail\n");

            }else{
                printf("Fail\n");
            }

        } while(1);
 
        close(parent_child[WR]);
        close(child_parent[RD]);
        
    }
    check_error(wait(NULL) != -1, "wait");

    printf("adios\n");

    exit(EXIT_SUCCESS);
}
