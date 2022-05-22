#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "LineParser.h"
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char** argv)
{
    cmdLine* cmd = parseCmdLines("ls -l | head -n 2");
    int debug = 0;
    for(int i = 1; i < argc && !debug; i++)
    {
        debug = strcmp(argv[i], "-d") == 0 ? 1 : 0;
    }
    int fd[2];
    int errpipe = pipe(fd);
    if (errpipe == -1)
    {
        perror("Error with pipe");
        _exit(errpipe);
    }
    if (debug) fprintf(stderr, "(parent_process>forking...)\n");
    int ch1 = fork();
    if (!ch1)
    {
        if (debug) fprintf(stderr, "(child1>redirecting stdout to the write end of the pipe...)\n");
        close(1);
        dup(fd[1]);
        close(fd[1]);
        if (debug) fprintf(stderr, "(child1>going to execute cmd: %s)\n", cmd->arguments[0]);
        int err = execvp(cmd->arguments[0], cmd->arguments);
        if (err < 0)
            {
                perror("Error");
                _exit(err);
            }
        
    }
    else
    {
        if (debug) 
        {
            fprintf(stderr, "(parent_process>created process with id: %i)\n", ch1);
            fprintf(stderr, "(parent_process>closing the write end of the pipe...)\n");
        }
        close(fd[1]);
        if (debug) fprintf(stderr, "(parent_process>forking...)\n");
        int ch2 = fork();
        if (!ch2)
        {
            if (debug) fprintf(stderr, "(child2>redirecting stdout to the read end of the pipe...)\n");
            close(0);
            dup(fd[0]);
            close(fd[0]);
            if (debug) fprintf(stderr, "(child2>going to execute cmd: %s)\n", cmd->next->arguments[0]);
            int err = execvp(cmd->next->arguments[0], cmd->next->arguments);
            if (err < 0)
                {
                    perror("Error");
                    _exit(err);
                }
        }
        if (debug) 
        {
            fprintf(stderr, "(parent_process>created process with id: %i)\n", ch2);
            fprintf(stderr, "(parent_process>closing the read end of the pipe...)\n");
        }
        close(fd[0]);
        if (debug)
            fprintf(stderr, "(parent_process>waiting for child process to terminate...)\n");
        if (cmd->blocking)
            waitpid(ch1, NULL, 0);
        if (cmd->next != NULL && cmd->next->blocking)
            waitpid(ch2, NULL, 0);
    }
    if (debug) fprintf(stderr, "(parent_process>exiting...)\n");
    return 0;
}