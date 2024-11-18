# Scheduler

The [Scheduler](https://en.wikipedia.org/wiki/Round-robin_scheduling) is a software component designed to manage and execute multiple tasks in a time-based manner. It allows for the registration, starting, stopping, and periodic execution of tasks.

![Alt text](https://www.boardinfinity.com/blog/content/images/2022/12/Your-paragraph-text--82--1.jpg)

## Characteristics

- **Task Management:** Handles multiple tasks with specified periods.
- **Time-Based Execution:** Executes tasks based on a defined time base (tick).
- **Initialization and Cleanup:** Provides functions to initialize and reset the scheduler.
- **Task Control:** Allows starting, stopping, and changing the periodicity of tasks.

## How it works?
 
The Scheduler uses a structure to manage tasks and their execution. Each task has an associated period, initialization function, and task function. The Scheduler runs tasks based on their periods and the defined tick value.

## Functions

To implement the Scheduler, we need to create the following functions:

1. **AppSched_initScheduler:** Function to initialize the scheduler.
2. **AppSched_registerTask:** Function to register a task with the scheduler.
3. **AppSched_stopTask:** Function to stop a registered task from running.
4. **AppSched_startTask:** Function to start a previously stopped task.
5. **AppSched_periodTask:** Function to change the period of a registered task.
6. **AppSched_startScheduler:** Function to start the scheduler and run the registered tasks.

# Implementation in C

For the implementation, we'll use a three-file structure following C programming best practices:

1. [Main.c](Main.c): The main source file that contains:
- Program entry point (main() function)
- Function calls to test the scheduler
- Required library includes
2. [Scheduler.c](Scheduler.c): The implementation source file that contains:
- Function implementations
- Required library includes
3. [Scheduler.h](Scheduler.h):The header file that contains:
- Structure definitions
- Function declarations (prototypes)
- Constant definitions
- Include guards

# Scheduler.h

## Data structure definition

### Task Structure
Each task is represented by a structure containing:
```c
typedef struct _task
{
    uint32_t period;                    /*!< How often the task should run in ms */
    uint32_t elapsed;                   /*!< The current elapsed time */
    uint8_t startFlag;                  /*!< Flag to run task */
    void (*initFunc)(void);             /*!< Pointer to init task function */
    void (*taskFunc)(void);             /*!< Pointer to task function */

} AppSched_Task; 
```
### Scheduler Structure
The Scheduler structure contains the following elements:

```c
typedef struct _scheduler
{
    uint8_t tasks;                      /*!< Number of tasks to handle */
    uint32_t tick;                      /*!< The time base in ms */
    uint32_t timeout;                   /*!< The number of milliseconds the scheduler should run */
    uint8_t tasksCount;                 /*!< Internal task counter */
    uint8_t tickCount;                  /*!< Internal tick counter */
    AppSched_Task *taskPtr;             /*!< Pointer to buffer for the TCB tasks */

} AppSched_Scheduler;
```

# Scheduler.c

## Initialize the scheduler

```c
void AppSched_initScheduler( AppSched_Scheduler *scheduler )
{
    scheduler->tasksCount = 0;        /* Initialize the task counter */
    scheduler->tickCount  = 0;        /* Initialize the tick counter */
}

```
The function AppSched_initScheduler initializes the scheduler by setting:
- tasksCount to 0 (initializes the task counter).
- tickCount to 0 (initializes the tick counter).

## Register a task


```c

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


```
- The function AppSched_registerTask registers a new task in the scheduler.

### Validate Periodicity
The function also validates the periodicity to ensure it is not less than the tick value and is a multiple of TIME_OUT. If the validation is successful, the task is registered, and the function returns a task ID. If not, it returns FALSE.

## Stop a task
The function AppSched_stopTask stops a specific task in the scheduler. It sets the startFlag of the task to FALSE to indicate that the task should no longer run.
```c
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


```
### Explanation of the stop Function

- The function takes a scheduler and a task ID as parameters.
- It checks if the task ID is valid (greater than 0 and less than or equal to TASKS_N).
- If valid, it sets the startFlag of the task to FALSE and returns TRUE to indicate success.
- If not valid, it returns FALSE to indicate failure.

## Start a task

The function AppSched_startTask starts a specific task in the scheduler. It sets the startFlag of the task to TRUE to indicate that the task should run.

```c
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


```
### Explanation of the start Function

- The function takes a scheduler and a task ID as parameters.
- It checks if the task ID is valid (greater than 0 and less than or equal to TASKS_N).
- If valid, it sets the startFlag of the task to TRUE and returns TRUE to indicate success.
- If not valid, it returns FALSE to indicate failure.



## Change task period
The function AppSched_periodTask changes the periodicity of a specific task in the scheduler. It sets the period of the task and ensures the task’s periodicity is valid.

```c
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


```

### Explanation of the change task period Function

- The function takes a scheduler, a task ID, and a period as parameters.
- It checks if the task ID is valid (greater than 0 and less than or equal to TASKS_N).
- It validates the periodicity to ensure it is a multiple of TICK_VAL.
- If valid, it sets the period of the task and the startFlag to TRUE, returning TRUE to indicate success.
- If not valid, it returns FALSE to indicate failure.


## Start the scheduler

The function AppSched_startScheduler starts the scheduler and manages the execution of tasks based on their periodicity and initialization routines.

```c

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
        }
    }
}

```
### Initialization of Time Variables
```c
uint32_t tickstart = milliseconds();
uint32_t elapsed = 0; /* Counter of ticks */
uint32_t new_elapsed = 0; /* Counter of ticks */
```
- tickstart is initialized with the current time in milliseconds.
- elapsed and new_elapsed are initialized to 0 to keep track of the elapsed time.

### Running Initialization Functions
```c
```
- The function iterates through all tasks in the scheduler.
- For each task, if an initialization function (initFunc) is defined and the task’s startFlag is TRUE, the initialization function is called.

### Main Scheduler Loop
```c
```
- The loop runs until new_elapsed exceeds the scheduler’s timeout value.
- new_elapsed is updated with the current time minus tickstart.
- If the difference between new_elapsed and elapsed is greater than or equal to the scheduler’s tick interval, the following actions are performed:
    - The tickCount is incremented.
    - elapsed is updated by adding the tick interval.
    - The function iterates through all tasks in the scheduler.
    - For each task, if the task’s period is a multiple of the tick interval and the task’s startFlag is TRUE, the task function (taskFunc) is called.
