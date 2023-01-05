#pragma once
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <string.h>
#include <stdbool.h>
#include <sys/sem.h>
#include <sys/shm.h>

void P_operation(int semid, int num);

void V_operation(int semid, int num);

