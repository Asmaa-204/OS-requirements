#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <ctype.h>

#define MAX_MSG_SIZE 256

struct msgBuffer
{
    long mtype;
    char msg[MAX_MSG_SIZE];
};

int upQueue, downQueue;

void cleanup(int signum)
{
    printf("\nClient Terminated.\n");
    signal(SIGINT, cleanup);
    exit(0);
}

int main()
{
    signal(SIGINT, cleanup);

    key_t upKey = ftok("./keyFile", 1);
    key_t downKey = ftok("./keyFile", 2);

    upQueue = msgget(upKey, IPC_CREAT | 0666);
    if (upQueue == -1)
    {
        perror("Error in Creating up queue");
        exit(-1);
    }

    downQueue = msgget(downKey, IPC_CREAT | 0666);
    if (downQueue == -1)
    {
        perror("Error in Creating down queue");
        exit(-1);
    }

    struct msgBuffer msgBuffer;
    msgBuffer.mtype = getpid() % 10000;
    int recVal;
    int sentVal;

    while (1)
    {
        printf("Enter message to send: ");
        scanf(" %[^\n]", msgBuffer.msg);
        sleep(5);

        sentVal = msgsnd(upQueue, &msgBuffer, sizeof(msgBuffer.msg), !IPC_NOWAIT);
        if (sentVal == -1)
        {
            perror("Error in send");
        }
        recVal = msgrcv(downQueue, &msgBuffer, sizeof(msgBuffer.msg), msgBuffer.mtype, !IPC_NOWAIT);
        if (recVal == -1)
        {
            perror("Error in receive");
        }
        printf("Converted message: %s\n", msgBuffer.msg);
    }
}