#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "LineParser.h"
#include <linux/limits.h>
#include <sys/wait.h>

typedef struct process{
    cmdLine* cmd; /* the parsed command line*/
    pid_t pid; /* the process id that is running the command*/
    int status; /* status of the process; RUNNING/SUSPENDED/TERMINATED*/
    struct process* next; /* next process in chain*/
} process;

#define TERMINATED -1
#define RUNNING 1
#define SUSPENDED 0

process* plist = NULL;

cmdLine* execute(cmdLine* pCmdLine, int debug);
void addProcess(process** process_list, cmdLine* cmd, pid_t pid);
process* nextProcess(process* p, cmdLine* cmd, pid_t pid);
void printProcessList(process** process_list);

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
        printf("%s $ ", dir);
        char userInput[2048];
        fgets(userInput, 2048, stdin);
        cmdLine* cmnd = parseCmdLines(userInput);
        execute(cmnd, debug);
        freeCmdLines(cmnd);
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
    strcmp(pCmdLine->arguments[0], "cd") == 0 ? changeDir(pCmdLine->arguments[1]) :
    strcmp(pCmdLine->arguments[0], "showprocs") == 0 ? printProcessList(&plist) : ret--;
    return ret;
}

cmdLine* execute(cmdLine* pCmdLine, int debug)
{
    if (specialForms(pCmdLine)) return pCmdLine; // Doesn't need to fork process if it was quit(stopped) / cd(done) / showprocs(done)

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

    pid != -1 ? addProcess(&plist, pCmdLine, pid) : 0;

    debug == 1 ? fprintf(stderr, "PID %d\n Command: %s\n\n", pid, pCmdLine->arguments[0]) : 0;

    if (pCmdLine->blocking)
        waitpid(pid, NULL, 0);


    return pCmdLine;
}

void addProcess(process** process_list, cmdLine* cmd, pid_t pid) 
{
    *process_list = nextProcess(*process_list, cmd, pid);
}

process* nextProcess(process* p, cmdLine* cmd, pid_t pid) 
{
    if (p == NULL)
    {
        p = malloc(sizeof(struct process));
        p->cmd = cmd;
        p->pid = pid;
        p->status = RUNNING;
        p->next = NULL;
    }
    else
        p->next = nextProcess(p->next, cmd, pid);
    return p;
}

process* createProcess(process* p, cmdLine* cmd, pid_t pid)
{
    p = malloc(sizeof(struct process));
    p->cmd = cmd;
    p->pid = pid;
    p->status = RUNNING;
    p->next = NULL;
    return p;
}

void printProcessList(process** process_list)
{
    process* current = *process_list;
    while (current != NULL)
    {
        char* status = current->status == TERMINATED ? "TERMINATED" : 
                       current->status == RUNNING ? "RUNNING" : 
                       "SUSPENDED";
        printf("%i %s %s\n", current->pid, current->cmd->arguments[0], status);
        current = current->next;
    }
}