#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "LineParser.h"
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

char* lastCommand;

/*
    Lab 6 task 3c
    We improve on lab 5 task 1 by adding support for pipe | functionality
    task 3c involves adding functionality for multiple pipe chaining
*/

void runPipe(cmdLine* p, int** pipes, int* lPipe, int* rPipe, int debug);

int ** createPipes(int nPipes){
    int** pipes;
    pipes=(int**) calloc(nPipes, sizeof(int));

    for (int i=0; i<nPipes;i++){
        pipes[i]=(int*) calloc(2, sizeof(int));
        pipe(pipes[i]);
    }  
    return pipes;

    }
void releasePipes(int **pipes, int nPipes){
        for (int i=0; i<nPipes;i++){
            free(pipes[i]);
        
        }  
    free(pipes);
}
int *leftPipe(int **pipes, cmdLine *pCmdLine){
    if (pCmdLine->idx == 0) return NULL;
    return pipes[pCmdLine->idx -1];
}

int *rightPipe(int **pipes, cmdLine *pCmdLine){
    if (pCmdLine->next == NULL) return NULL;
    return pipes[pCmdLine->idx];
}

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
        lastCommand = malloc(2048);
        strcpy(lastCommand, userInput); // Unseen
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

void printLastCMD()
{
    lastCommand == NULL ? printf("\n") : printf("%s", lastCommand);
}

int specialForms(cmdLine* pCmdLine) 
{
    int ret = 1;
    strcmp(pCmdLine->arguments[0], "quit") == 0 ? _exit(0) :
    strcmp(pCmdLine->arguments[0], "cd") == 0 ? changeDir(pCmdLine->arguments[1]) : 
    strcmp(pCmdLine->arguments[0], "prtrls") == 0 ? printLastCMD() :
    ret--;
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

void execCMD(cmdLine* pCmdLine)
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

cmdLine* execute(cmdLine* pCmdLine, int debug)
{
    if (specialForms(pCmdLine)) return pCmdLine; 
    // Doesn't need to fork process if it was quit(stopped) / cd(done) / showprocs(done)
    if (pCmdLine->next == NULL) return singleCommand(pCmdLine, debug); // Single command

    int numOfPipes = 0;

    for(cmdLine* curr = pCmdLine; curr != NULL; curr = curr->next)
        if (curr->next == NULL) numOfPipes = curr->idx;
    
    int** pipes = createPipes(numOfPipes);
    runPipe(pCmdLine, pipes, leftPipe(pipes, pCmdLine), rightPipe(pipes, pCmdLine), debug);
    releasePipes(pipes, numOfPipes);
    return pCmdLine;
}

void runPipe(cmdLine* p, int** pipes, int* lPipe, int* rPipe, int debug)
{
    if (lPipe != NULL) close(lPipe[1]);
    if (rPipe == NULL) // last function on pipeline
    {
        if (debug) fprintf(stderr, "(parent_process>forking...)\n");
        int cid2 = fork();
        if (!cid2)
        {
            dup2(lPipe[0], 0);
            close(lPipe[0]);
            if (debug) fprintf(stderr, "(child>going to execute cmd: %s)\n", p->next->arguments[0]);
            execCMD(p);
        }
        else
        {
            if (debug) 
            {
                fprintf(stderr, "(parent_process>created process with id: %i)\n", cid2);
                fprintf(stderr, "(parent_process>closing the read end of the pipe...)\n");
            }
            close(lPipe[0]);
            if (debug)
                fprintf(stderr, "(parent_process>waiting for child process to terminate...)\n");
            if (p->blocking)
                waitpid(cid2, NULL, 0);
            if (debug) fprintf(stderr, "(parent_process>exiting...)\n");
            return;
        }
    }
    if (debug) fprintf(stderr, "(parent_process>forking...)\n");
    int cid1 = fork();
    if (!cid1)
    {
        if (lPipe != NULL)
        {
            dup2(lPipe[0], 0);
            close(lPipe[0]);
        }
        if (debug) fprintf(stderr, "(child>redirecting stdout to the write end of the pipe...)\n");
        dup2(rPipe[1], 1);
        close(rPipe[1]);
        if (debug) fprintf(stderr, "(child>going to execute cmd: %s)\n", p->arguments[0]);
        execCMD(p);
    }
    else 
    {
        if (debug) 
        {
            fprintf(stderr, "(parent_process>created process with id: %i)\n", cid1);
            fprintf(stderr, "(parent_process>closing the write end of the pipe...)\n");
        }
        if (p->blocking)
            waitpid(cid1, NULL, 0);
        runPipe(p->next, pipes, leftPipe(pipes, p->next), rightPipe(pipes, p->next), debug);
    }
}

/*
A | B | C | D
pipe[0][1]
... 
pipe[0][0]
... -> pipe[1][0]
pipe[1][1]
...
pipe[1][0]
...-> pipe[2][0]
.
.
.
.
*/