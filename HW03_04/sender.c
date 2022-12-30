#include "common.h"

int main(void)
{
	key_t key_sem = ftok("common", 0);
	int semid = semget(key_sem, 1, 0666 | IPC_CREAT | IPC_EXCL);
	if (semid == -1)
	{
		perror("sender semget error");
		goto label;
	}

	key_t key = ftok("common", 1);
	int shmid = shmget(key, 128, 0666 | IPC_CREAT | IPC_EXCL);
	if (shmid == -1)
	{
		perror("sender shmget error");
		goto label;
	}

	char* buf;
	buf = shmat(shmid, NULL, 0);

	printf("Input string: ");
	fgets(buf, 127, stdin);

	P_operation(semid, 0);

	printf("String received from receiver: ");
	printf("%s\n", buf);

	shmdt(buf);

label:
	semid = semget(key_sem, 1, 0666);
	if (semid != -1) semctl(semid, 0, IPC_RMID, NULL);

	shmid = shmget(key, 128, 0666);
	if (shmid != -1) shmctl(shmid, IPC_RMID, NULL);
	if (shmid == -1)
		perror("delete error: ");
}

