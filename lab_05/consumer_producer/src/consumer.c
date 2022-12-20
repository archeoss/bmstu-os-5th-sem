#include "consumer.h"

struct sembuf cons_lock[] = { 
    { buf_full, -1, 0 }, 
    { bin_sem, -1, 0 } 
};

struct sembuf cons_release[] = { 
    { buf_empty, 1, 0 }, 
    { bin_sem, 1, 0 } 
};

int consumer_run(int fd, const int sid, const int consid) {
    char* addr = shmat(fd, NULL, 0);
    srand(time(NULL) + consid);

    for (int i = 0; i < iter_cnt; i++) 
    {
        int sleep_time = rand() % 4 + 1;
        sleep(sleep_time);
        if (-1 == semop(sid, cons_lock, 2)) {
            perror("Producer lock error\n");
            exit(1);
        }

        char *consumer_ptr = (char*)(addr);
        char sym = (*consumer_ptr)++;

        fprintf(stdout, "\t\t\t\t\t\t");
        fprintf(stdout, "Consumer %d: %c, sleep=%d\n", consid + 1, sym, sleep_time);

        if (-1 == semop(sid, cons_release, 2)) {
            perror("Producer release error\n");
            exit(1);
        }
    }
    if (shmdt((void *) addr) == -1)
    {
        perror("shmdt error\n");
        exit(1);
    }

    return 0;
}
