
#ifndef _COMHAND_H
#define _COMHAND_H



void command_handler();

/**
 * Provides instructions on how to use each command.
 * @param command the current entry in the terminal
 */
void help(char command[]);

/**
 * Called during execuation and enables the operating system to read all of the available commands.
 */

/**
 * Retrieves the current date of the operating system.
 */
void getdate();

/**
 * Sets the date of the operating system.
 * @param year the year
 * @param month the month
 * @param day the day
 */
void setdate(int year, int month, int day);

/**
 * Sets the time of the operating system
 * @param hours the hour
 * @param minutes the minute
 * @param seconds the second
 */
void settime(int hours, int minutes, int seconds);

/**
 * Retrieves the current time of the operating system.
 */
void gettime();

/**
 * Retrieves the current year of the operating system.
 */

int gethours();

/**
 * Sets the current hour of the operating system.
 * @param hour the hour you want to set
 */
void sethours(int hour);

/**
 * Gets the current minute of the operating system.
 */
int getmins();

/**
 * Sets the current minute of the operating system.
 * @param min minute you want to set
 */
void setmin(int min);

/**
 * Gets the current second of the operating system.
 */
int getseconds();

/**
 * Sets the current second of the operating system.
 * @param seconds the second you want to set
 */
void setsec(int seconds);

/**
 * Binary coded digit converter. Converts the time to the BCD format.
 * @param time time of the operating system
 */

char *itoa(int number,char buffer[]);

/**
 * Reverses a character array.
 * @param input the character array that will be reversed
 */
void reverse(char input[]);

/**
 Yields the current running process.
 */

#endif

