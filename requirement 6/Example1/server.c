#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int upQueue, downQueue;

void cleanup(int signum)
{
    msgctl(upQueue, IPC_RMID, NULL);
    msgctl(downQueue, IPC_RMID, NULL);
    signal(SIGINT, cleanup);
    exit(0);
}

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

struct msgBuffer
{
    int mtype;
    char msg[256];
};

int main()
{
    signal(SIGINT, cleanup);
    key_t upKey = ftok("./keyFile", 1);
    key_t downKey = ftok("./keyFile", 2);

    downQueue = msgget(downKey, IPC_CREAT | 0666);
    if (downQueue == -1)
    {
        perror("Error in creating down queue\n");
        exit(-1);
    }

    upQueue = msgget(upKey, IPC_CREAT | 0666);
    if (upQueue == -1)
    {
        perror("Error in creating up queue\n");
        exit(-1);
    }

    int recVal;
    int sentVal;
    struct msgBuffer msgBuffer;

    while (1)
    {
        sleep(10);
        recVal = msgrcv(upQueue, &msgBuffer, sizeof(msgBuffer.msg), 0, !IPC_NOWAIT);
        if (recVal == -1)
        {
            perror("Error in receive\n");
        }

        conv(msgBuffer.msg, sizeof(msgBuffer.msg));

        sentVal = msgsnd(downQueue, &msgBuffer, sizeof(msgBuffer.msg), !IPC_NOWAIT);

        if (sentVal == -1)
        {
            perror("Error in receive\n");
        }
    }
}