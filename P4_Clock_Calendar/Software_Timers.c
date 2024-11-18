/**
 * \file       Software_Timers.c
 * \author     Jennifer Reynaga
 * \brief      Implementation for Software Timers
 */

/*----------------------------------------------------------------------------*/
/*                                 Includes                                   */
/*----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Scheduler.h"
#include "Software_Timers.h"

/*----------------------------------------------------------------------------*/
/*                               Local defines                                */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                       Declaration of Global Variables                      */
/*----------------------------------------------------------------------------*/

/* Timer configuration structure */
AppSched_Timer timers[ TIMERS_N ];

/* Timers IDs */
uint8_t TimerID;
uint8_t TimerID2;
uint8_t TimerID3;

/*----------------------------------------------------------------------------*/
/*                     Implementation of functions                            */
/*----------------------------------------------------------------------------*/

uint8_t AppSched_registerTimer( AppSched_Scheduler *scheduler, uint32_t timeout, void (*callbackPtr)(void) )
{
    uint8_t register_timer_status;

    /* Declares a pointer to an AppSched_Timer structure */
    AppSched_Timer *newTimer = &scheduler -> timerPtr[scheduler -> timersCount]; 

    /* Timeout is larger than the actual tick and multiple and if i have space to store timers*/
    if( (timeout > scheduler -> tick) && (timeout % scheduler -> tick ==  0) && (scheduler -> timersCount <= scheduler -> timers) )
    {
        /* Timer successfully register */
        /* Set the callback function */
        newTimer -> callbackPtr = callbackPtr;
        /* Set the timeout value in milliseconds */
        newTimer -> timeout = timeout;
        /* Returns an ID which is a value from 1 to the number of timer registers in the scheduler */
        register_timer_status = (scheduler -> timersCount) + OFF_SET;
        /* Increment Timers count*/
        scheduler -> timersCount++;
    }
    else
    {   
        /*  Timer couldn't be registered */
        /*  Returns 0 */
        register_timer_status = FALSE;
    }

    return register_timer_status;
}

uint32_t AppSched_getTimer( AppSched_Scheduler *scheduler, uint8_t timer )
{
    uint32_t get_timer_status;

    /* Declares a pointer to an AppSched_Timer structure */
    AppSched_Timer *getTimer = &scheduler -> timerPtr[timer]; 

    /*  Evaluate if Timer has been registered  */
    if(timer <= scheduler -> timersCount)
    {
        /*  Timer has been registered  */
        /*  Return the current timer pending time in milliseconds   */
        get_timer_status = ((getTimer ->  timeout) - (getTimer -> count)* scheduler -> tick);
    }
    else
    {
        /*  Timer has not been registered  */
        get_timer_status = FALSE;
    }

    return get_timer_status;

}

uint8_t AppSched_reloadTimer( AppSched_Scheduler *scheduler, uint8_t timer, uint32_t timeout )
{
    uint8_t reload_timer_status;

    AppSched_Timer *reloadTimer = &scheduler -> timerPtr[timer - OFF_SET]; 

    /*  Evaluate if Timer to reload has been registered  */
    if((timer <= scheduler -> timersCount) && (timeout > scheduler -> tick) && (timeout % scheduler -> tick ==  0))
    {
        /*  Timer to reload has been registered  */
        /*  The timer will be reloaded with a new value in milliseconds */  
        reloadTimer -> timeout = timeout;
        reloadTimer -> startFlag = TRUE;
        reload_timer_status = TRUE;
    }
    else
    {
        /*  Timer to reload has not been registered  */
        reload_timer_status = FALSE;
    }

    return reload_timer_status;

}

uint8_t AppSched_startTimer( AppSched_Scheduler *scheduler, uint8_t timer )
{
    uint8_t start_timer_status;

    AppSched_Timer *startTimer = &scheduler -> timerPtr[timer - OFF_SET];

    /*  Evaluate if Timer to reload has been registered  */
    if(timer - OFF_SET <= scheduler -> timersCount)
    {
        /* Timer to start has been registered  */
        startTimer -> startFlag = TRUE;
        start_timer_status = TRUE;
    }
    else
    {
        /*  Timer to start has not been registered  */
        start_timer_status = FALSE;
    }

    return start_timer_status;

}

uint8_t AppSched_stopTimer( AppSched_Scheduler *scheduler, uint8_t timer )
{

    uint8_t stop_timer_status;

    AppSched_Timer *stopTimer = &scheduler -> timerPtr[timer - OFF_SET];

    /*  Evaluate if Timer to stop has been registered  */
    if(timer <= scheduler -> timersCount)
    {
        /*  Timer to stop has been registered  */
        stopTimer -> startFlag = FALSE;
        stop_timer_status = TRUE;
    }
    else
    {
        /*  Timer to stop has not been registered  */
        stop_timer_status = FALSE;
    }

    return stop_timer_status;

}

/*----------------------------------------------------------------------------*/
/*                             END OF FILE                                    */
/*----------------------------------------------------------------------------*/