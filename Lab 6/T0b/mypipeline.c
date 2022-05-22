#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "LineParser.h"
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/wait.h>

cmdLine* execute(cmdLine* pCmdLine, int debug);

int main(int argc, char** argv) {
    /* check for debug in argv */
    int debug = 0;
    for (int i = 1; i < argc; i++)
        if (strcmp(argv[i], "-d") == 0) 
        {
            debug = 1;
            break;
        }


    while(1)
    {
        char cdbuffer[PATH_MAX];
        char* dir = getcwd(cdbuffer, PATH_MAX);
        printf("%s$", dir);
        char userInput[2048];
        fgets(userInput, 2048, stdin);
        freeCmdLines(
            execute(parseCmdLines(userInput), debug)
        );
    }
    return 0;
}

void changeDir(char* dir) 
{
    int cderr = chdir(dir);
    if (cderr < 0)
    {
        perror("Error changing directory");
        _exit(cderr);
    }
}

int specialForms(cmdLine* pCmdLine) 
{
    int ret = 1;
    strcmp(pCmdLine->arguments[0], "quit") == 0 ? _exit(0) :
    strcmp(pCmdLine->arguments[0], "cd") == 0 ? changeDir(pCmdLine->arguments[1]) : ret--;
    return ret;
}

cmdLine* singleCommand(cmdLine* pCmdLine, int debug)
{
    int pid = fork();
    if (!pid) // pid is 0 if it is a child process, meaning it forked successfully 
    {
        int err = execvp(pCmdLine->arguments[0], pCmdLine->arguments);
        if (err < 0)
            {
                perror("Error");
                _exit(err);
            }
    }

    if (debug)
        fprintf(stderr, "PID %d\n Command: %s\n\n", pid, pCmdLine->arguments[0]);

    if (pCmdLine->blocking)
        waitpid(pid, NULL, 0);

    return pCmdLine;

}

cmdLine* execute(cmdLine* pCmdLine, int debug)
{
    if (specialForms(pCmdLine)) return pCmdLine; // Doesn't need to fork process if it was quit(stopped) / cd(done) / showprocs(done)
    if (pCmdLine->next == NULL) // There's no pipeline character
    {
        return singleCommand(pCmdLine, debug);
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
        if (debug) fprintf(stderr, "(child1>going to execute cmd: %s)\n", pCmdLine->arguments[0]);
        int err = execvp(pCmdLine->arguments[0], pCmdLine->arguments);
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
            if (debug) fprintf(stderr, "(child2>going to execute cmd: %s)\n", pCmdLine->next->arguments[0]);
            int err = execvp(pCmdLine->next->arguments[0], pCmdLine->next->arguments);
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
        if (pCmdLine->blocking)
            waitpid(ch1, NULL, 0);
        if (pCmdLine->next != NULL && pCmdLine->next->blocking)
            waitpid(ch2, NULL, 0);
    }
    if (debug) fprintf(stderr, "(parent_process>exiting...)\n");
    return pCmdLine;
}
