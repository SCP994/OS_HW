#include "receiver.h"

void* receiver(void* v)
{
	key_t key_sem = ftok("receiver", 0);
	int semid = semget(key_sem, 0, 0666);

	P_operation(semid, 0);

	char buf[MAX];
	key_t key = ftok("receiver", 1);
	int msgid = msgget(key, 0666);
	if (msgid == -1)
	{
		perror("receiver msgget error");
		return NULL;
	}

	message* msg = (message*)malloc(sizeof(message));
	int count = 2;

	while (count)
	{
		sleep(5);
		while (count)
		{
			memset(buf, '\0', MAX);

			int rcvid = msgrcv(msgid, msg, sizeof(message), 3, IPC_NOWAIT);  // 接受类型为 3,不能填 0，即接受队列中第一个消息，否则会接受到自己发出去的消息
			//if (errno == ENOMSG)
			if (rcvid == -1)
				break;

			if (strcmp(msg->msgtext, "end1") == 0)
			{
				memset(msg->msgtext, '\0', MAX);
				strcpy(msg->msgtext, "over1");  // 没有考虑用户输入 "over1" 的情况
				msg->msgtype = 1;
				msgsnd(msgid, msg, sizeof(message), 0);
				--count;
				V_operation(semid, 1);
				continue;
			}

			if (strcmp(msg->msgtext, "end2") == 0)
			{
				memset(msg->msgtext, '\0', MAX);
				strcpy(msg->msgtext, "over2");
				msg->msgtype = 2;
				msgsnd(msgid, msg, sizeof(message), 0);
				--count;
				V_operation(semid, 1);
				continue;
			}

			printf("Data Received ");
			if (msg->senderid = 1)
				printf("from sender1: ");
			else if (msg->senderid = 2)
				printf("from sender2: ");
			printf("%s", msg->msgtext);

		}


		//V_operation(semid, 1);
	}

	if (msg) free(msg);

	return NULL;
}

