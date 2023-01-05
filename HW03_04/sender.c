#include "common.h"

int main(void)
{
	key_t key_sem = ftok("common", 0);
	int semid = semget(key_sem, 1, 0666 | IPC_CREAT | IPC_EXCL);
	if (semid == -1)
	{
		perror("sender semget error: ");
		goto ret;
	}

	key_t key_shm = ftok("common", 1);
	int shmid = shmget(key_shm, 128, 0666 | IPC_CREAT | IPC_EXCL);
	if (shmid == -1)
	{
		perror("sender shmget error: ");
		goto ret;
	}

	char* buf = shmat(shmid, NULL, 0);

	printf("Input string: ");
	fgets(buf, 128, stdin);
	P_operation(semid, 0);

	printf("String received from receiver: ");
	printf("%s\n", buf);

	int error = shmdt(buf);
	if (error == -1)
	{
		perror("sender shmdt error: ");
		goto ret;
	}

ret:
	semid = semget(key_sem, 1, 0666);
	if (semid != -1) semctl(semid, 0, IPC_RMID, NULL);

	shmid = shmget(key_shm, 128, 0666);
	if (shmid != -1) shmctl(shmid, IPC_RMID, NULL);
}

