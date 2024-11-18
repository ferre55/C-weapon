/**
 * \file       Scheduler.c
 * \author     Jennifer Reynaga
 * \brief      Implementation for Scheduler
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
/* Scheduler configuration structures */
AppSched_Scheduler Sche;
AppSched_Task tasks[ TASKS_N ];

/* Tasks IDs */
uint8_t TaskID1, TaskID2;

/*----------------------------------------------------------------------------*/
/*                       Declaration of local functions                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                     Implementation of global functions                     */
/*----------------------------------------------------------------------------*/

void AppSched_initScheduler( AppSched_Scheduler *scheduler )
{
    scheduler->tasksCount = 0;        /* Initialize the task counter */
    scheduler->tickCount  = 0;        /* Initialize the tick counter */
    scheduler->timersCount = 0;       /* Initialize the timer counter */
}

uint8_t AppSched_registerTask( AppSched_Scheduler *scheduler, void (*initPtr)(void), void (*taskPtr)(void), uint32_t period )
{
    uint8_t register_task_status;
    
    /* Set the task TCB with the following parameters */
    AppSched_Task *newTask = &scheduler->taskPtr[scheduler->tasksCount]; /* Declares a pointer to an AppSched_Task structure */

    /* 1. Address of the function to hold the init routine/NULL for the given task */
    if (initPtr == NULL) /* Evaluate if the task has an init routine */
    {
        newTask->initFunc = NULL; /* A NULL parameter should be accepted by the function */
    }
    else /* If the task HAS an init routine */
    {
        newTask->initFunc = initPtr; /* Address of the function to hold the init routine for the given task */
    }

    /* 2. Address for the actual routine that will run as the task */
    newTask->taskFunc = taskPtr;

    /* Validate the periodicity */
    if ((period >= scheduler->tick) && (TIME_OUT % period == 0)) /* Should not be less than the tick value and always be multiple */
    {
        /* 3. The periodicity in milliseconds of the task to register */
        newTask->period = period;
        /* Periodicity Validated */
        /* The function shall return a Task ID which will be a number from 1 to n task registered if the operation was a success */
        register_task_status = (scheduler->tasksCount) + 1; /* Operation was a success */
        newTask->startFlag = TRUE; /* Start to run task */
        scheduler->tasksCount++;
    }
    else
    {
        /* Periodicity NOT Validated */
        register_task_status = FALSE; /* Operation was NOT successful */
    }
    
    return register_task_status;
}

uint8_t AppSched_stopTask( AppSched_Scheduler *scheduler, uint8_t task )
{
    uint8_t stop_task_status;

    /* Stop task of a specific task */
    AppSched_Task *stopTask = &scheduler->taskPtr[task - 1];

    if ((task > 0) && (task <= TASKS_N))
    {
        stopTask->startFlag = FALSE;
        stop_task_status = TRUE;   /* The function will return TRUE if the task was stopped */
    }
    else
    {
        stop_task_status = FALSE; /* The function will return FALSE if the task was NOT stopped */
    }

    return stop_task_status;
}

uint8_t AppSched_startTask( AppSched_Scheduler *scheduler, uint8_t task )
{
    uint8_t start_task_status;

    /* Start task of a specific task */
    AppSched_Task *startTask = &scheduler->taskPtr[task - 1];

    if ((task > 0) && (task <= TASKS_N))
    {
        startTask->startFlag = TRUE;
        start_task_status = TRUE;   /* TRUE if the task was started */ 
    }
    else
    {
        start_task_status = FALSE; /* FALSE if the task was NOT started */
    }

    return start_task_status;
}

uint8_t AppSched_periodTask( AppSched_Scheduler *scheduler, uint8_t task, uint32_t period )
{
    uint8_t period_task_status;

    AppSched_Task *periodTask = &scheduler->taskPtr[task - 1];

    if ((task > 0) && (task <= TASKS_N))
    {
        if (TICK_VAL % period == 0) /* Should not be less than the tick value and always be multiple */
        {
            /* The periodicity in milliseconds of the task to register */
            periodTask->period = period;
            /* Periodicity Validated */
            periodTask->startFlag = TRUE; /* Start to run task */
            period_task_status = TRUE;   /* TRUE if the task period was changed */
        }
        else
        {
            period_task_status = FALSE; /* FALSE if the task period was NOT changed */
        }
    }
    else
    {
        period_task_status = FALSE; /* FALSE if the task period was NOT changed */
    }

    return period_task_status;
}

long milliseconds(void)
{
    return clock() / (CLOCKS_PER_SEC / 1000);
}

void AppSched_startScheduler( AppSched_Scheduler *scheduler )
{
    /* Get the milliseconds for the first time */
    uint32_t tickstart = milliseconds();
    uint32_t elapsed = 0; /* Counter of ticks */
    uint32_t new_elapsed = 0; /* Counter of ticks */

    /* Running the task init functions one single time */
    for (uint8_t y = 0; y < scheduler->tasks; y++)
    {
        if ((scheduler->taskPtr[y].initFunc != NULL) && (scheduler->taskPtr[y].startFlag == TRUE))
        {
            scheduler->taskPtr[y].initFunc();
        }
        else
        {
            /* initFunc == NULL */
            /* Do Nothing */
        }
    }

    while (new_elapsed <= scheduler->timeout)
    {   
        new_elapsed = milliseconds() - tickstart;

        if (new_elapsed - elapsed >= scheduler->tick)
        {
            scheduler->tickCount++;
            elapsed += scheduler->tick;

            for (uint8_t a = 0; a < scheduler->tasks; a++)
            {
                if ((scheduler->tickCount % ((scheduler->taskPtr[a].period) / (scheduler->tick)) == 0) && (scheduler->taskPtr[a].startFlag == TRUE))
                {
                    scheduler->taskPtr[a].taskFunc();
                }
            }
            
            for (uint8_t b = 0; b < scheduler -> timers; b++)
            {
                
                scheduler -> timerPtr[b].count++; /* Store each tick*/
                
                /* Timer shall count from a timeout value down to zero*/
                if(((AppSched_getTimer(scheduler,b)) == 0) && (scheduler -> timerPtr[b].startFlag == TRUE))
                {
                    /* Reset count to start again*/
                    scheduler -> timerPtr[b].count = 0;
                    /* Callback function*/
                    scheduler -> timerPtr[b].callbackPtr();

                }

            }

        }
    }
}

/*----------------------------------------------------------------------------*/
/*                             END OF FILE                                    */
/*----------------------------------------------------------------------------*/
