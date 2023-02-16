#ifndef _CONSTANTS_
#define _CONSTANTS_

#define perms S_IRWXU | S_IRWXG | S_IRWXO
#define key IPC_PRIVATE
#define n 64
#define iter_cnt 8
#define consumers_amount 2
#define producers_amount 2

#define sem_size 2

#define sems_amount 3
#define bin_sem 0
#define buf_full 1
#define buf_empty 2

#define buf_size 256

#define SEMKEYPATH "/dev/null"
#define SEMKEYID 1
#define SHMKEYPATH "/dev/null"
#define SHMKEYID 1

#endif // _CONSTANTS_