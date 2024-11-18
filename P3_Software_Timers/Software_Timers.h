/* ---- Headerswitch on (for prevention of nested includes) ------------------*/
#ifndef SOFTWARE_TIMERS_H_
#define SOFTWARE_TIMERS_H_

/*----------------------------------------------------------------------------*/
/*                                 Includes                                   */
/*----------------------------------------------------------------------------*/
#include <stdint.h>
#include "Scheduler.h"

/*----------------------------------------------------------------------------*/
/*                             Defines and macros                             */
/*----------------------------------------------------------------------------*/

#define TIMERS_N                1u       /*!< Number of timers */
#define OFF_SET                 1u       /*!< Offset value */

/* Declare global variables as extern */

/* AppSched_Timer structure as extern*/
extern AppSched_Timer timers[ TIMERS_N ];

/* Declare the Timer IDs as extern */
extern uint8_t TimerID;
extern uint8_t TimerID2;
extern uint8_t TimerID3;

/*----------------------------------------------------------------------------*/
/*                                 Data types                                 */
/*----------------------------------------------------------------------------*/


/**
 * @brief Structure to represent a software timer
 */
typedef struct _AppSched_Timer
{
    uint32_t timeout;                   /*!< Timer timeout value to decrement and reload when the timer is restarted */
    uint32_t count;                     /*!< Current timer decrement count */
    uint8_t startFlag;                  /*!< Flag to indicate if the timer is running */
    void (*callbackPtr)(void);          /*!< Pointer to the callback function to be called when the timer expires */

} AppSched_Timer;


/*----------------------------------------------------------------------------*/
/*                           Declaration of functions                         */
/*----------------------------------------------------------------------------*/
/**
 * @brief Registers a timer to run within the scheduler
 * 
 * @param scheduler Pointer to the scheduler
 * @param timeout Timeout value for the timer
 * @param callbackPtr Pointer to the callback function to be called when the timer expires
 * @return uint8_t Status of the timer registration (TRUE if successful, FALSE otherwise)
 */
uint8_t AppSched_registerTimer(AppSched_Scheduler *scheduler, uint32_t timeout, void (*callbackPtr)(void));

/**
 * @brief Gets the current time remaining for a specified timer
 * 
 * @param scheduler Pointer to the scheduler
 * @param timer Timer ID
 * @return uint32_t Current time remaining for the timer
 */
uint32_t AppSched_getTimer(AppSched_Scheduler *scheduler, uint8_t timer);

/**
 * @brief Starts a specified timer
 * 
 * @param scheduler Pointer to the scheduler
 * @param timer Timer ID
 * @return uint8_t Status of the timer start (TRUE if successful, FALSE otherwise)
 */
uint8_t AppSched_startTimer(AppSched_Scheduler *scheduler, uint8_t timer);

/**
 * @brief Stops a specified timer
 * 
 * @param scheduler Pointer to the scheduler
 * @param timer Timer ID
 * @return uint8_t Status of the timer stop (TRUE if successful, FALSE otherwise)
 */
uint8_t AppSched_stopTimer(AppSched_Scheduler *scheduler, uint8_t timer);

#endif /* SOFTWARE_TIMERS_H_ */
