#include "sender.h"

void* sender1(void* v)
{
	char buf[MAX];
	key_t key_sem = ftok("msg", 0), key_msg = ftok("msg", 1);
	int semid = semget(key_sem, 2, 0666);
	int msgid = msgget(key_msg, 0666 | IPC_CREAT | IPC_EXCL);
	if (msgid == -1)
	{
		perror("sender1 msgget error");
		return NULL;
	}
	V_operation(semid, 0);
	V_operation(semid, 0);

	message msg;
	while (true)
	{
		P_operation(semid, 1);
		printf("Sender1 input the date: ");
		fgets(buf, MAX, stdin);
		V_operation(semid, 1);
		buf[strlen(buf) - 1] = '\0';

		msg.msgtype = RCV;
		msg.senderid = SND1;
		if (strcmp(buf, "exit") == 0)
		{
			strcpy(msg.msgtext, "end1");
			msgsnd(msgid, &msg, sizeof(message), 0);
			break;
		}
		strcpy(msg.msgtext, buf);
		msgsnd(msgid, &msg, sizeof(message), 0);
	}
	msgrcv(msgid, &msg, sizeof(message), SND1, 0);
	printf("From receiver: %s\n", msg.msgtext);
	return NULL;
}

void* sender2(void* v)
{
	char buf[MAX];
	key_t key_sem = ftok("msg", 0), key_msg = ftok("msg", 1);
	int semid = semget(key_sem, 2, 0666);
	P_operation(semid, 0);
	int msgid = msgget(key_msg, 0666);
	if (msgid == -1)
	{
		perror("sender2 msgget error");
		return NULL;
	}

	message msg;
	while (true)
	{
		P_operation(semid, 1);
		printf("Sender2 input the data: ");
		fgets(buf, MAX, stdin);
		V_operation(semid, 1);
		buf[strlen(buf) - 1] = '\0';

		msg.msgtype = RCV;
		msg.senderid = SND2;
		if (strcmp(buf, "exit") == 0)
		{
			strcpy(msg.msgtext, "end2");
			msgsnd(msgid, &msg, sizeof(message), 0);
			break;
		}
		strcpy(msg.msgtext, buf);
		msgsnd(msgid, &msg, sizeof(message), 0);
	}
	msgrcv(msgid, &msg, sizeof(message), SND2, 0);
	printf("From receiver: %s\n", msg.msgtext);
	return NULL;
}

