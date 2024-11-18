/* ---- Headerswitch on (for prevention of nested includes) ------------------*/

#ifndef CLOCK_CALENDARS_H_
#define CLOCK_CALENDARS_H_

/**
 * \file       Clock_Calendar.h
 * \author     Jennifer Reynaga
 * \brief      Header file for the Clock_Calendar.
 */

/*----------------------------------------------------------------------------*/
/*                                  Includes                                  */
/*----------------------------------------------------------------------------*/
#include <stdint.h>

/*----------------------------------------------------------------------------*/
/*                             Defines and macros                             */
/*----------------------------------------------------------------------------*/
#define DISABLE                 0
#define ENABLE                  1
#define DEFAULT_VAL             0

/* Time Max and Min */
/* Note: All time parameters have the same min val */
#define MIN_TIME                0
#define MAX_MIN                 59
#define MAX_SEC                 59
#define MAX_HOUR                23

/* Date Max and Min */
#define MIN_DAY                 1
#define MAX_DAY                 31
#define MIN_WDAY                0       /* 0 == SUNDAY */
#define MAX_WDAY                6       /* 6 == SATURDAY */
#define RESET_WDAY              1       /* 1 == MONDAY */
#define MIN_MONTH               1
#define MAX_MONTH               12
#define MIN_YEAR                1900
#define MAX_YEAR                2100

/* For increment Time and Date Values*/
#define SECOND                  1
#define MINUTE                  1
#define HOUR                    1
#define MONTH                   1
#define YEAR                    1

/* Days for Months */
#define THIRTY_ONE              31
#define THIRTY                  30
#define TWENTY_EIGHT            28
#define TWENTY_NINE             29

/* MONTHS to evaluate */
#define JANUARY                 1
#define FEBRUARY                2



/*----------------------------------------------------------------------------*/
/*                                 Data types                                 */
/*----------------------------------------------------------------------------*/

/*  The RTCC should have a control union structure to handle certain number of 
flags each of them represented by a single bit */
typedef union _AppRtcc_Ctrl
{
    uint8_t Register;   /*8 bit vaiable*/
    struct _bits
    {
        uint8_t clk_en : 1;         /*!< clock enable flag      */
        uint8_t al_set : 1;         /*!< alarm set flag         */
        uint8_t al_active : 1;      /*!< alarm active flag      */
        uint8_t none : 4;
    } bits;
} AppRtcc_Ctrl;

/* The RTCC should have the following control structure which will be passed 
as parameter on each interface, making no necessary the usage of global variables  */

typedef struct _AppRtcc_Clock 
{
    uint8_t tm_sec;             /*!< seconds,  range 0 to 59          */
    uint8_t tm_min;             /*!< minutes, range 0 to 59           */
    uint8_t tm_hour;            /*!< hours, range 0 to 23             */
    uint8_t tm_mday;            /*!< day of the month, range 1 to 31  */
    uint8_t tm_mon;             /*!< month, range 1 to 12             */
    uint16_t tm_year;           /*!< years in rage 1900 2100          */
    uint8_t tm_wday;            /*!< day of the week, range 0 to 6    */
    uint8_t al_min;             /*!< alarm minutes, range 0 to 59     */
    uint8_t al_hour;            /*!< alarm hours, range 0 to 23       */
    uint8_t mt_days[13u];       /*!< max days on each month */
    AppRtcc_Ctrl ctrl;          /*!< clock control bits */
} AppRtcc_Clock;

/*----------------------------------------------------------------------------*/
/*                       Declaration of Global Variables                      */
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/*                           Declaration of functions                         */
/*----------------------------------------------------------------------------*/

/**
 * @brief  Interface to initialize the internal clock structure
*/
void AppRtcc_clockInit( AppRtcc_Clock *rtcc );

/**
 * @brief   Interface to set a new time
*/
uint8_t AppRtcc_setTime( AppRtcc_Clock *rtcc, uint8_t hour, uint8_t minutes, uint8_t seconds );

/**
 * @brief   Interface to set a new date
*/
uint8_t AppRtcc_setDate( AppRtcc_Clock *rtcc, uint8_t day, uint8_t month, uint16_t year );

/**
 * @brief   Interface to set a new alarm value
*/
uint8_t AppRtcc_setAlarm( AppRtcc_Clock *rtcc, uint8_t hour, uint8_t minutes );

/**
 * @brief   Interface to read the current time
*/
void AppRtcc_getTime( AppRtcc_Clock *rtcc, uint8_t *hour, uint8_t *minutes, uint8_t *seconds );

/**
 * @brief    Interface to read the current date
*/
void AppRtcc_getDate( AppRtcc_Clock *rtcc, uint8_t *day, uint8_t *month, uint16_t *year, uint8_t *weekDay );

/**
 * @brief   Interface to read the current alarm
*/
uint8_t AppRtcc_getAlarm( AppRtcc_Clock *rtcc, uint8_t *hour, uint8_t *minutes );

/**
 * @brief   Interface  to clear an active current alarm
*/
void AppRtcc_clearAlarm( AppRtcc_Clock *rtcc );

/**
 * @brief   Interface to get the flag that indicates if the alarm is active
*/
uint8_t AppRtcc_getAlarmFlag( AppRtcc_Clock *rtcc );

/**
 * @brief   Interface to Clock Periodic function
*/
void AppRtcc_periodicTask( AppRtcc_Clock *rtcc );

/**
 * @brief   Interface to update Time values
*/
uint8_t AppRtcc_updateTime( AppRtcc_Clock *rtcc);

/**
 * @brief   Interface to update Date values
*/
void AppRtcc_updateDate( AppRtcc_Clock *rtcc, uint8_t update_day);

/**
 * @brief   Interface to reset/set initial values of date and Time
*/
void AppRtcc_initDateTime( AppRtcc_Clock *rtcc);

/**
 * @brief   Interface to evaluate if the year is leap or not
*/
uint8_t AppRtcc_leapYear( uint16_t year);

/**
 * @brief   Interface to set MAX Days for each month
*/
void AppRtcc_maxDaysMonths( AppRtcc_Clock *rtcc);

/**
 * @brief   Interface to give MAX Days depending the month
*/
uint8_t AppRtcc_maxDaysMonth( AppRtcc_Clock *rtcc, uint8_t month, uint8_t leap);

/**
 * @brief   Interface to indicate if the Alarm is Active
*/
void AppRtcc_alarmStatus( AppRtcc_Clock *rtcc);

#endif /* CLOCK_CALENDARS_H_ */
