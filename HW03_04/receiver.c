#include "common.h"

int main(void)
{
	key_t key_sem = ftok("common", 0);
	int semid = semget(key_sem, 1, 0666);

	key_t key = ftok("common", 1);
	int shmid = shmget(key, 128, 0666);

	char* buf;
	buf = shmat(shmid, NULL, 0);

	printf("String receiverd from sender: ");
	printf("%s", buf);

	memset(buf, '\0', 128);
	strcpy(buf, "over");

	shmdt(buf);

	V_operation(semid, 0);
}

