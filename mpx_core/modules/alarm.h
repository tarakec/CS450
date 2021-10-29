#ifndef _ALARM_H
#define _ALARM_H
/**
 *Initializes the process for the alarm process.
 */
void initAlarm();

/**
 Sets the alarm.
 @param message the message user enters.
 @param hours the hours needed for timer.
 @param minutes the minutes needed for timer.
 @param seconds the seconds needed for timer.
 */
void setAlarm();

/**
 * Checks against the the list of times and runs the appropriate process
 */
void checkAlarm();

/**
 * Deletes the appropriate alarm.
 * @param id the alarm selected for deletion.
 */
void deleteAlarm();





#endif
