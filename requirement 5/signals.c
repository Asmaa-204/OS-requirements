#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int searchValue;
int pid1, pid2;

void handler(int sigNum)
{
    int stat_loc;
    int sid = wait(&stat_loc) == pid1 ? 1 : 2;
    if (!(stat_loc & 0x00FF))
    {
        printf("Child %d: Value %d found at position %d.\n", sid, searchValue, stat_loc >> 8);
    }
    killpg(getpgrp(), SIGKILL);
    signal(SIGUSR1, handler);
}

void search(int list[], int childRank, int start, int end)
{
    for (int i = start; i < end; i++)
    {
        if (searchValue == list[i])
        {
            kill(getppid(), SIGUSR1);
            exit(i);
        }
    }

    sleep(3);
    printf("Child %d terminates\n", childRank);
    exit(-1);
}

int main(int argc, char *argv[])
{
    searchValue = atoi(argv[1]);
    int arraySize = argc - 2;

    int list[argc - 1];
    for (int i = 0; i < arraySize; i++)
    {
        list[i] = atoi(argv[i + 2]);
    }

    signal(SIGUSR1, handler);

    printf("I'm parent process with pid = %d\n", getpid());
    pid1 = fork();

    if (pid1 == 0)
    {
        printf("I am the first child, PID = %d, PPID = %d\n", getpid(), getppid());
        search(list, 1, 0, arraySize / 2);
    }
    else
    {
        pid2 = fork();
        if (pid2 == 0)
        {
            printf("I am the second child, PID = %d, PPID = %d\n", getpid(), getppid());
            search(list, 2, arraySize / 2, arraySize);
        }
        else
        {
            sleep(5);
            wait(NULL);
            wait(NULL);
            printf("Value Not Found\n");
        }
    }
}

// The process executing the handler is the one that receives the signal..
// in this case the parent is the one that executes the handler

// The handler will always receive exit code as it's not called unless a value is found
// and an exit code with this position is sent..is waits for the exit code to conitue executing