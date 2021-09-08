
#ifndef _COMHAND_H
#define _COMHAND_H



void command_handler();

void help();

int shutdown();

void version();

/**
 * Prints error message when invcalid command is entered
 */
void error();


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


/**
 * Sets the time of the operating system
 * @param hours the hour
 * @param minutes the minute
 * @param seconds the second
 */
void setTime(int hours, int minutes, int seconds);

/**
 * Retrieves the current time of the operating system.
 */
void getTime();

/**
 * Retrieves the current year of the operating system.
 */

int getHours();

/**
 * Sets the current hour of the operating system.
 * @param hour the hour you want to set
 */
void setHours(int hour);

/**
 * Gets the current minute of the operating system.
 */
int getMins();

/**
 * Sets the current minute of the operating system.
 * @param min minute you want to set
 */
void setMin(int min);

/**
 * Gets the current second of the operating system.
 */
int getSeconds();

/**
 * Sets the current second of the operating system.
 * @param seconds the second you want to set
 */
void setSec(int seconds);

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

