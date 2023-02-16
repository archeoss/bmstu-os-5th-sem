#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define n 2
#define bsize 256

char *MESSAGES[n][bsize] = { "ыыыыыыыыыыыыы\n", "ллллллллллллллллллллллллллллллллллллллллллллллллллллллл\n" };

int main()
{
    pid_t child[n];
    int fd[n];
    
    if (pipe(fd) == -1)
    {
        perror("Can't pipe");
        exit(1);
    }

    for (size_t i = 0; i < n; i++)
    {
        if ((child[i] = fork()) == -1)
        {
            perror("Can't fork");
            exit(1);
        }
        else if (child[i] == 0)
        {
            close(fd[0]);
            write(fd[1], (*MESSAGES)[i], strlen((*MESSAGES)[i]));
            printf("\nMessage #%d (pid = %d) sent to parent\n", i + 1, getpid());

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
    close(fd[1]);
    read(fd[0], *MESSAGES, sizeof(MESSAGES));

    printf("Received messages:\n%s", *MESSAGES);

    return 0;
}
