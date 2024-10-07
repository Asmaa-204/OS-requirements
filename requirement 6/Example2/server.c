#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void conv(char *msg, int size)
{
    int i;
    for (i = 0; i < size; ++i)
    {
        if (islower(msg[i]))
        {
            msg[i] = toupper(msg[i]);
        }
        else if (isupper(msg[i]))
        {
            msg[i] = tolower(msg[i]);
        }
    }
}

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
        exit(-1);
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
    while (1)
    {
        down(sem1);
        conv((char *)shmadd, strlen(shmadd));
        up(sem2);
    }
}