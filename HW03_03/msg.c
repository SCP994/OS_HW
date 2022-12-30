#include "sender.h"
#include "receiver.h"

int main(void)
{
	key_t key_sem = ftok("receiver", 0);
	int semid = semget(key_sem, 2, 0666 | IPC_CREAT | IPC_EXCL);  // 创建 IPC 信号量
	if (semid == -1)
		perror("semget error");
	else
	{
		V_operation(semid, 1);  // 初始值为 1

		pthread_t tid1, tid2, tid3;
		pthread_create(&tid1, NULL, sender1, NULL);
		pthread_create(&tid2, NULL, sender2, NULL);
		pthread_create(&tid3, NULL, receiver, NULL);
		pthread_join(tid1, NULL);  // 等待三个子线程结束
		pthread_join(tid2, NULL);
		pthread_join(tid3, NULL);
	}

	key_t key = ftok("receiver", 1);
	int msgid = msgget(key, 0666);
	if (msgid != -1) msgctl(msgid, IPC_RMID, NULL);  // 删除消息队列

	semid = semget(key_sem, 1, 0666);
	if (semid != -1) semctl(semid, 0, IPC_RMID, NULL);  // 删除 IPC 信号量
}

