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
void freeProcessList(process* process_list);
void updateProcessList(process **process_list);
void deleteTerminated(process** process_list);
void freeProcess(process* process);
void updateProcessStatus(process* process_list, int pid, int status);
void sendToSleep(int time, int pid);
void stopProc(int pid);
void quit();

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
    strcmp(pCmdLine->arguments[0], "quit") == 0 ? quit() :
    strcmp(pCmdLine->arguments[0], "cd") == 0 ? changeDir(pCmdLine->arguments[1]) :
    strcmp(pCmdLine->arguments[0], "showprocs") == 0 ? printProcessList(&plist) :
    strcmp(pCmdLine->arguments[0], "nap") == 0 ? sendToSleep(atoi(pCmdLine->arguments[1]), atoi(pCmdLine->arguments[2])) :
    strcmp(pCmdLine->arguments[0], "stop") == 0 ? stopProc(atoi(pCmdLine->arguments[1])) :
    ret--;
    return ret;
}

void quit()
{
    process* curr = plist;
    for(; curr != NULL; curr = curr->next)
    {
        curr->status == TERMINATED ? kill(curr->pid, SIGINT) : 0;
    }
    updateProcessList(&plist);
    _exit(0);
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

void printProcessList(process** process_list)
{
    updateProcessList(process_list);
    process* current = process_list[0];
    while (current != NULL)
    {
        char* status = current->status == TERMINATED ? "TERMINATED" : 
                       current->status == RUNNING ? "RUNNING" : 
                       "SUSPENDED";
        printf("%i %s %s\n", current->pid, current->cmd->arguments[0], status);
        current = current->next;
    }
    deleteTerminated(process_list);
}

void deleteTerminated(process** process_list)
{
    if (*process_list == NULL) return;
    process* head = *process_list;
    for (; head != NULL && head->status == TERMINATED; head = head->next) {};
    if (head == NULL) 
    {
        *process_list = NULL;
        return;
    }
    process* current = head;
    process* next = current->next;
    for(;next != NULL; next = next->next)
    {
        if (next->status != TERMINATED) 
        {
            current->next = next;
            current = next;
        }
        else 
        {
            current->next = NULL;
            freeProcess(next);
        }
    }
    process_list[0] = head;
}

void freeProcess(process* process)
{
    freeCmdLines(process->cmd);
    free(process);
}

void freeProcessList(process* process_list)
{
    if (process_list == NULL) return;
    freeCmdLines(process_list->cmd);
    freeProcessList(process_list->next);
    free(process_list);
}

void updateProcessList(process **process_list)
{
    process* current = *process_list;
    for(; current != NULL; current = current->next)
    {
        int status;
        int procid = waitpid(current->pid, &status, WNOHANG | WUNTRACED | WCONTINUED);
        if (procid != 0) 
        {
            WIFEXITED(status) || WIFSIGNALED(status) ? updateProcessStatus(*process_list, current->pid, TERMINATED) :
            WIFSTOPPED(status) ? updateProcessStatus(*process_list, current->pid, SUSPENDED) :
            updateProcessStatus(*process_list, current->pid, RUNNING);
        }
    }
}

void updateProcessStatus(process* process_list, int pid, int status)
{
    process* current = process_list;
    for (; current != NULL; current = current->next)
    {
        if (current->pid == pid)
        {
            current->status = status;
            break;
        }
    }
}

void sendToSleep(int time, int pid)
{
    int child = fork();
    if (child == 0)
    {
        kill(pid, SIGTSTP);
        sleep(time);
        kill(pid, SIGCONT);
    }
}

void stopProc(int pid)
{
    kill(pid, SIGINT);
}