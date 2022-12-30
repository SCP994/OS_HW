#include "common.h"

void P_operation(int semid, int num)
{
    struct sembuf sops;
    sops.sem_num = num;
    sops.sem_op = -1;
    sops.sem_flg = SEM_UNDO;
    
    semop(semid, &sops, 1);
}

void V_operation(int semid, int num)
{
    struct sembuf sops;
    sops.sem_num = num;
    sops.sem_op = 1;
    sops.sem_flg = SEM_UNDO;

    semop(semid, &sops, 1);                                                                                                                       
}

