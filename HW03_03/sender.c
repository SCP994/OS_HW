#include "sender.h"

void* sender1(void* v)
{
	key_t key_sem = ftok("receiver", 0);
	int semid = semget(key_sem, 2, 0666);

	V_operation(semid, 0);
	V_operation(semid, 0);

	char buf[MAX];

	key_t key = ftok("receiver", 1);
	int msgid = msgget(key, 0666 | IPC_CREAT | IPC_EXCL);  // 创建消息队列，已经存在则报错
	if (msgid == -1)
	{
		perror("sender1 msgget error");
		return NULL;
	}

	while (true)
	{
		P_operation(semid, 1);
		memset(buf, '\0', MAX);
		printf("Sender1 input the date: ");
		fgets(buf, MAX - 1, stdin);

		if (strcmp(buf, "exit\n") == 0)
		{
			V_operation(semid, 1);
			break;
		}

		message* msg = (message*)malloc(sizeof(message));
		msg->msgtype = 3;
		msg->senderid = 1;
		strcpy(msg->msgtext, buf);
		msgsnd(msgid, msg, sizeof(message), 0);
		free(msg);

		V_operation(semid, 1);
	}

	message* msg = (message*)malloc(sizeof(message));
	msg->msgtype = 3;
	strcpy(msg->msgtext, "end1");
	msgsnd(msgid, msg, sizeof(message), 0);
	free(msg);

	//P_operation(semid, 1);
	msg = (message*)malloc(sizeof(message));
	msgrcv(msgid, msg, sizeof(message), 1, 0);  // 接收类型为 1
	printf("From receiver: %s\n", msg->msgtext);
	free(msg);
	//V_operation(semid, 1);

	return NULL;
}

void* sender2(void* v)
{
	key_t key_sem = ftok("receiver", 0);
	int semid = semget(key_sem, 2, 0666);

	P_operation(semid, 0);

	char buf[MAX];
	key_t key = ftok("receiver", 1);
	int msgid = msgget(key, 0666);  // 使用创建好的消息队列
	if (msgid == -1)
	{
		perror("sender2 msgget error");
		return NULL;
	}

	while (true)
	{
		P_operation(semid, 1);
		memset(buf, '\0', MAX);
		printf("Sender2 input the data: ");
		fgets(buf, MAX, stdin);

		if (strcmp(buf, "exit\n") == 0)
		{
			V_operation(semid, 1);
			break;
		}

		message* msg = (message*)malloc(sizeof(message));
		msg->msgtype = 3;
		msg->senderid = 2;
		strcpy(msg->msgtext, buf);
		msgsnd(msgid, msg, sizeof(message), 0);
		free(msg);

		V_operation(semid, 1);
	}

	message* msg = (message*)malloc(sizeof(message));
	msg->msgtype = 3;
	strcpy(msg->msgtext, "end2");
	msgsnd(msgid, msg, sizeof(message), 0);
	free(msg);

	//P_operation(semid, 1);
	msg = (message*)malloc(sizeof(message));
	msgrcv(msgid, msg, sizeof(message), 2, 0);  // 接受类型为 2
	printf("From receiver: %s\n", msg->msgtext);
	free(msg);
	//V_operation(semid, 1);

	return NULL;
}

