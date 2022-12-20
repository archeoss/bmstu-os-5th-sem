#ifndef __CONSTANTS__
#define __CONSTANTS__
#define SEMMSL  512
#define perms S_IRWXU | S_IRWXG | S_IRWXO
#define iter_amount 50
#define writers_amount 3
#define readers_amount 5

#define sems_amount 4
#define act_readers 0
#define act_writers 1
#define read_queue 2
#define write_queue 3

// Time [time_from, time_from + time_range)
#define time_from 1
#define time_range 5

#define SEMKEYPATH "/tmp"
#define SEMKEYID 'a'
#define SHMKEYPATH "/tmp"
#define SHMKEYID 'b'

#endif // __CONSTANTS__
