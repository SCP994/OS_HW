#pragma once
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/sem.h>

#define MAX 128
#define SND1 1
#define SND2 2
#define RCV 3

typedef struct msgbuf
{
	long msgtype;
	long senderid;
	char msgtext[MAX];
} message;

void P_operation(int semid, int num);

void V_operation(int semid, int num);

