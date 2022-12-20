#ifndef _CONSUMER_H_
#define _CONSUMER_H_

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

int consumer_run(int, const int, const int);

#endif