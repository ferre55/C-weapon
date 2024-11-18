/*----------------------------------------------------------------------------*/
/*                                 Includes                                   */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include "Scheduler.h"
#include "Software_Timers.h"
#include "Clock_Calendar.h"


/*----------------------------------------------------------------------------*/
/*                            Function Prototypes                             */
/*----------------------------------------------------------------------------*/
void Init_500ms(void);
void Init_1000ms(void);
void Task_500ms(void);
void Task_1000ms(void);
void Callback(void);
void Callback2(void);

/*----------------------------------------------------------------------------*/
/*                                  Main                                      */
/*----------------------------------------------------------------------------*/
int main( void )
{
    /*Set the scheduler with two tasks, 1 timer and a tick time of 100ms*/
    Sche.tick = TICK_VAL; /* Tick 100 ms*/
    /* Tasks set values */
    Sche.tasks = TASKS_N; /* Capacity for 2 tasks */
    Sche.timeout = TIME_OUT; 
    Sche.taskPtr = tasks; /* Pointer to buffer for the TCB tasks */
    /* Timer set values */
    Sche.timers = TIMERS_N; /* Capacity for 1 timer */
    Sche.timerPtr = timers; /* Pointer to buffer for the TCB TIMERS */

    AppSched_initScheduler( &Sche ); /* Init scheduler */

    /*register two task with thier corresponding init fucntions and their periodicyt, 1000ms and 500ms*/
    TaskID1 = AppSched_registerTask( &Sche, Init_500ms, Task_500ms, 500 );
    TaskID2 = AppSched_registerTask( &Sche, Init_1000ms, Task_1000ms, 1000 );

    /* register the timer */
    TimerID = AppSched_registerTimer( &Sche, 1000u, Callback );

    AppRtcc_clockInit( &clock_rtcc); /* Initialize Clock*/
    
    AppRtcc_setTime( &clock_rtcc, 23, 59,0); /* Set Time */
    AppRtcc_setDate( &clock_rtcc, 28, 2,2000); /* Set Date */
    AppRtcc_setAlarm( &clock_rtcc, 0, 1 ); /* Set Alarm */

    AppSched_startTimer( &Sche, TimerID ); /* Set the flag to start the timer*/

    /*run the scheduler for the mount of time stablished in Sche.timeout*/
    AppSched_startScheduler( &Sche );

    return 0;
}

/*----------------------------------------------------------------------------*/
/*                            Task Functions                                  */
/*----------------------------------------------------------------------------*/

void Init_500ms(void)
{
    printf("Init task 500 millisecond \n");
}

void Init_1000ms(void)
{
    printf("Init task 1000 millisecond\n");
}

void Task_500ms(void)
{
    static int loop = 0;
    printf("This is a counter from task 500ms: %d \n", loop++);
}

void Task_1000ms(void)
{
    static int loop = 0;
    uint8_t get_hour, get_minutes, get_seconds;  /* Variables to store Time values*/
    uint8_t get_day, get_month, get_weekDay; /* Variables to store Date values*/
    uint16_t get_year;

    printf("This is a counter from task 1000ms:%d\n", loop++);

    AppRtcc_periodicTask( &clock_rtcc);

    /*Print Values from GetTime */
    AppRtcc_getTime(&clock_rtcc, &get_hour, &get_minutes, &get_seconds);
    printf("time- H:%dM:%dS:%d \n",get_hour,get_minutes,get_seconds);
    
    /*Print Values from GetDate */
    AppRtcc_getDate(&clock_rtcc, &get_day, &get_month, &get_year, &get_weekDay );
    printf("DATE- Day:%d WDay:%d Month:%d Year:%d\n",get_day,get_weekDay,get_month,get_year);

    if( AppRtcc_getAlarmFlag(&clock_rtcc) == ENABLE)
    {
        printf( "ALARM ACTIVE !!! \n");
        AppRtcc_clearAlarm(&clock_rtcc);
    }

} 

/*----------------------------------------------------------------------------*/
/*                            Callback Functions                              */
/*----------------------------------------------------------------------------*/

void Callback(void)
{
    static int loop = 0;
    printf("This is a counter from timer callback tim1: %d \n", loop++);
    AppSched_startTimer( &Sche,TimerID);
}