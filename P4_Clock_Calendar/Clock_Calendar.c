/**
 * \file       Clock_Calendar.c
 * \author     Jennifer Reynaga
 * \brief      Implementation for Clock_Calendar
 */

/*----------------------------------------------------------------------------*/
/*                                 Includes                                   */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Scheduler.h"
#include "Clock_Calendar.h"

/*----------------------------------------------------------------------------*/
/*                               Local defines                                */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                       Declaration of Global Variables                      */
/*----------------------------------------------------------------------------*/
AppRtcc_Clock clock_rtcc;

/*----------------------------------------------------------------------------*/
/*                       Declaration of local functions                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                     Implementation of global functions                     */
/*----------------------------------------------------------------------------*/

void AppRtcc_clockInit(AppRtcc_Clock *rtcc)
{
    /* Initialize all the clock control structure to valid values */
    /* Initialize Time and Date */
    AppRtcc_initDateTime(rtcc);
    
    /* Set maximum days for each month */
    uint8_t mt_days_max[13u] = {28, 29, 31, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    memcpy(rtcc->mt_days, mt_days_max, sizeof(mt_days_max));
    
    /* Initialize Alarm */
    rtcc->al_min = DEFAULT_VAL;
    rtcc->al_hour = DEFAULT_VAL;
    
    /* Enable the clock bits */
    rtcc->ctrl.bits.clk_en = ENABLE;
    
    /* Disable the alarm control bits */
    rtcc->ctrl.bits.al_set = DISABLE;
    rtcc->ctrl.bits.al_active = DISABLE;
}

void AppRtcc_initDateTime(AppRtcc_Clock *rtcc)
{
    rtcc->tm_sec = DEFAULT_VAL;
    rtcc->tm_min = DEFAULT_VAL;
    rtcc->tm_hour = DEFAULT_VAL;
    rtcc->tm_mday = MIN_DAY;
    rtcc->tm_mon = MIN_MONTH;
    rtcc->tm_year = MIN_YEAR;
    rtcc->tm_wday = RESET_WDAY;
}

uint8_t AppRtcc_setTime(AppRtcc_Clock *rtcc, uint8_t hour, uint8_t minutes, uint8_t seconds)
{
    uint8_t set_time_status;
    if ((hour >= MIN_TIME && hour <= MAX_HOUR) && (minutes >= MIN_TIME && minutes <= MAX_MIN) && (seconds >= MIN_TIME && seconds <= MAX_SEC))
    {
        /* Parameters Validated */
        set_time_status = TRUE;
        rtcc->tm_hour = hour;
        rtcc->tm_min = minutes;
        rtcc->tm_sec = seconds;
    }
    else
    {
        /* Parameters NOT Validated */
        set_time_status = FALSE;
    }

    return set_time_status;
}

uint8_t AppRtcc_setDate(AppRtcc_Clock *rtcc, uint8_t day, uint8_t month, uint16_t year)
{
    uint8_t set_date_status;
    if ((day >= MIN_DAY && day <= MAX_DAY) && (month >= MIN_MONTH && month <= MAX_MONTH) && (year >= MIN_YEAR && year <= MAX_YEAR))
    {
        /* Parameters Validated */
        set_date_status = TRUE;
        rtcc->tm_mday = day;
        rtcc->tm_mon = month;
        rtcc->tm_year = year;
        
        /* Calculate the day of the week using Zeller's Congruence formula */
        static int t[MONTH_ARRAY_SIZE] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
        year -= IS_JANUARY_OR_FEBRUARY(month);
        rtcc->tm_wday = ZELLER_CONGRUENCE(year, month, day, t);
    }
    else
    {
        /* Parameters NOT Validated */
        set_date_status = FALSE;
    }

    return set_date_status;
}

uint8_t AppRtcc_setAlarm(AppRtcc_Clock *rtcc, uint8_t hour, uint8_t minutes)
{
    uint8_t set_alarm_status;

    if ((hour >= MIN_TIME && hour <= MAX_HOUR) && (minutes >= MIN_TIME && minutes <= MAX_MIN))
    {
        /* Parameters Validated */
        set_alarm_status = TRUE;
        rtcc->al_hour = hour;
        rtcc->al_min = minutes;
        rtcc->ctrl.bits.al_set = ENABLE;
    }
    else
    {
        /* Parameters NOT Validated */
        set_alarm_status = FALSE;
    }

    return set_alarm_status;
}

void AppRtcc_getTime(AppRtcc_Clock *rtcc, uint8_t *hour, uint8_t *minutes, uint8_t *seconds)
{
    /* Returns by reference the current values */
    *hour = rtcc->tm_hour;
    *minutes = rtcc->tm_min;
    *seconds = rtcc->tm_sec;
}

void AppRtcc_getDate(AppRtcc_Clock *rtcc, uint8_t *day, uint8_t *month, uint16_t *year, uint8_t *weekDay)
{
    /* Returns by reference the current values */
    *day = rtcc->tm_mday;
    *month = rtcc->tm_mon;
    *year = rtcc->tm_year;
    *weekDay = rtcc->tm_wday;
}

uint8_t AppRtcc_getAlarm(AppRtcc_Clock *rtcc, uint8_t *hour, uint8_t *minutes)
{
    /* Returns by reference the current values */
    *hour = rtcc->al_hour;
    *minutes = rtcc->al_min;
    return rtcc->ctrl.bits.al_set;
}

void AppRtcc_clearAlarm(AppRtcc_Clock *rtcc)
{
    /* Clear the active flag */
    rtcc->ctrl.bits.al_active = DISABLE;
    /* Disable the alarm */
    rtcc->ctrl.bits.al_set = DISABLE;
}

uint8_t AppRtcc_getAlarmFlag(AppRtcc_Clock *rtcc)
{
    return rtcc->ctrl.bits.al_active;
}



void AppRtcc_periodicTask(AppRtcc_Clock *rtcc)
{
    /* Increment time and check if a day needs to be updated */
    uint8_t update_day = AppRtcc_updateTime(rtcc);

    /* Increment date if necessary */
    AppRtcc_updateDate(rtcc, update_day);
    
    /* Check and update alarm status */
    AppRtcc_alarmStatus(rtcc);
}

void AppRtcc_alarmStatus(AppRtcc_Clock *rtcc)
{
    /* Get current time */
    uint8_t get_hour, get_minutes, get_seconds;
    AppRtcc_getTime(rtcc, &get_hour, &get_minutes, &get_seconds);

    /* Get current alarm settings */
    uint8_t get_al_hour, get_al_minutes, get_alarm_status;
    get_alarm_status = AppRtcc_getAlarm(rtcc, &get_al_hour, &get_al_minutes);

    /* Evaluate if the alarm should be activated */
    if ((get_hour == get_al_hour) && (get_minutes == get_al_minutes) && (get_alarm_status == ENABLE))
    {
        /* Activate alarm */
        rtcc->ctrl.bits.al_active = ENABLE;
    }
    else
    {
        /* Keep waiting for the alarm to trigger */
    }
}

uint8_t AppRtcc_updateTime(AppRtcc_Clock *rtcc)
{
    uint8_t update_day_status = FALSE; /* Indicates if a day should be added */

    /* Get current time */
    uint8_t get_hour, get_minutes, get_seconds;
    AppRtcc_getTime(rtcc, &get_hour, &get_minutes, &get_seconds);

    /* Increment seconds */
    if (get_seconds + SECOND > MAX_SEC)
    {
        /* Reset seconds and increment minutes */
        rtcc->tm_sec = MIN_TIME;

        if (get_minutes + MINUTE <= MAX_MIN)
        {
            /* Increment minutes */
            rtcc->tm_min = get_minutes + MINUTE;
        }
        else
        {
            /* Reset minutes and increment hours */
            rtcc->tm_min = MIN_TIME;

            if (get_hour + HOUR <= MAX_HOUR)
            {
                /* Increment hours */
                rtcc->tm_hour = get_hour + HOUR;
            }
            else
            {
                /* Reset hours and indicate that a day should be added */
                rtcc->tm_hour = MIN_TIME;
                update_day_status = TRUE;
            }
        }
    }
    else
    {
        /* Increment seconds */
        rtcc->tm_sec += SECOND;
    }

    return update_day_status;
}

void AppRtcc_updateDate(AppRtcc_Clock *rtcc, uint8_t update_day)
{
    /* Get current date */
    uint8_t get_day, get_month, get_weekDay;
    uint16_t get_year;
    AppRtcc_getDate(rtcc, &get_day, &get_month, &get_year, &get_weekDay);

    /* Determine if the current year is a leap year */
    uint8_t leap = AppRtcc_leapYear(get_year);

    /* Get the maximum number of days in the current month */
    uint8_t max_days_month = AppRtcc_maxDaysMonth(rtcc, get_month, leap);

    /* Increment day */
    if (get_day + update_day > max_days_month)
    {
        /* Reset day and increment month */
        rtcc->tm_mday = MIN_DAY;

        if (get_month + MONTH <= MAX_MONTH)
        {
            /* Increment month */
            rtcc->tm_mon = get_month + MONTH;
        }
        else
        {
            /* Reset month and increment year */
            rtcc->tm_mon = MIN_MONTH;

            if (get_year + YEAR <= MAX_YEAR)
            {
                /* Increment year */
                rtcc->tm_year = get_year + YEAR;
            }
            else
            {
                /* Reset date and time values */
                AppRtcc_initDateTime(rtcc);
            }
        }
    }
    else
    {
        /* Increment day */
        rtcc->tm_mday += update_day;
    }

    /* Increment day of the week */
    if (get_weekDay + update_day < MAX_WDAY)
    {
        /* Increment weekday */
        rtcc->tm_wday += update_day;
    }
    else
    {
        /* Reset weekday */
        rtcc->tm_wday = MIN_WDAY;
    }
}

uint8_t AppRtcc_leapYear(uint16_t year)
{
    uint8_t leap_result;

    if ((DIVISIBLE_BY_4(year) && !DIVISIBLE_BY_100(year)) || DIVISIBLE_BY_400(year))
    {
        leap_result = TRUE;
    }
    else
    {
        leap_result = FALSE;
    }

    return leap_result;
}

uint8_t AppRtcc_maxDaysMonth(AppRtcc_Clock *rtcc, uint8_t month, uint8_t leap)
{
    uint8_t max_days_month;

    if (month == JANUARY)
    {
        /* Max days is the position 2 of the array */
        max_days_month = rtcc->mt_days[SECOND_POSITION];
    }
    else if (month == FEBRUARY)
    {
        /* Max days depends on whether it's a leap year */
        max_days_month = rtcc->mt_days[leap];
    }
    else
    {
        /* Max days for other months */
        max_days_month = rtcc->mt_days[month];
    }

    return max_days_month;
}


/*----------------------------------------------------------------------------*/
/*                             END OF FILE                                    */
/*----------------------------------------------------------------------------*/
