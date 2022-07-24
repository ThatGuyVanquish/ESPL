#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "LineParser.h"
#include <linux/limits.h>

int execute(cmdLine* pCmdLine, int debug);

int main(int argc, char** argv) {
    int debug = 0;
    for (int i = 1; i < argc; i++)
        if (strcmp(argv[i], "-d") == 0) {
            debug = 1;
            break;
        }
    while(1)
    {
        char cd[PATH_MAX];
        char* dir = getcwd(cd, PATH_MAX);
        printf("%s$\n", dir);
        char userInput[2048];
        fgets(userInput, 2048, stdin);
        cmdLine* command = parseCmdLines(userInput);
        execute(command, debug);
        freeCmdLines(command);
    }
    return 0;
}

int execute(cmdLine* pCmdLine, int debug)
{
    if (strcmp(pCmdLine->arguments[0], "quit") == 0) _exit(0);
    if (strcmp(pCmdLine->arguments[0], "cd") == 0) {
        int cderr = chdir(pCmdLine->arguments[1]);
        if (cderr < 0) {
            perror("Error");
            _exit(cderr);
        }
        return 0;
    }
    int pid = fork();
    if (pid == 0)
    {
        int err = execvp(pCmdLine->arguments[0], 
            pCmdLine->arguments);
        if (err < 0)
            {
                perror("Error");
                _exit(err);
            }
    }
    if (debug == 1)
    {
        fprintf(stderr, "PID %d\n Command: %s\n\n", pid, 
            pCmdLine->arguments[0]);
    }
    
    return 0;
}
