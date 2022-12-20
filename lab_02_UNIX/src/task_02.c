#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define n 2

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
            printf("\nChild process #%d. PID: %d, PPID: %d, GROUP: %d\n",i + 1, getpid(), getppid(), getpgrp());
            return 0;
        }
        else
        {
            printf("Parent process. Child ID: %d\n, PID: %d, GROUP: %d\n", child[i], getpid(), getpgrp());
        }
    }

    for (size_t i = 0; i < n; i++) 
    {
        int status;

        pid_t childpid = waitpid(child[i], &status, 0);

        if (WIFEXITED(status)) 
        {
            printf("Child process(PID %d) #%d finished with code: %d\n", childpid, i + 1, WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            printf("Child process(PID %d) #%d finished from signal with code: %d\n", childpid, i + 1, WTERMSIG(status));
        }
        else if (WIFSTOPPED(status))
        {
            printf("Child process(PID %d) #%d finished stopped with code: %d\n", childpid, i + 1, WSTOPSIG(status));
        }
    }

    return 0;
}
