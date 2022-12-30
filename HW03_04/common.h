#ifndef _COMMON_H                                                                                                                                 
#define _COMMON_H

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>  // sleep
#include <sys/msg.h>  // key_t
#include <string.h>
#include <stdbool.h>
#include <sys/sem.h>
#include <sys/shm.h>
#define MAX 128

union semun
{
    int val;
    struct semid_ds* buf;
    unsigned short* array;
    struct seminfo* __buf;
};

void P_operation(int semid, int num);

void V_operation(int semid, int num);

#endif

