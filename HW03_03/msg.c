#include "sender.h"
#include "receiver.h"

int main()
{
	key_t key_sem = ftok("msg", 0), key_msg = ftok("msg", 1);
	int semid = semget(key_sem, 2, 0666 | IPC_CREAT | IPC_EXCL);
	if (semid == -1)
		perror("semget error");
	else
	{
		V_operation(semid, 1);

		pthread_t tid1, tid2, tid3;
		pthread_create(&tid1, NULL, sender1, NULL);
		pthread_create(&tid2, NULL, sender2, NULL);
		pthread_create(&tid3, NULL, receiver, NULL);
		pthread_join(tid1, NULL);
		pthread_join(tid2, NULL);
		pthread_join(tid3, NULL);
	}

	int msgid = msgget(key_msg, 0666);
	if (msgid != -1) msgctl(msgid, IPC_RMID, NULL);

	semid = semget(key_sem, 1, 0666);
	if (semid != -1) semctl(semid, 0, IPC_RMID, NULL);

	return 0;
}

