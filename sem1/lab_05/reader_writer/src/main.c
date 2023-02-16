#include <wait.h>
#include <sys/stat.h>
#include <errno.h>
#include "constants.h"
#include "io_objects.h"

int main(void) {
    setbuf(stdout, NULL);
    
    key_t shmkey = ftok(SHMKEYPATH, SHMKEYID); 
    if (shmkey == -1) {
        perror("Failed to create shared memory!");
        exit(1);
    }

    key_t semkey = ftok(SEMKEYPATH, SEMKEYID); 
    if (semkey == -1) {
        perror("Failed to create shared memory!");
        exit(1);
    }

    int fd = shmget(shmkey, sizeof(short), IPC_CREAT | perms);
    if (-1 == fd) {
        perror("Failed to create shared memory!");
        exit(1);
    }

    int sid = semget(semkey, sems_amount, perms | IPC_CREAT);
    if (-1 == sid) {
        perror("Can't create array of semaphores!");
        exit(1);
    }

    semctl(sid, act_readers, SETVAL, 0);
    semctl(sid, act_writers, SETVAL, 0);
    semctl(sid, write_queue, SETVAL, 0);
    semctl(sid, read_queue, SETVAL, 0);

    int child_pid;
    for (size_t i = 0; i < readers_amount; i++) {
        switch ((child_pid = fork())) {
        case -1:
            perror("Failed to fork for producer!");
            exit(1);
        case 0:
            run_reader(fd, sid, i);
            return 0;
        }
    }

    for (int i = 0; i < writers_amount; i++) {
        switch ((child_pid = fork())) {
        case -1:
            perror("Failed to fork for consumer!");
            exit(1);
        case 0:
            run_writer(fd, sid, i);
            return 0;
        }
    }

    for (size_t i = 0; i < writers_amount + readers_amount; i++) {
        int status;
        if (-1 == wait(&status)) {
            perror("Something wrong with children waiting!");
            exit(1);
        }
        if (!WIFEXITED(status))
            printf("One of children terminated abnormally\n");
    }
    
    if ( -1 == shmctl(fd, IPC_RMID, NULL) || 
         -1 == semctl(sid, IPC_RMID, 0)) {
        perror("Something went wrong during shutdown!");
        exit(1);
    }

    return 0;
}
