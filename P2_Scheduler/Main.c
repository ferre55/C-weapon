/*----------------------------------------------------------------------------*/
/*                                 Includes                                   */
/*----------------------------------------------------------------------------*/

#include <stdio.h>
#include "Scheduler.h"

/*----------------------------------------------------------------------------*/
/*                               Global Variables                             */
/*----------------------------------------------------------------------------*/

static AppSched_Task tasks[TASKS_N]; /* Array of TASKS_N AppSched_Task structures to be used as TCB */
static AppSched_Scheduler Sche;      /* Scheduler instance */

/*----------------------------------------------------------------------------*/
/*                           Function Prototypes                              */
/*----------------------------------------------------------------------------*/

void Init_500ms(void);
void Init_1000ms(void);
void Task_500ms(void);
void Task_1000ms(void);

/*----------------------------------------------------------------------------*/
/*                                Main Function                               */
/*----------------------------------------------------------------------------*/

int main(void)
{
    unsigned char TaskID1;
    unsigned char TaskID2;

    /* Initialize the scheduler with two tasks, a tick time of 100ms, and run for 10 seconds only */
    Sche.tick = TICK_VAL;
    Sche.tasks = TASKS_N;
    Sche.timeout = 10000;
    Sche.taskPtr = tasks;    /* Pointer to buffer for the TCB tasks */
    AppSched_initScheduler(&Sche);    

    /* Register two tasks with their corresponding init functions and their periodicity, 1000ms and 500ms */
    printf("The count of tasks is %d\n", Sche.tasksCount); // 0
    TaskID1 = AppSched_registerTask(&Sche, Init_500ms, Task_500ms, 500);
    printf("The count of tasks is %d\n", Sche.tasksCount); // 1

    printf("The period of Task %d is %d\n", TaskID1, Sche.taskPtr[0].period);
    printf("The flag is %d\n", Sche.taskPtr[0].startFlag);

    TaskID2 = AppSched_registerTask(&Sche, Init_1000ms, Task_1000ms, 1000);
    printf("The count of tasks is %d\n", Sche.tasksCount); // 2
    printf("The period of Task %d is %d\n", TaskID2, Sche.taskPtr[1].period);
    printf("The flag is %d\n", Sche.taskPtr[1].startFlag);

    /* Stop Task 2 */
    AppSched_stopTask(&Sche, 2);
    printf("The flag of Task 1 is %d\n", Sche.taskPtr[0].startFlag);
    printf("The flag of Task 2 is %d\n", Sche.taskPtr[1].startFlag);

    /* Start Task 2 */
    AppSched_startTask(&Sche, 2);


    /* Run the scheduler for the amount of time established in Sche.timeout */
    AppSched_startScheduler(&Sche);

    return 0;
}

/*----------------------------------------------------------------------------*/
/*                           Task Initialization Functions                    */
/*----------------------------------------------------------------------------*/

/**
 * @brief Initialization function for the 500ms task.
 */
void Init_500ms(void)
{
    printf("Init task 500 millisecond\n");
}

/**
 * @brief Initialization function for the 1000ms task.
 */
void Init_1000ms(void)
{
    printf("Init task 1000 millisecond\n");
}

/*----------------------------------------------------------------------------*/
/*                               Task Functions                               */
/*----------------------------------------------------------------------------*/

/**
 * @brief Task function that runs every 500ms.
 */
void Task_500ms(void)
{
    static int loop = 0;
    printf("This is a counter from task 500ms: %d\n", loop++);
}

/**
 * @brief Task function that runs every 1000ms.
 */
void Task_1000ms(void)
{
    static int loop = 0;
    printf("This is a counter from task 1000ms: %d\n", loop++);
}
