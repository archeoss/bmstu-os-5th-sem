#include "io_objects.h"

struct sembuf READER_LOCK[] = { 
    { read_queue, 1, 0 }, 
    { act_writers, 0, 0 }, 
    { write_queue, 0, 0 },
    { act_readers, 1, 0 }, 
    { read_queue, -1, 0 }, 
};

struct sembuf READER_RELEASE[] = { 
    { act_readers, -1, 0 }, 
};

struct sembuf WRITER_LOCK[] = {  
    { write_queue, 1, 0 }, 
    { act_readers, 0, 0 }, 
    { act_writers, 0, 0 }, 
    { act_writers, 1, 0 }, 
    { write_queue, -1, 0 },
};

struct sembuf WRITER_RELEASE[] = { 
    { act_writers, -1, 0 }, 
}; 


int start_read(int sid) {
    return semop(sid, READER_LOCK, 5) != -1;
}

int stop_read(int sid) {
    return semop(sid, READER_RELEASE, 1) != -1;
}

int run_reader(int fd, const int sid, const int reader_id)
{
    int* shared_addr = shmat(fd, NULL, 0);

    srand(time(NULL) + reader_id);

    int sleep_time;
    for (int i = 0; i < iter_amount; i++) {
        sleep_time = rand() % time_range + time_from;
        sleep(sleep_time);
        if (!start_read(sid)) {
            perror("Something went wrong with start_read!");
            exit(1);
        }

        int val = *shared_addr;
        printf("Reader #%d read:  %5d (slept %ds)\n", reader_id, val, sleep_time);

        if (!stop_read(sid)) {
            perror("Something went wrong with stop_read!");
            exit(1);
        }
    }
    if (shmdt((void *) shared_addr) == -1)
    {
        perror("shmdt error\n");
        exit(1);
    }

    return 0;
}

int start_write(int sid) {
    return semop(sid, WRITER_LOCK, 5) != -1;
}

int stop_write(int sid) {
    return semop(sid, WRITER_RELEASE, 1) != -1;
}

int run_writer(int fd, const int sid, const int writer_id)
{
    int* shared_addr = shmat(fd, NULL, 0);

    srand(time(NULL) + writer_id + readers_amount);

    int sleep_time;
    for (int i = 0; i < iter_amount; i++) {
        sleep_time = rand() % time_range + time_from;
        sleep(sleep_time);

        if (!start_write(sid)) {
            perror("Something went wrong with start_write!");
            exit(1);
        }

        int val = ++(*shared_addr);
        printf("Writer #%d write: %5d (slept %ds)\n", writer_id, val, sleep_time);

        if (!stop_write(sid)) {
            perror("Something went wrong with stop_write!");
            exit(1);
        }
    }
    if (shmdt((void *) shared_addr) == -1)
    {
        perror("shmdt error\n");
        exit(1);
    }

    return 0;
}
