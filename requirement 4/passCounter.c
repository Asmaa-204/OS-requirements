#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int getPassNum(int numStudents, int numTAs, int taIndex, int passGrade, int *grades)
{
    int start = (numStudents / numTAs) * taIndex;
    int end = (taIndex == numTAs - 1) ? numStudents : start + (numStudents / numTAs);

    int passingCount = 0;
    for (int i = start; i < end; i++)
    {
        if (grades[i * 2] + grades[i * 2 + 1] >= passGrade)
        {
            passingCount++;
        }
    }

    exit(passingCount);
}

int main(int argc, char *argv[])
{
    char *filePath = argv[1];
    int numTAs = atoi(argv[2]);
    int passGrade = atoi(argv[3]);
    int pid[numTAs];

    int stat_loc;
    int sid;

    FILE *file = fopen(filePath, "r");
    if (!file)
    {
        printf("Error: Unable to open file\n");
        return 1;
    }

    int numStudents;
    fscanf(file, "%d", &numStudents);

    int *grades = (int *)malloc(sizeof(int) * numStudents * 2);

    for (int i = 0; i < numStudents; i++)
    {
        fscanf(file, "%d %d", &grades[i * 2], &grades[i * 2 + 1]);
    }

    fclose(file);

    int *passingCounts = (int *)malloc(sizeof(int) * numTAs);

    for (int i = 0; i < numTAs; i++)
    {
        pid[i] = fork();
        if (pid[i] == -1)
        {
            perror("Error in forking\n");
            return 1;
        }
        if (pid[i] == 0)
        {
            getPassNum(numStudents, numTAs, i, passGrade, grades);
        }
    }

    for (int i = 0; i < numTAs; i++)
    {
        waitpid(pid[i] ,&stat_loc, 0);
        if (!(stat_loc & 0X00FF))
        {
            printf("%d ",stat_loc >> 8);
        }
    }

    printf("\n");
}