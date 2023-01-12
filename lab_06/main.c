#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define readers_cnt 5
#define writers_cnt 3

#define witer_cnt 8
#define riter_cnt 7

#define write_timeout 300
#define read_timeout 300
#define diff 4000

#define create_mutex_failed 1
#define create_event_failed 2
#define create_thread_failed 3

HANDLE mutex;
HANDLE can_read;
HANDLE can_write;

LONG active_readers = 0;
LONG waiting_writers = 0;
LONG waiting_readers = 0;

int active_writer = 0;
int value = 0;

void start_read(void) {
    InterlockedIncrement(&waiting_readers);

    if (active_writer || (WaitForSingleObject(can_write, 0) == WAIT_OBJECT_0 && waiting_writers))
    {
        WaitForSingleObject(can_read, INFINITE);
    }

    WaitForSingleObject(mutex, INFINITE);
    InterlockedDecrement(&waiting_readers);
    InterlockedIncrement(&active_readers);

    SetEvent(can_read);
    ReleaseMutex(mutex);
}

void stop_read(void) {
    InterlockedDecrement(&active_readers);
    if (active_readers == 0) {
        ResetEvent(can_read);
        SetEvent(can_write);
    }
}

DWORD WINAPI run_reader(CONST LPVOID lpParams) {
srand(time(NULL) + writers_cnt);
int sleep_time;

for (size_t i = 0; i < riter_cnt; i++) {
sleep_time = read_timeout + rand() % diff;
Sleep(sleep_time);
start_read();
printf("Reader %d read:  %d\n", (int)lpParams;, value);
stop_read();
}

return 0;
}

void start_write(void) {
    InterlockedIncrement(&waiting_writers);

    if (active_writer || active_readers > 0) {
        WaitForSingleObject(can_write, INFINITE);
    }

    InterlockedDecrement(&waiting_writers);
    active_writer = 1;
}

void stop_write(void) {
    active_writer = 0;

    if (waiting_readers) {
        SetEvent(can_read);
    } else {
        SetEvent(can_write);
    }
}

DWORD WINAPI run_writer(CONST LPVOID lpParams) {
srand(time(NULL)+ readers_cnt);
int sleep_time;

for (int i = 0; i < witer_cnt; ++i) {
sleep_time = write_timeout + rand() % diff;
Sleep(sleep_time);
start_write();

printf("Writer %d write: %d\n", (int)lpParams;, ++value);
stop_write();
}

return 0;
}

int main(void) {
    HANDLE writers_threads[writers_cnt];
    HANDLE readers_threads[readers_cnt];

    if (!(mutex = CreateMutex(NULL, 0, NULL))) {
        perror("Failed call of CreateMutex");
        exit(1);
    }

    if (!(can_read = CreateEvent(NULL, 0, 0, NULL)) || !(can_write = CreateEvent(NULL, 0, 0, NULL))) {
        perror("Failed call of CreateEvent");
        exit(1);
    }

    for (int i = 0; i < readers_cnt; ++i) {
        if (!(readers_threads[i] = CreateThread(NULL, 0, run_reader, (LPVOID)i, 0, NULL))) {
            perror("Failed call of CreateThread");
            exit(1);
        }
    }

    for (int i = 0; i < writers_cnt; i++) {
        if (!(writers_threads[i] = CreateThread(NULL, 0, run_writer, (LPVOID)i, 0, NULL))) {
            perror("Failed call of CreateThread");
            exit(1);
        }
    }

    WaitForMultipleObjects(readers_cnt, readers_threads, 1, INFINITE);
    WaitForMultipleObjects(writers_cnt, writers_threads, 1, INFINITE);

    CloseHandle(mutex);
    CloseHandle(can_read);
    CloseHandle(can_write);

    return 0;
}
