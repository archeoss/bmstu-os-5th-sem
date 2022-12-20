#include "producer.h"

struct sembuf prod_lock[] = { 
    { buf_empty, -1, 0 }, 
    { bin_sem, -1, 0 } 
};

struct sembuf prod_release[] = { 
    { buf_full, 1, 0 }, 
    { bin_sem, 1, 0 } 
};

int producer_run(int fd, const int sid, const int prodid, char* alphabet_ptr) {
    char* addr = shmat(fd, NULL, 0);
    srand(time(NULL) + prodid);

    for (size_t i = 0; i < iter_cnt; i++) {
        int sleep_time = rand() % 4 + 1;
        sleep(sleep_time);
        if (-1 == semop(sid, prod_lock, 2)) {
            perror("Producer lock error\n");
            exit(1);
        }

        char *producer_ptr = (char*)(addr);

        char symb = (*alphabet_ptr)++;
        *(producer_ptr) = symb;
        (producer_ptr)++;

        fprintf(stdout, "Producer %d write: %c, sleep: %d\n", prodid + 1, symb, sleep_time);

        if (-1 == semop(sid, prod_release, 2)) {
            perror("Producer release error\n");
            exit(1);
        }

    }

    return 0;
}
