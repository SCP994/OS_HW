#include "common.h"

int main(void)
{
	key_t key_sem = ftok("common", 0);
	int semid = semget(key_sem, 1, 0666);

	key_t key_shm = ftok("common", 1);
	int shmid = shmget(key_shm, 128, 0666);

	char* buf = shmat(shmid, NULL, 0);

	printf("String receiverd from sender: ");
	printf("%s", buf);

	strcpy(buf, "over");
	shmdt(buf);

	V_operation(semid, 0);
	sleep(1);
}

