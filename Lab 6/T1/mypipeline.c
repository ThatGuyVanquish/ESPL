#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "LineParser.h"
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

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
        if (pCmdLine->inputRedirect != NULL)
        {
            close(0);
            int inputfd = open(pCmdLine->inputRedirect, O_RDONLY, 0644);
            dup(inputfd);
        }
        if (pCmdLine->outputRedirect != NULL)
        {
            close(1);
            int outputfd = open(pCmdLine->outputRedirect, O_WRONLY | O_CREAT, 0644);
            dup(outputfd);
        }
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
        return singleCommand(pCmdLine, debug);
    return pCmdLine;
}
