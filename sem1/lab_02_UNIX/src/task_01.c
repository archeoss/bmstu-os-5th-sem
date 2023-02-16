#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#define n 2
#define sleep_t 2

int main()
{
    pid_t child[n];

    for (size_t i = 0; i < n; i++)
    {

        if ((child[i] = fork()) == -1)
        {
            perror("Can't fork");
            exit(1);
        }
        else if (child[i] == 0)
        {
            printf("\nBefore sleep:\nChild process #%d. PID: %d, PPID: %d, GROUP: %d\n",i + 1, getpid(), getppid(), getpgrp());
            sleep(sleep_t);
            printf("\nAfter sleep:\nChild process #%d. PID: %d, PPID: %d, GROUP: %d\n",i + 1, getpid(), getppid(), getpgrp());
            return 0;
        }
        else
        {
            printf("Parent process. Child ID: %d\n, PID: %d, GROUP: %d\n", child[i], getpid(), getpgrp());
        }
    }
    
    return 0;
}