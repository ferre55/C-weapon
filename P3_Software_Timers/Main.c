#include <stdio.h>
#include "Scheduler.h"
#include "Software_Timers.h"

void Init_500ms(void);
void Task_500ms(void);
void Callback(void);
void Callback2(void);

int main( void )
{

    /*init the scheduler with two tasks and a tick time of 100ms and run for 10 seconds only*/
    Sche.tick = TICK_VAL;
    Sche.tasks = TASKS_N;
    Sche.timeout = 10000;
    Sche.taskPtr = tasks; /* Pointer to buffer for the TCB tasks */
    Sche.timers = TIMERS_N;
    Sche.timerPtr = timers; /* Pointer to buffer for the TCB TIMERS */

    AppSched_initScheduler( &Sche );

    /*register two task with thier corresponding init fucntions and their periodicyt, 100ms and 500ms*/
    TaskID1 = AppSched_registerTask( &Sche, Init_500ms, Task_500ms, 500 );
    TimerID = AppSched_registerTimer( &Sche, 1000u, Callback );
    printf ("This is timer ID: %d \n", TimerID);

    /* Test to register 2 timers*/
    /*
    TimerID2 = AppSched_registerTimer( &Sche, 500u, Callback2 );
    printf ("This is timer ID: %d \n", TimerID2);

    AppSched_startTimer( &Sche, TimerID2 );
    */

    AppSched_startTimer( &Sche, TimerID );
    
    //AppSched_reloadTimer( &Sche, 1, 1500);
    //AppSched_stopTimer( &Sche, TimerID);
    /*run the scheduler for the mount of time stablished in Sche.timeout*/
    AppSched_startScheduler( &Sche );
    
    return 0;
}

void Init_500ms(void)
{
    printf("Init task 500 millisecond \n");
}



void Task_500ms(void)
{
    static int loop = 0;
    printf("This is a counter from task 500ms: %d \n", loop++);
}



void Callback(void)
{
    static int loop = 0;
    printf("This is a counter from timer callback tim1: %d \n", loop++);
    AppSched_startTimer( &Sche,TimerID);

}

/*  Test to register 2 timers*/
void Callback2(void)
{
    static int loop = 0;
    printf("This is a counter from timer callback tim2: %d \n", loop++);
    AppSched_startTimer( &Sche,TimerID2);

}