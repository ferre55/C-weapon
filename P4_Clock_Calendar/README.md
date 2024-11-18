# Clock_Calendar

The Clock_Calendar module provides a comprehensive implementation for managing date and time functionalities, including setting and retrieving the current time and date, handling alarms, and performing periodic updates. This module is designed to be integrated with a scheduler to ensure accurate timekeeping and alarm management.

## Characteristics
- **Time Management**: Set and retrieve the current time.
- **Date Management**: Set and retrieve the current date.
- **Alarm Management**: Set, retrieve, and clear alarms.
- **Periodic Updates**: Automatically update time and date periodically.
- **Leap Year Calculation**: Determine if a given year is a leap year.
- **Month Days Calculation**: Get the maximum number of days in a given month.

## How it works?
The Clock_Calendar module uses a structure to manage the clock and calendar functionalities. Each instance of the clock structure holds the current time, date, alarm settings, and control flags. The module provides functions to set and get these values, as well as to perform periodic updates and check alarm statuses.

## Functions

1. **AppRtcc_clockInit**: Initializes the internal clock structure with default values.
2. **AppRtcc_setTime**: Sets a new time.
3. **AppRtcc_setDate**: Sets a new date.
4. **AppRtcc_setAlarm**: Sets a new alarm value.
5. **AppRtcc_getTime**: Gets the current time.
6. **AppRtcc_getDate**: Gets the current date.
7. **AppRtcc_getAlarm**: Gets the current alarm values.
8. **AppRtcc_clearAlarm**: Clears an active alarm.
9. **AppRtcc_getAlarmFlag**: Gets the flag indicating if the alarm is active.
10. **AppRtcc_periodicTask**: Performs periodic updates to the time and date, and checks the alarm status.
11. **AppRtcc_updateTime**: Increments the time and checks if a day needs to be updated.
12. **AppRtcc_updateDate**: Increments the date if necessary.
13. **AppRtcc_leapYear**: Determines if a given year is a leap year.
14. **AppRtcc_maxDaysMonth**: Gets the maximum number of days in a given month.


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
6. [Clock_Calendar.c](Clock_Calendar.c): The implementation source file that contains:
- Function implementations
- Required library includes
7. [Clock_Calendar.h](Clock_Calendar.h):The header file that contains:
- Structure definitions
- Function declarations (prototypes)
- Constant definitions
- Include guards

# Clock_Calendar.h

## Data structure definition

### Clock Control Union

The control union structure handles various flags, each represented by a single bit.

```c
typedef union _AppRtcc_Ctrl
{
    uint8_t Register;
    struct _bits
    {
        uint8_t clk_en : 1;
        uint8_t al_set : 1;
        uint8_t al_active : 1;
        uint8_t none : 4;
    } bits;
} AppRtcc_Ctrl;
```

### Clock Structure
The main clock structure holds all the time and date information, as well as alarm settings and control flags.

```c
typedef struct _AppRtcc_Clock 
{
    uint8_t tm_sec;
    uint8_t tm_min;
    uint8_t tm_hour;
    uint8_t tm_mday;
    uint8_t tm_mon;
    uint16_t tm_year;
    uint8_t tm_wday;
    uint8_t al_min;
    uint8_t al_hour;
    uint8_t mt_days[13u];
    AppRtcc_Ctrl ctrl;
} AppRtcc_Clock;
```

# Clock_Calendar.c

## Initialization
### AppRtcc_clockInit
Initializes the internal clock structure with default values.

```c
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
```

### AppRtcc_initDateTime
Initializes the date and time to default values.

```c
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
```

## Setting Time and Date
### AppRtcc_setTime
Sets a new time.

```c
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
```

### AppRtcc_setDate
Sets a new date.

```c
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
```

## Getting Time and Date
### AppRtcc_getTime
Gets the current time.

```c
void AppRtcc_getTime(AppRtcc_Clock *rtcc, uint8_t *hour, uint8_t *minutes, uint8_t *seconds)
{
    /* Returns by reference the current values */
    *hour = rtcc->tm_hour;
    *minutes = rtcc->tm_min;
    *seconds = rtcc->tm_sec;
}
```

### AppRtcc_getDate
Gets the current date.

```c
void AppRtcc_getDate(AppRtcc_Clock *rtcc, uint8_t *day, uint8_t *month, uint16_t *year, uint8_t *weekDay)
{
    /* Returns by reference the current values */
    *day = rtcc->tm_mday;
    *month = rtcc->tm_mon;
    *year = rtcc->tm_year;
    *weekDay = rtcc->tm_wday;
}
```


## Alarm Management Functions

### AppRtcc_getAlarm
Gets the current alarm values.

```c
uint8_t AppRtcc_getAlarm(AppRtcc_Clock *rtcc, uint8_t *hour, uint8_t *minutes)
{
    /* Returns by reference the current values */
    *hour = rtcc->al_hour;
    *minutes = rtcc->al_min;
    return rtcc->ctrl.bits.al_set;
}
```

### AppRtcc_clearAlarm
Clears an active alarm.

```c
void AppRtcc_clearAlarm(AppRtcc_Clock *rtcc)
{
    /* Clear the active flag */
    rtcc->ctrl.bits.al_active = DISABLE;
    /* Disable the alarm */
    rtcc->ctrl.bits.al_set = DISABLE;
}
```

### Get Alarm Flag
Gets the flag indicating if the alarm is active

```c
uint8_t AppRtcc_getAlarmFlag(AppRtcc_Clock *rtcc)
{
    return rtcc->ctrl.bits.al_active;
}
```

## Periodic Update Functions

### Periodic Task
Performs periodic updates to the time and date, and checks the alarm status.

```c
void AppRtcc_periodicTask(AppRtcc_Clock *rtcc)
{
    /* Increment time and check if a day needs to be updated */
    uint8_t update_day = AppRtcc_updateTime(rtcc);

    /* Increment date if necessary */
    AppRtcc_updateDate(rtcc, update_day);
    
    /* Check and update alarm status */
    AppRtcc_alarmStatus(rtcc);
}
```

### AppRtcc_alarmStatus
Checks and updates the alarm status.

```c
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
```

## Time and Date Update Functions

### AppRtcc_updateTime
Increments the time and checks if a day needs to be updated.

```c
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
```

### AppRtcc_updateDate
Increments the date if necessary.

```c
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
```

## Leap Year Calculation
Determines if a year is a leap year.

```c
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
```
