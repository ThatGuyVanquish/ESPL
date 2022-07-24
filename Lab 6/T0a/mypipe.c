#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


/*
    Lab 6 task 0, improving on lab 5 by adding
    pipe to the shell
    also added forking to create child
    processes
*/
int main(int argc, char** argv)
{
    char c;
    int p[2];

    pipe(p);
    int pid = fork();

    if (pid > 0)
    {
        close(p[1]);
        printf("Reading from parent\n");
        while (read(p[0], &c, 1) != 0)
            putchar(c);    
        close(p[0]);
        wait(NULL);
    }
    else 
    {
        close(p[0]);
        write(p[1], "hello\n", sizeof("hello\n"));
        close(p[1]);
    }
    return 0;
}