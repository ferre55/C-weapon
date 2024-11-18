/* ---- Headerswitch on (for prevention of nested includes) ------------------*/

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

/**
 * \file       Scheduler.h
 * \author     Jennifer Reynaga
 * \brief      Header file for the Scheduler.
 */

/*----------------------------------------------------------------------------*/
/*                                  Includes                                  */
/*----------------------------------------------------------------------------*/
#include <stdint.h>

/*----------------------------------------------------------------------------*/
/*                             Defines and macros                             */
/*----------------------------------------------------------------------------*/
#define FALSE                0u         /*!< Boolean false value */
#define TRUE                 1u         /*!< Boolean true value */
#define TASKS_N              2u         /*!< Number of tasks */
#define TICK_VAL             100u       /*!< Tick value in milliseconds */
#define TIME_OUT             10000u     /*!< Timeout value in milliseconds */

/*----------------------------------------------------------------------------*/
/*                                 Data types                                 */
/*----------------------------------------------------------------------------*/
/**
 * @brief Structure to represent a task.
 */
typedef struct _task
{
    uint32_t period;                    /*!< How often the task should run in ms */
    uint32_t elapsed;                   /*!< The current elapsed time */
    uint8_t startFlag;                  /*!< Flag to run task */
    void (*initFunc)(void);             /*!< Pointer to init task function */
    void (*taskFunc)(void);             /*!< Pointer to task function */

} AppSched_Task; 

/**
 * @brief Structure to represent the scheduler.
 */
typedef struct _scheduler
{
    uint8_t tasks;                      /*!< Number of tasks to handle */
    uint32_t tick;                      /*!< The time base in ms */
    uint32_t timeout;                   /*!< The number of milliseconds the scheduler should run */
    uint8_t tasksCount;                 /*!< Internal task counter */
    uint8_t tickCount;                  /*!< Internal tick counter */
    AppSched_Task *taskPtr;             /*!< Pointer to buffer for the TCB tasks */

} AppSched_Scheduler;

/*----------------------------------------------------------------------------*/
/*                           Declaration of functions                         */
/*----------------------------------------------------------------------------*/

/**
 * @brief Scheduler initialization interface.
 * 
 * Initializes the scheduler with the given parameters.
 * 
 * @param scheduler Pointer to the scheduler structure to initialize.
 */
void AppSched_initScheduler( AppSched_Scheduler *scheduler );

/**
 * @brief Interface to register the tasks to run.
 * 
 * Registers a task with the scheduler.
 * 
 * @param scheduler Pointer to the scheduler structure.
 * @param initPtr Pointer to the task initialization function.
 * @param taskPtr Pointer to the task function.
 * @param period The period in milliseconds for the task to run.
 * @return uint8_t Status of the task registration.
 */
uint8_t AppSched_registerTask( AppSched_Scheduler *scheduler, void (*initPtr)(void), void (*taskPtr)(void), uint32_t period );

/**
 * @brief Interface to stop any of the registered tasks from running.
 * 
 * Stops a registered task from running.
 * 
 * @param scheduler Pointer to the scheduler structure.
 * @param task The task index to stop.
 * @return uint8_t Status of the task stop operation.
 */
uint8_t AppSched_stopTask( AppSched_Scheduler *scheduler, uint8_t task );

/**
 * @brief Interface to start any of the registered tasks previously stopped.
 * 
 * Starts a previously stopped task.
 * 
 * @param scheduler Pointer to the scheduler structure.
 * @param task The task index to start.
 * @return uint8_t Status of the task start operation.
 */
uint8_t AppSched_startTask( AppSched_Scheduler *scheduler, uint8_t task );

/**
 * @brief Interface to change the task periodicity of any of the registered tasks.
 * 
 * Changes the period of a registered task.
 * 
 * @param scheduler Pointer to the scheduler structure.
 * @param task The task index to change the period.
 * @param period The new period in milliseconds for the task to run.
 * @return uint8_t Status of the task period change operation.
 */
uint8_t AppSched_periodTask( AppSched_Scheduler *scheduler, uint8_t task, uint32_t period );

/**
 * @brief Interface that will run the different tasks that have been registered.
 * 
 * Starts the scheduler to run the registered tasks.
 * 
 * @param scheduler Pointer to the scheduler structure.
 */
void AppSched_startScheduler( AppSched_Scheduler *scheduler );

#endif /* SCHEDULER_H_ */
