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
#define DISABLE                 0u
#define ENABLE                  1u
#define DEFAULT_VAL             0u

/* Time Max and Min */
/* Note: All time parameters have the same min val */
#define MIN_TIME                0u
#define MAX_MIN                 59u
#define MAX_SEC                 59u
#define MAX_HOUR                23u

/* Date Max and Min */
#define MIN_DAY                 1u
#define MAX_DAY                 31u
#define MIN_WDAY                0u       /* 0 == SUNDAY */
#define MAX_WDAY                6u       /* 6 == SATURDAY */
#define RESET_WDAY              1u       /* 1 == MONDAY */
#define MIN_MONTH               1u
#define MAX_MONTH               12u
#define MIN_YEAR                1900u
#define MAX_YEAR                2100u

/* For increment Time and Date Values*/
#define SECOND                  1u
#define MINUTE                  1u
#define HOUR                    1u
#define MONTH                   1u
#define YEAR                    1u

/* Days for Months */
#define THIRTY_ONE              31u
#define THIRTY                  30u
#define TWENTY_EIGHT            28u
#define TWENTY_NINE             29u

/* MONTHS to evaluate */
#define JANUARY                 1u
#define FEBRUARY                2u



/*----------------------------------------------------------------------------*/
/*                                 Data types                                 */
/*----------------------------------------------------------------------------*/

/* Control union structure to handle various flags, each represented by a single bit */
typedef union _AppRtcc_Ctrl
{
    uint8_t Register;   /* 8-bit variable */
    struct _bits
    {
        uint8_t clk_en : 1;         /*!< Clock enable flag      */
        uint8_t al_set : 1;         /*!< Alarm set flag         */
        uint8_t al_active : 1;      /*!< Alarm active flag      */
        uint8_t none : 4;           /*!< Unused bits            */
    } bits;
} AppRtcc_Ctrl;

/* Control structure for the RTCC, passed as a parameter in each interface, 
   eliminating the need for global variables */
typedef struct _AppRtcc_Clock 
{
    uint8_t tm_sec;             /*!< Seconds, range 0 to 59          */
    uint8_t tm_min;             /*!< Minutes, range 0 to 59           */
    uint8_t tm_hour;            /*!< Hours, range 0 to 23             */
    uint8_t tm_mday;            /*!< Day of the month, range 1 to 31  */
    uint8_t tm_mon;             /*!< Month, range 1 to 12             */
    uint16_t tm_year;           /*!< Year, range 1900 to 2100         */
    uint8_t tm_wday;            /*!< Day of the week, range 0 to 6    */
    uint8_t al_min;             /*!< Alarm minutes, range 0 to 59     */
    uint8_t al_hour;            /*!< Alarm hours, range 0 to 23       */
    uint8_t mt_days[13u];       /*!< Maximum days in each month       */
    AppRtcc_Ctrl ctrl;          /*!< Clock control bits               */
} AppRtcc_Clock;

/*----------------------------------------------------------------------------*/
/*                       Declaration of Global Variables                      */
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/*                           Declaration of functions                         */
/*----------------------------------------------------------------------------*/
/**
 * @brief  Interface to initialize the internal clock structure
 * 
 * @param rtcc Pointer to the RTCC clock structure
 */
void AppRtcc_clockInit(AppRtcc_Clock *rtcc);

/**
 * @brief   Interface to set a new time
 * 
 * @param rtcc Pointer to the RTCC clock structure
 * @param hour Hour value to set
 * @param minutes Minute value to set
 * @param seconds Second value to set
 * @return uint8_t Status of the time set operation (TRUE if successful, FALSE otherwise)
 */
uint8_t AppRtcc_setTime(AppRtcc_Clock *rtcc, uint8_t hour, uint8_t minutes, uint8_t seconds);

/**
 * @brief   Interface to set a new date
 * 
 * @param rtcc Pointer to the RTCC clock structure
 * @param day Day value to set
 * @param month Month value to set
 * @param year Year value to set
 * @return uint8_t Status of the date set operation (TRUE if successful, FALSE otherwise)
 */
uint8_t AppRtcc_setDate(AppRtcc_Clock *rtcc, uint8_t day, uint8_t month, uint16_t year);

/**
 * @brief   Interface to set a new alarm value
 * 
 * @param rtcc Pointer to the RTCC clock structure
 * @param hour Hour value to set for the alarm
 * @param minutes Minute value to set for the alarm
 * @return uint8_t Status of the alarm set operation (TRUE if successful, FALSE otherwise)
 */
uint8_t AppRtcc_setAlarm(AppRtcc_Clock *rtcc, uint8_t hour, uint8_t minutes);

/**
 * @brief   Interface to read the current time
 * 
 * @param rtcc Pointer to the RTCC clock structure
 * @param hour Pointer to store the current hour value
 * @param minutes Pointer to store the current minute value
 * @param seconds Pointer to store the current second value
 */
void AppRtcc_getTime(AppRtcc_Clock *rtcc, uint8_t *hour, uint8_t *minutes, uint8_t *seconds);

/**
 * @brief    Interface to read the current date
 * 
 * @param rtcc Pointer to the RTCC clock structure
 * @param day Pointer to store the current day value
 * @param month Pointer to store the current month value
 * @param year Pointer to store the current year value
 * @param weekDay Pointer to store the current weekday value
 */
void AppRtcc_getDate(AppRtcc_Clock *rtcc, uint8_t *day, uint8_t *month, uint16_t *year, uint8_t *weekDay);

/**
 * @brief   Interface to read the current alarm
 * 
 * @param rtcc Pointer to the RTCC clock structure
 * @param hour Pointer to store the current alarm hour value
 * @param minutes Pointer to store the current alarm minute value
 * @return uint8_t Status of the alarm read operation (TRUE if successful, FALSE otherwise)
 */
uint8_t AppRtcc_getAlarm(AppRtcc_Clock *rtcc, uint8_t *hour, uint8_t *minutes);

/**
 * @brief   Interface to clear an active current alarm
 * 
 * @param rtcc Pointer to the RTCC clock structure
 */
void AppRtcc_clearAlarm(AppRtcc_Clock *rtcc);

/**
 * @brief   Interface to get the flag that indicates if the alarm is active
 * 
 * @param rtcc Pointer to the RTCC clock structure
 * @return uint8_t Status of the alarm flag (TRUE if active, FALSE otherwise)
 */
uint8_t AppRtcc_getAlarmFlag(AppRtcc_Clock *rtcc);

/**
 * @brief   Interface to Clock Periodic function
 * 
 * @param rtcc Pointer to the RTCC clock structure
 */
void AppRtcc_periodicTask(AppRtcc_Clock *rtcc);

/**
 * @brief   Interface to update Time values
 * 
 * @param rtcc Pointer to the RTCC clock structure
 * @return uint8_t Status of the time update operation (TRUE if successful, FALSE otherwise)
 */
uint8_t AppRtcc_updateTime(AppRtcc_Clock *rtcc);

/**
 * @brief   Interface to update Date values
 * 
 * @param rtcc Pointer to the RTCC clock structure
 * @param update_day Day value to update
 */
void AppRtcc_updateDate(AppRtcc_Clock *rtcc, uint8_t update_day);

/**
 * @brief   Interface to reset/set initial values of date and time
 * 
 * @param rtcc Pointer to the RTCC clock structure
 */
void AppRtcc_initDateTime(AppRtcc_Clock *rtcc);

/**
 * @brief   Interface to evaluate if the year is leap or not
 * 
 * @param year Year value to evaluate
 * @return uint8_t Status indicating if the year is leap (TRUE if leap year, FALSE otherwise)
 */
uint8_t AppRtcc_leapYear(uint16_t year);

/**
 * @brief   Interface to set MAX Days for each month
 * 
 * @param rtcc Pointer to the RTCC clock structure
 */
void AppRtcc_maxDaysMonths(AppRtcc_Clock *rtcc);

/**
 * @brief   Interface to give MAX Days depending on the month
 * 
 * @param rtcc Pointer to the RTCC clock structure
 * @param month Month value to evaluate
 * @param leap Status indicating if the year is leap
 * @return uint8_t Maximum days in the specified month
 */
uint8_t AppRtcc_maxDaysMonth(AppRtcc_Clock *rtcc, uint8_t month, uint8_t leap);

/**
 * @brief   Interface to indicate if the Alarm is Active
 * 
 * @param rtcc Pointer to the RTCC clock structure
 */
void AppRtcc_alarmStatus(AppRtcc_Clock *rtcc);


#endif /* CLOCK_CALENDARS_H_ */
