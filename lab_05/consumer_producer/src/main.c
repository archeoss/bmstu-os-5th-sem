#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h> 
#include <unistd.h>
#include <wait.h>

#include "consumer.h"
#include "producer.h"
#include "constants.h"

int main(void) {
    setbuf(stdout, NULL);
    char *addr1, *addr2, *addr3;
    char *letter_ptr;
    key_t shmkey = ftok(SHMKEYPATH, SHMKEYID); 
    if (-1 == shmkey) {
        perror("Failed to create shared memory!");
        exit(1);
    }

    key_t semkey = ftok(SEMKEYPATH, SEMKEYID); 
    if (-1 == semkey) {
        perror("Failed to create shared memory!");
        exit(1);
    }

    int fd = shmget(shmkey, 256, perms | IPC_CREAT);
    if (-1 == fd) 
    {
        perror("shmget failed");
        exit(1);
    }

    char *addr = shmat(fd, 0, 0);
    if ((void *)-1 == addr) {
        perror("Failed to shmat!");
        exit(1);
    }
    addr1 = addr + sizeof(char*);
    addr2 = addr1 + sizeof(char*);
    addr3 = addr2 + sizeof(char);

    char **prod_ptr = (char **)addr;
    char **cons_ptr = (char **)addr1;
    letter_ptr = addr2;

    *letter_ptr = 'a';
    *prod_ptr = addr3;
    *cons_ptr = addr3;

    int sid = semget(semkey, sems_amount, IPC_CREAT | perms);
    if (-1 == sid) {
        perror("Can't create array of semaphores!");
        exit(1);
    }
    if  (
        -1 == semctl(sid, bin_sem, SETVAL, 1) ||
        -1 == semctl(sid, buf_empty, SETVAL, n) ||
        -1 == semctl(sid, buf_full, SETVAL, 0) 
    ) {
        perror("semctl error\n");
        exit(1);
    }
    int child_pid;
    
    for (int i = 0; i < consumers_amount; i++) {
        switch ((child_pid = fork())) {
        case -1:
            perror("Failed to fork for consumer!");
            exit(1);
        case 0:
            consumer_run(fd, sid, i);
            return 0;
        }
    }
    for (size_t i = 0; i < producers_amount; i++) {
        switch ((child_pid = fork())) {
        case -1:
            perror("Failed to fork for producer!");
            exit(1);
        case 0:
            // producer_run(*((char **)producer_ptr), sid, i, alphabet_ptr);
            producer_run(fd, sid, i, letter_ptr);
            
            return 0;
        }
    }
    producer_run(fd, sid, producers_amount - 1, letter_ptr);
    
    for (size_t i = 0; i < consumers_amount + producers_amount; i++) {
        int status;
        if ((child_pid = wait(&status)) == -1) {
            perror("Something wrong with children waiting!");
            exit(1);
        }
        if (!WIFEXITED(status))
            fprintf(stderr, "Child process %d terminated abnormally\n", child_pid);
    }
    
    if (-1 == shmdt(addr))
    {
        perror("shmdt error");
        exit(1);
    }

    if (-1 == shmctl(fd, IPC_RMID, NULL)) 
    {
        perror("shmctl with command IPC_RMID failed");
        exit(1);
    }

    if (-1 == semctl(sid, 0, IPC_RMID))
    {
        perror("semctl with command IPC_RMID failed");
        exit(1);
    }

    return 0;
}