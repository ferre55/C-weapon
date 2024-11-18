# Software Timers

The Timers provides a mechanism to manage and execute multiple timers within a scheduler. It allows for the registration, starting, stopping, and periodic execution of timers with specified timeouts and callback functions.

![Alt text](https://c.tadst.com/gfx/1200x630/fb-stopwatch2.png?1)

## Characteristics
- **Timer Management:** Handles multiple timers with specified timeouts.
- **Callback Execution:** Executes callback functions when timers expire.
- **Initialization and Cleanup:** Provides functions to initialize and reset timers.
- **Timer Control:** Allows starting, stopping, and reloading timers.

## How it works?
The Software Timers module uses a structure to manage timers and their execution. Each timer has an associated timeout, callback function, and control flags. The Scheduler runs timers based on their timeouts and the defined tick value.

Unlike hardware timers in microcontrollers, which rely on dedicated hardware resources, software timers are implemented purely in software. This allows for greater flexibility and scalability, as the number of timers is limited only by the available memory and processing power.

## Functions
1. **AppSched_registerTimer:** Function to register a timer with the scheduler.
2. **AppSched_getTimer:** Function to get the current time remaining for a specified timer.
3. **AppSched_startTimer:** Function to start a specified timer.
4. **AppSched_stopTimer:** Function to stop a specified timer.
5. **AppSched_reloadTimer:** Function to reload a specified timer with a new timeout value.



# Implementation in C

For the implementation, we'll use a three-file structure following C programming best practices:

1. [Main.c](Main.c): The main source file that contains:
- Program entry point (main() function)
- Function calls to test the software timers
- Required library includes
2. [Scheduler.c](Scheduler.c): The implementation source file that contains:
- Function implementations
- Required library includes
3. [Scheduler.h](Scheduler.h):The header file that contains:
- Structure definitions
- Function declarations (prototypes)
- Constant definitions
- Include guards
4. [Software_Timers.c](Software_Timers.c): The implementation source file that contains:
- Function implementations
- Required library includes
5. [Software_Timers.h](Software_Timers.h):The header file that contains:
- Structure definitions
- Function declarations (prototypes)
- Constant definitions
- Include guards

# Software_Timers.h

## Data structure definition

### Timer Structure
Each timer is represented by a structure containing:

```c
typedef struct _AppSched_Timer
{
    uint32_t timeout;                   /*!< Timer timeout value to decrement and reload when the timer is restarted */
    uint32_t count;                     /*!< Current timer decrement count */
    uint8_t startFlag;                  /*!< Flag to indicate if the timer is running */
    void (*callbackPtr)(void);          /*!< Pointer to the callback function to be called when the timer expires */
} AppSched_Timer;

```

# Software_Timers.c

## Register a timer

```c
uint8_t AppSched_registerTimer( AppSched_Scheduler *scheduler, uint32_t timeout, void (*callbackPtr)(void) )
{
    uint8_t register_timer_status;

    /* Declares a pointer to an AppSched_Timer structure */
    AppSched_Timer *newTimer = &scheduler->timerPtr[scheduler->timersCount]; 

    /* Timeout is larger than the actual tick and multiple and if i have space to store timers */
    if ((timeout > scheduler->tick) && (timeout % scheduler->tick ==  0) && (scheduler->timersCount <= scheduler->timers))
    {
        /* Timer successfully registered */
        /* Set the callback function */
        newTimer->callbackPtr = callbackPtr;
        /* Set the timeout value in milliseconds */
        newTimer->timeout = timeout;
        /* Returns an ID which is a value from 1 to the number of timer registers in the scheduler */
        register_timer_status = (scheduler->timersCount) + OFF_SET;
        /* Increment Timers count */
        scheduler->timersCount++;
    }
    else
    {   
        /* Timer couldn't be registered */
        /* Returns 0 */
        register_timer_status = FALSE;
    }

    return register_timer_status;
}

```
### Explanation of the Register Timer Function

- **Functionality**:
    - Declares a pointer to an AppSched_Timer structure.
    - Checks if the timeout is valid and if there is space to store timers.
    - Sets the callback function and timeout value.
    - Returns a timer ID if successful, otherwise returns FALSE.


## Get the current time remaining for a timer
```c
uint32_t AppSched_getTimer( AppSched_Scheduler *scheduler, uint8_t timer )
{
    uint32_t get_timer_status;

    /* Declares a pointer to an AppSched_Timer structure */
    AppSched_Timer *getTimer = &scheduler->timerPtr[timer]; 

    /* Evaluate if Timer has been registered */
    if (timer <= scheduler->timersCount)
    {
        /* Timer has been registered */
        /* Return the current timer pending time in milliseconds */
        get_timer_status = ((getTimer->timeout) - (getTimer->count) * scheduler->tick);
    }
    else
    {
        /* Timer has not been registered */
        get_timer_status = FALSE;
    }

    return get_timer_status;
}


```

### Explanation of the Get Timer Function

- **Functionality**:
    - Declares a pointer to an AppSched_Timer structure.
    - Checks if the timer has been registered.
    - Returns the current time remaining for the timer if registered, otherwise returns FALSE.


## Start a timer

```c
uint8_t AppSched_startTimer( AppSched_Scheduler *scheduler, uint8_t timer )
{
    uint8_t start_timer_status;

    AppSched_Timer *startTimer = &scheduler->timerPtr[timer - OFF_SET];

    /* Evaluate if Timer to start has been registered */
    if (timer - OFF_SET <= scheduler->timersCount)
    {
        /* Timer to start has been registered */
        startTimer->startFlag = TRUE;
        start_timer_status = TRUE;
    }
    else
    {
        /* Timer to start has not been registered */
        start_timer_status = FALSE;
    }

    return start_timer_status;
}


```

### Explanation of the Start Timer Function

- **Functionality**:
    - Declares a pointer to an AppSched_Timer structure.
    - Checks if the timer has been registered.
    - Sets the start flag to TRUE if registered, otherwise returns FALSE.


## Stop a timer

```c

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
```

### Explanation of Stop a timer Function

- **Functionality**:
    - Declares a pointer to an AppSched_Timer structure.
    - Checks if the timer has been registered.
    - Sets the start flag to FALSE.
    - Returns the stop_timer_status TRUE if was stopped, otherwise returns FALSE.




# Scheduler

## Scheduler.h

## Scheduler.c