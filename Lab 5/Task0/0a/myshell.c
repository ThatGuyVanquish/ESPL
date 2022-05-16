#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "LineParser.h"
#include <linux/limits.h>

int execute(cmdLine* pCmdLine);

int main(int argc, char** argv) {
    while(1)
    {
        char cd[PATH_MAX];
        char* dir = getcwd(cd, PATH_MAX);
        printf("%s$\n", dir);
        char userInput[2048];
        fgets(userInput, 2048, stdin);
        cmdLine* command = parseCmdLines(userInput);
        execute(command);
        freeCmdLines(command);
    }
    return 0;
}

int execute(cmdLine* pCmdLine)
{
    if (strcmp(pCmdLine->arguments[0], "quit") == 0) return 0;
    int err = execvp(pCmdLine->arguments[0], pCmdLine->arguments);
    if (err < 0)
    {
        perror("Error");
        return err;
    }
    return 0;
}
