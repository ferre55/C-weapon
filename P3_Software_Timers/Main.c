/*----------------------------------------------------------------------------*/
/*                                 Includes                                   */
/*----------------------------------------------------------------------------*/

#include <stdio.h>
#include "Scheduler.h"
#include "Software_Timers.h"

/*----------------------------------------------------------------------------*/
/*                            Function Prototypes                             */
/*----------------------------------------------------------------------------*/

void Init_500ms(void);
void Task_500ms(void);
void Callback(void);
void Callback2(void);

/*----------------------------------------------------------------------------*/
/*                                  Main                                      */
/*----------------------------------------------------------------------------*/

int main( void )
{

    /* Initialize the scheduler with two tasks and a tick time of 100ms, run for 10 seconds */
    Sche.tick = TICK_VAL;
    Sche.tasks = TASKS_N;
    Sche.timeout = 10000;
    Sche.taskPtr = tasks;   /* Pointer to buffer for the TCB tasks */
    Sche.timers = TIMERS_N;
    Sche.timerPtr = timers; /* Pointer to buffer for the TCB TIMERS */

    AppSched_initScheduler( &Sche );

    /* Register task with their corresponding init functions and periodicity*/
    TaskID1 = AppSched_registerTask( &Sche, Init_500ms, Task_500ms, 500 );
    TimerID = AppSched_registerTimer( &Sche, 1000u, Callback );
    printf ("This is timer ID: %d \n", TimerID);

    AppSched_startTimer( &Sche, TimerID );
    
    /* Uncomment to test timer reload and stop functionality */
    //AppSched_reloadTimer( &Sche, 1, 1500);
    //AppSched_stopTimer( &Sche, TimerID);

    /* Run the scheduler for the amount of time established in Sche.timeout */
    AppSched_startScheduler( &Sche );
    
    return 0;
}

/*----------------------------------------------------------------------------*/
/*                            Task Functions                                  */
/*----------------------------------------------------------------------------*/

/**
 * @brief Initialization function for the 500ms task.
 */
void Init_500ms(void)
{
    printf("Init task 500 millisecond \n");
}

/**
 * @brief Task function that runs every 500ms.
 */
void Task_500ms(void)
{
    static int loop = 0;
    printf("This is a counter from task 500ms: %d \n", loop++);
}

/*----------------------------------------------------------------------------*/
/*                            Callback Functions                              */
/*----------------------------------------------------------------------------*/

/**
 * @brief Callback function for the timer.
 */
void Callback(void)
{
    static int loop = 0;
    printf("This is a counter from timer callback tim1: %d \n", loop++);
    AppSched_startTimer( &Sche,TimerID);

}

/**
 * @brief Test function to register a second timer.
 */
void Callback2(void)
{
    static int loop = 0;
    printf("This is a counter from timer callback tim2: %d \n", loop++);
    AppSched_startTimer( &Sche,TimerID2);

}

