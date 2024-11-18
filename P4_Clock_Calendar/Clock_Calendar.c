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


/*----------------------------------------------------------------------------*/
/*                       Declaration of local functions                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                     Implementation of global functions                     */
/*----------------------------------------------------------------------------*/

void AppRtcc_clockInit( AppRtcc_Clock *rtcc )
{
    /* Initialize  all the clock control structure to valid values*/
    /* Init Time and Date */
    AppRtcc_initDateTime(rtcc);
    /* Set each MAX DAYS for each month*/
    uint8_t mt_days_max[13u] = {28,29,31,31,30,31,30,31,31,30,31,30,31};
    memcpy(rtcc -> mt_days, mt_days_max, sizeof(mt_days_max));
    //AppRtcc_maxDaysMonths(rtcc);
    
    /* Init Alarm */
    rtcc -> al_min = DEFAULT_VAL;
    rtcc -> al_hour = DEFAULT_VAL;
    /* Enable the clock bits */
    rtcc -> ctrl.bits.clk_en = ENABLE;
    /* Disable the alarm control bits*/
    rtcc -> ctrl.bits.al_set = DISABLE;
    rtcc -> ctrl.bits.al_active = DISABLE;
}

/**
 * @brief   Interface to reset/set initial values of date and Time
*/
void AppRtcc_initDateTime( AppRtcc_Clock *rtcc)
{
    rtcc -> tm_sec = DEFAULT_VAL;
    rtcc -> tm_min = DEFAULT_VAL;
    rtcc -> tm_hour = DEFAULT_VAL;
    rtcc -> tm_mday = MIN_DAY;
    rtcc -> tm_mon = MIN_MONTH;
    rtcc -> tm_year = MIN_YEAR;
    rtcc -> tm_wday = RESET_WDAY;   
}


/**
 * @brief   Interface to set a new time
*/
/*The function will receive the hour, minutes and seconds 
in 24 hour format, the values should be validated if they make
a valid hour, in case true the new values shall be assigned to 
the corresponding internal clock structure, Function shall return 
a TRUE is the new time could be set otherwise returns FALSE
*/
uint8_t AppRtcc_setTime( AppRtcc_Clock *rtcc, uint8_t hour, uint8_t minutes, uint8_t seconds )
{
    uint8_t set_time_status;
    if((hour >= MIN_TIME && hour <= MAX_HOUR) && (minutes >= MIN_TIME && minutes <= MAX_MIN) && (seconds >= MIN_TIME && minutes <= MAX_SEC) )
    { 
        /* Parameters Validated */
        set_time_status = TRUE;
        rtcc -> tm_hour = hour;
        rtcc -> tm_min = minutes;
        rtcc -> tm_sec = seconds;
    }
    else
    {
        /* Parameters NOT Validated */
        set_time_status = FALSE;
    }

    return set_time_status;
}


/**
 * @brief   Interface to set a new date
*/
/* The function will receive the day of the month, month of the year 
and the years in four digits, the values should be validated if they 
make a valid date, in case true the new values shall be assigned to 
the corresponding internal clock structure, plus, the function shall 
calculate the day of the week and assigned to the corresponding clock 
structure element. Function shall return a TRUE is the new time could 
be set otherwise returns FALSE
*/
uint8_t AppRtcc_setDate( AppRtcc_Clock *rtcc, uint8_t day, uint8_t month, uint16_t year )
{
    uint8_t set_date_status;
    if((day >= MIN_DAY && day <= MAX_DAY) && (month >= MIN_MONTH && month <= MAX_MONTH) && (year >= MIN_YEAR && year <= MAX_YEAR) )
    { 
        /* Parameters Validated */
        set_date_status = TRUE;
        rtcc -> tm_mday = day;
        rtcc -> tm_mon = month;
        rtcc -> tm_year = year;
        
        /* Calculate the day of the week */
        /* Magic Number Month array.*/
        static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
        year -= month < 3;
        /* Weekday using Zeller’s Congruence formula*/
        rtcc -> tm_wday = (year + year / 4 - year / 100 + year / 400 + t[month - 1] + day)% 7;
    }
    else
    {
        /* Parameters NOT Validated */
        set_date_status = FALSE;
    }

    return set_date_status;
}


/**
 * @brief   Interface to set a new alarm value
*/
/* The function will receive the hour and minutes in 24 hour format, the values should
be validated id they make a valid time, in case true the new values shall be assigned
to the corresponding alarm internal clock structure, and set the flag  al_set which 
indicate a alarm will be activated as soon as the time match the alarm value. 
Function shall return a TRUE is the new time could be set otherwise returns FALSE
*/
uint8_t AppRtcc_setAlarm( AppRtcc_Clock *rtcc, uint8_t hour, uint8_t minutes )
{

    uint8_t set_alarm_status;

    if((hour >= MIN_TIME && hour <= MAX_HOUR) && (minutes >= MIN_TIME && minutes <= MAX_MIN))
    { 
        /* Parameters Validated */
        set_alarm_status = TRUE;
        rtcc -> al_hour = hour;
        rtcc -> al_min = minutes;
        rtcc -> ctrl.bits.al_set = ENABLE;
    }
    else
    {
        /* Parameters NOT Validated */
        set_alarm_status = FALSE;
    }

    return set_alarm_status;
}


/**
 * @brief   Interface to read the current time
*/
/*  The function returns by reference the current time values, hour, minutes and seconds,
all the values will be taken from the clock control structure
*/
void AppRtcc_getTime( AppRtcc_Clock *rtcc, uint8_t *hour, uint8_t *minutes, uint8_t *seconds )
{
    /* Returns by reference the current values */
    *hour = rtcc -> tm_hour;
    *minutes = rtcc -> tm_min;
    *seconds  = rtcc -> tm_sec;
}


/**
 * @brief    Interface to read the current date
*/
/*  The function returns by reference the current date values, 
day of the month, month, year and day of the week, all the values will be taken from the clock control structure
*/
void AppRtcc_getDate( AppRtcc_Clock *rtcc, uint8_t *day, uint8_t *month, uint16_t *year, uint8_t *weekDay )
{
    /* Returns by reference the current values */
    *day = rtcc -> tm_mday;
    *month = rtcc -> tm_mon;
    *year = rtcc -> tm_year;
    *weekDay = rtcc -> tm_wday;
}


/**
 * @brief   Interface to read the current alarm
*/
/*The function returns by reference the current alarm values, hour and minutes, 
plus the the status of the set bit which indicates if the alarm is 
currently ready for activation. All the values will be taken from the clock control structure
*/
uint8_t AppRtcc_getAlarm( AppRtcc_Clock *rtcc, uint8_t *hour, uint8_t *minutes )
{
    /* Returns by reference the current values */
    *hour = rtcc -> al_hour;
    *minutes = rtcc -> al_min;
    return rtcc -> ctrl.bits.al_set;
}


/**
 * @brief   Interface  to clear an active current alarm
*/
/*  Once the alarm is active (time and alarm values are the same) this function will be 
use to clear the active flag and also will disable the alarm, been necessary to call 
the AppClk_setAlarm function again to set the alarm*/
void AppRtcc_clearAlarm( AppRtcc_Clock *rtcc )
{
        /* Clear the active flag */
        rtcc -> ctrl.bits.al_active = DISABLE;
        /* Disable the alarm */ 
        rtcc -> ctrl.bits.al_set = DISABLE;
}


/**
 * @brief   Interface to get the flag that indicates if the alarm is active
*/
/* The function will return the alarm active bit */
uint8_t AppRtcc_getAlarmFlag( AppRtcc_Clock *rtcc )
{
    return rtcc->ctrl.bits.al_active;
}


/**
 * @brief   Interface to Clock Periodic function
*/
/*  This is the main function in charge of increment the time and date values based on any
regular clock functionality, besides of incrementing time and date it will be in charge
of supervise when an alarm is set and should be activated when time and minutes match. 
function most take care of also handle the increment of the day 
of the week accordingly and the leap year when necessary. 
It is necessary this function been called every one second using any
other external mechanism like a timer, software timer or an interrupt when available. */
void AppRtcc_periodicTask( AppRtcc_Clock *rtcc )
{       
    /* Increment time */
    uint8_t update_day = AppRtcc_updateTime(rtcc);

    /* Increment Date */
    AppRtcc_updateDate(rtcc, update_day);
    
    /* Supervise when an alarm is Enable when time and minutes match */
    AppRtcc_alarmStatus(rtcc);

}

/**
 * @brief   Interface to indicate if the Alarm is Active
*/
void AppRtcc_alarmStatus( AppRtcc_Clock *rtcc)
{
    
    /* Get Time */
    uint8_t get_hour, get_minutes, get_seconds;
    AppRtcc_getTime(rtcc, &get_hour, &get_minutes, &get_seconds);

    /* Get Alarm */
    uint8_t get_al_hour, get_al_minutes, get_alarm_status; /* Variables to store Alarm values*/
    get_alarm_status = AppRtcc_getAlarm( rtcc, &get_al_hour, &get_al_minutes );

    /* Alarm evaluation */
    if( (get_hour == get_al_hour) && (get_minutes == get_al_minutes) && (get_alarm_status == ENABLE))
    {
        /* Activate ALARM*/
        rtcc->ctrl.bits.al_active = ENABLE;
    }
    else
    {
        /* Keep waiting alarm */
    }

}

/**
 * @brief   Interface to update Time values
*/
uint8_t AppRtcc_updateTime( AppRtcc_Clock *rtcc)
{
    uint8_t update_day_status = FALSE; /* Return that a day not should be added*/

    /* GetTime */
    uint8_t get_hour, get_minutes, get_seconds;  /* Variables to store Time values*/
    AppRtcc_getTime(rtcc, &get_hour, &get_minutes, &get_seconds);

    /* Time increments */
    /* Seconds */
    if (get_seconds + SECOND > MAX_SEC) 
    {
        /* MAX_SEC exceed */
        rtcc -> tm_sec = MIN_TIME; /* Restart seconds */
        
        /* Minutes Increment */
        if(get_minutes + MINUTE <= MAX_MIN)
        {
            /* Minutes is not greater than MAX_MIN */
            rtcc -> tm_min = get_minutes + MINUTE; /* Add Minute*/
        }
        else
        {
            /* Minutes is greater than MAX_MIN */
            rtcc -> tm_min = MIN_TIME; /* Restart Minutes */

            /*Hour increment*/
            if(get_hour + HOUR <= MAX_HOUR)
            {
                rtcc -> tm_hour = get_hour + HOUR; /* Add HOUR*/
            }
            else
            {
                rtcc -> tm_hour = MIN_TIME; /* Restart HOURS */
                update_day_status = TRUE;   /* Return that a day should be added*/
            }
        }
    }
    else
    {
        rtcc -> tm_sec = rtcc -> tm_sec + SECOND; /* Add the new second :)*/
    }

    return update_day_status;

}


void AppRtcc_updateDate( AppRtcc_Clock *rtcc, uint8_t update_day)
{
    /* Get Date */
    uint8_t get_day, get_month, get_weekDay; /* Variables to store Date values*/
    uint16_t get_year;

    AppRtcc_getDate( rtcc, &get_day, &get_month, &get_year, &get_weekDay );

    uint8_t leap = AppRtcc_leapYear( get_year); /* Store if year is Leap or not */

    uint8_t max_days_month = AppRtcc_maxDaysMonth( rtcc, get_month,leap); /* Store max of days for the month */

    /*Evaluate date :) */
    if (get_day + update_day > max_days_month) 
    {
        rtcc -> tm_mday = MIN_DAY; /* Restart day */
        
        /* Month Increment */
        if(get_month + MONTH  <= MAX_MONTH) /* If the evaluation is less or equal Max */
        {
            /*Month is not MAX_MONTH*/
            rtcc -> tm_mon = get_month + MONTH; /* Add MONTH */
        }
        else
        {
            /*Month is MAX_MONTH*/
            rtcc -> tm_mon = MIN_MONTH; /* Restart Month*/

            /*Year increment*/
            if(get_year + YEAR <= MAX_YEAR)
            {
                rtcc -> tm_year = get_year + YEAR; /* Add YEAR */
            }
            else
            {
                AppRtcc_initDateTime(rtcc); /* Reset some Data-Time values */
            }
        }
    }
    else
    {
        rtcc -> tm_mday = rtcc -> tm_mday + update_day; /* Add the new day :)*/
    }

    /* Increment Day of the Week */
    if( get_weekDay + update_day < MAX_WDAY)
    {
        /*Is smaller than MAX_WDAY*/
        rtcc -> tm_wday = rtcc -> tm_wday + update_day; /* Increase wday*/
    }
    else
    {
        /*Is greater than MAX_WDAY*/
        rtcc -> tm_wday = MIN_WDAY; /* Reset wday*/
    }

}

/**
 * @brief   Interface to evaluate if the year is leap or not
*/
uint8_t AppRtcc_leapYear( uint16_t year)
{
    uint8_t leap_result;
    
    if(((year % 4 == 0) && (year % 100!= 0)) || (year %400 == 0))
    {
        leap_result = TRUE;
    }
    else
    {
        leap_result = FALSE;
    }

    return leap_result;
}

/**
 * @brief   Interface to set MAX Days for each month
*/
void AppRtcc_maxDaysMonths( AppRtcc_Clock *rtcc)
{
    rtcc -> mt_days[0]  = TWENTY_EIGHT;     /* February - NOT LEAP */
    rtcc -> mt_days[1]  = TWENTY_NINE;      /* February - LEAP */
    rtcc -> mt_days[2]  = THIRTY_ONE;       /* January */
    rtcc -> mt_days[3]  = THIRTY_ONE;       /* March */
    rtcc -> mt_days[4]  = THIRTY ;          /* April */
    rtcc -> mt_days[5]  = THIRTY_ONE;       /* May */
    rtcc -> mt_days[6]  = THIRTY ;          /* June */
    rtcc -> mt_days[7]  = THIRTY_ONE;       /* July */
    rtcc -> mt_days[8]  = THIRTY_ONE;       /* August */
    rtcc -> mt_days[9]  = THIRTY ;          /* September */
    rtcc -> mt_days[10] = THIRTY_ONE;       /* October */
    rtcc -> mt_days[11] = THIRTY ;          /* November */
    rtcc -> mt_days[12] = THIRTY_ONE;       /* December */

}

/**
 * @brief   Interface to set MAX Days depending the month
*/
uint8_t AppRtcc_maxDaysMonth( AppRtcc_Clock *rtcc, uint8_t month, uint8_t leap)
{
    uint8_t max_days_month;

    if( month == JANUARY)
    {
        /* Max days is the position 2 of the array */
        max_days_month = rtcc -> mt_days[2];
    }
    else if(month == FEBRUARY) /* Max Days Depends if is Leap or not*/
    {
        max_days_month = rtcc -> mt_days[leap];
    }
    else
    {
        max_days_month = rtcc -> mt_days[month];
    }

    return max_days_month;
}

/*----------------------------------------------------------------------------*/
/*                             END OF FILE                                    */
/*----------------------------------------------------------------------------*/
