#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

union Semun
{
    int val;               /* Value for SETVAL */
    struct semid_ds *buf;  /* Buffer for IPC_STAT, IPC_SET */
    unsigned short *array; /* Array for GETALL, SETALL */
    struct seminfo *__buf; /* Buffer for IPC_INFO (Linux-specific) */
};

void down(int semid)
{
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = -1;
    op.sem_flg = !IPC_NOWAIT;
    if (semop(semid, &op, 1) == -1)
    {
        printf("Error in down\n");
        exit(-1);
    }
}

void up(int semid)
{
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = 1;
    op.sem_flg = !IPC_NOWAIT;
    if (semop(semid, &op, 1) == -1)
    {
        printf("Error in down\n");
        exit(-1);
    }
}

int main()
{
    key_t key = ftok("./keyFile", 1);
    key_t key2 = ftok("./keyFile", 2);
    int shmid = shmget(key, 256, IPC_CREAT | 0666);
    if (shmid == -1)
    {
        printf("Error in creating shared memory");
    }
    void *shmadd = shmat(shmid, 0, 0);
    if (shmadd == (void *)-1)
    {
        printf("Error in attach");
        exit(-1);
    }
    int sem1 = semget(key, 1, IPC_CREAT | 0666);
    int sem2 = semget(key2, 1, IPC_CREAT | 0666);
    if (sem1 == -1 || sem2 == -1)
    {
        printf("Error in creating semaphore");
        exit(-1);
    }

    union Semun semun;
    semun.val = 0;
    semctl(sem1, 0, SETVAL, semun);
    semctl(sem2, 0, SETVAL, semun);

    while (1)
    {
        printf("Enter message to send: ");
        scanf(" %[^\n]", (char *)shmadd);
        sleep(5);
        
        up(sem1);
        down(sem2);
        printf("converted message: %s\n", (char *)shmadd);
    }
}
