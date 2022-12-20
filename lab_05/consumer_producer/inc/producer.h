#ifndef _PRODUCER_H_
#define _PRODUCER_H_

#include "constants.h"
#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <time.h>
#include <wait.h>

int producer_run(int, const int sid, const int prodid, char* alphabet_ptr);

#endif