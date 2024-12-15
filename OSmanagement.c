#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define EXIT 0
#define INVAID -1
#define CPU 1
#define PID 5
#define RACE 2
int resource = 5;

int displayMenu();
void cpuScheduling();
void fcfs(int process[], int arrvalTime[], int burstTime[]);
void sjf(int process[], int arrvalTime[], int burstTime[]);
void displaySchedule(int process[], int arrvalTime[], int burstTime[], int waitTime[], int turnAroundTime[]);
void raceCondition();
void *threadFuncOne(); 
void *threadFuncTwo();
//int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void*), void *arg);
//int pthread_join(pthread_t, void **);

int main()
{
    int choice = INVAID;

    while (choice != EXIT)
    {
        choice = displayMenu();
        switch (choice)
        {
        case CPU:
            cpuScheduling();
            break;
        case EXIT:
            exit(EXIT);
            return 0;
        case RACE: 
            raceCondition();
        }
    }

    return 0;
}

int displayMenu()
{
    int choice = INVAID;

    // Loop for menu
    while (choice = INVAID)
    {
        printf("************OS Management Menu*************\n");
        printf("Selection the OS program to run\n");
        printf("2. Race Condition\n");
        printf("1. CPU Scheduling\n");
        printf("0. Exit\n");
        printf("Selcetion: ");
        scanf("%d", &choice);

        if (choice < 0 || choice > 2)
        {
            choice = INVAID; // Did not input a vaild number
            printf("**Please provide a vaild input!!**\n");
        }
        else
            return choice;
    }
}

void cpuScheduling()
{
    int process[PID] = {1, 2, 3, 4, 5};    // Process Numbers
    int arrvalTime[PID] = {0, 2, 4, 6, 7}; // Arrval Times
    int burstTime[PID] = {8, 5, 10, 2, 3}; // Burst Times
    fcfs(process, arrvalTime, burstTime);  // First Come First Serve Function
    sjf(process, arrvalTime, burstTime);   // Shortest Job First
}

void fcfs(int process[], int arrvalTime[], int burstTime[])
{
    int waitTime[PID] = {};
    int turnAroundTime[PID] = {};

    // Wait time calulations
    waitTime[0] = 0;
    for (int i = 1; i < PID; ++i)
    {
        waitTime[i] = waitTime[i - 1] + burstTime[i - 1];
    }

    // Turn around time Calulations
    for (int j = 0; j < PID; ++j)
    {
        turnAroundTime[j] = waitTime[j] + burstTime[j];
    }

    // Display the schedule
    printf("\t**********FCFS**********\n");
    displaySchedule(process, arrvalTime, burstTime, waitTime, turnAroundTime);
}

void sjf(int process[], int arrvalTime[], int burstTime[])
{
    int waitTime[PID] = {};
    int turnAroundTime[PID] = {};
    int index = 0;
    int temp = 0;

    // Swaping and ordering burst times smallest to biggest
    for (int index = 0; index < PID; ++index)
    {
        for (int j = index + 1; j < PID - 1; ++j)
        {
            if (burstTime[j] < burstTime[index])
            {
                // Swap burst Time
                temp = burstTime[index];
                burstTime[index] = burstTime[j];
                burstTime[j] = temp;

                // Swap Process ID
                temp = process[index];
                process[index] = process[j];
                process[j] = temp;

                // Swap Arrival Times
                temp = arrvalTime[index];
                arrvalTime[index] = arrvalTime[j];
                arrvalTime[j] = temp;
            }
        }
    }

    // Wait Time calulations
    waitTime[0] = 0;
    for (int i = 1; i < PID; ++i)
    {
        waitTime[i] = waitTime[i - 1] + burstTime[i - 1];
    }

    // Turn around time Calulations
    for (int j = 0; j < PID; ++j)
    {
        turnAroundTime[j] = waitTime[j] + burstTime[j];
    }

    // Display the algorisom
    printf("\t**********SJF**********\n");
    displaySchedule(process, arrvalTime, burstTime, waitTime, turnAroundTime);
}

void displaySchedule(int process[], int arrvalTime[], int burstTime[], int waitTime[], int turnAroundTime[])
{
    int totalWT = 0;
    int totalTAT = 0;
    double averWT;
    double averTAT;

    // Calulations for Total
    for (int i = 0; i < PID; i++)
    {
        totalWT += waitTime[i];
        totalTAT += turnAroundTime[i];
    }
    printf("PID\tAT\tBT\tWT\tTAT\n");
    printf("___\t___\t___\t___\t___\n");
    for (int i = 0; i < PID; ++i)
    {
        printf("%d\t%d\t%d\t%d\t%d\n", process[i], arrvalTime[i], burstTime[i], waitTime[i], turnAroundTime[i]);
    }

    // Calulate the average of the times
    averTAT = (double) totalTAT / PID;
    averWT = (double) totalWT / PID;

    printf("The average Waiting Time = %.2lf\n", averWT);
    printf("The average Turn Around Time = %.2lf\n", averTAT);
}

void raceCondition()
{
    pthread_t threadOne;
    pthread_t threadTwo;

    pthread_create(&threadOne, NULL, threadFuncOne, NULL);
    pthread_create(&threadTwo, NULL, threadFuncTwo, NULL);

    pthread_join(threadOne, NULL);
    pthread_join(threadTwo, NULL);

    printf("The final value of the shared resource = %d\n", resource);
}

void *threadFuncOne()
{
    int threadID = 1; 
    int fOne = resource;
    
    printf("Thread %d reads the value of the shared resources as %d\n",threadID, fOne);
    fOne++;
    printf("Local Update by thread %d: %d\n",threadID, fOne);
    
    // Sleep Function
    sleep(1);
    resource = fOne;
    printf("The value of the shared resource = %d\n", resource);

}

void *threadFuncTwo()
{
    int threadID = 2; 
    int fTwo = resource;
    
    printf("Thread %d reads resource as %d\n",threadID, fTwo);
    fTwo--;
    printf("Local Update by thread %d: %d\n",threadID, fTwo);
    
    // Sleep Function
    sleep(1);
    resource = fTwo;
    printf("The value of the shared resource = %d\n", resource);

}