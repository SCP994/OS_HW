#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>  // sleep
#include <sys/msg.h>  // key_t
#include <string.h>
#include <stdbool.h>
#include <errno.h>
//#include <linux/ipc.h>
#include <sys/sem.h>
#define MAX 128

typedef struct msgbuf
{
	long msgtype;  // 3: 发给 receiver; 1: 发给 sender1; 2: 发给 sender2
	long senderid, receiverid;  // 只用到 senderid
	char msgtext[MAX];
} message;

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

