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

