#include "receiver.h"

void* receiver(void* v)
{
	char buf[MAX];
	key_t key_sem = ftok("msg", 0), key_msg = ftok("msg", 1);
	int semid = semget(key_sem, 2, 0666);
	P_operation(semid, 0);
	int msgid = msgget(key_msg, 0666);
	if (msgid == -1)
	{
		perror("receiver msgget error");
		return NULL;
	}

	int count = 2;
	message msg;
	while (count)
	{
		sleep(5);
		P_operation(semid, 1);
		while (count)
		{
			int rcvid = msgrcv(msgid, &msg, sizeof(message), RCV, IPC_NOWAIT);
			if (rcvid == -1)
				break;
			if (strcmp(msg.msgtext, "end1") == 0 && msg.senderid == SND1)
			{
				strcpy(msg.msgtext, "over1");
				msg.msgtype = SND1;
				msg.senderid = RCV;
				msgsnd(msgid, &msg, sizeof(message), 0);
				--count;
				continue;
			}
			if (strcmp(msg.msgtext, "end2") == 0 && msg.senderid == SND2)
			{
				strcpy(msg.msgtext, "over2");
				msg.msgtype = SND2;
				msg.senderid = RCV;
				msgsnd(msgid, &msg, sizeof(message), 0);
				--count;
				continue;
			}
			if (msg.senderid == SND1)
				printf("Data received from sender1: %s\n", msg.msgtext);
			else if (msg.senderid == SND2)
				printf("Data received from sender2: %s\n", msg.msgtext);
		}
		V_operation(semid, 1);
	}
	return NULL;
}

