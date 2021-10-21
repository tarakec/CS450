
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
void getDate();

/**
 * Sets the date of the operating system.
 * @param year the year
 * @param month the month
 * @param day the day
 */
void setDate(int year, int month, int day);

/**
 * Sets the time of the operating system
 * @param hours the hour
 * @param minutes the minute
 * @param seconds the second
 */

/*
 * Sets the current year of the operating system.
 * @param year is the year you want to set
 */
void setYear(int year);

/*
 * Gets the current year of the operating system.
 */
int getYear();

/*
 * Sets the current month of the operating system.
 * @param month is the month you want to set
 */
void setMonth(int month);

/*
 * Gets the current month of the operating system.
 */
int getMonth();

/*
 * Sets the current day of the operating system.
 * @param day is the day you want to set
 */
void setDay(int day);

/*
 * Gets the current day of the operating system.
 */
int getDay();


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
 * SHows all the commands a user can do with a description of each function
 */
void help();

/**
 Yields the current running process.
 */

#endif
