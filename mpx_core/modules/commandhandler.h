
/** file commandhandler.h
 * Interprets commands entered by the user and calls the corresponding functions.
 */

#ifndef _COMHAND_H
#define _COMHAND_H

#define F_CYAN "\x1b[36m"
#define F_YELLOW "\x1b[33m"
#define RESET "\x1b[0m"
#define F_RED "\x1b[31m"
#define F_GREEN "\x1b[32m"
#define F_BLUE "\x1b[34m"



/**
 * Interprets commands entered by the user and calls the corresponding functions.
 */
void command_handler();


/**
 * Displays the list of available commands and what they do.
 */
void help();

/**
 * Sends the signal shutdown to the machine.
 */
int shutdown();


/**
 * Displays the current version number and the date it was last updated.
 */
void version();

/**
 * Prints an error message when an invalid command is entered.
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
 * Retrieves the current hour of the operating system.
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
 * Converts an integer to a character string. 
 * @param num, the integer you want to convert to character string. 
 * @param buffer, the string you want to be converted from the integer. 
 */

char *itoa(int num,char buffer[]);

/**
 * Reverses a character array.
 * @param buffer the character array that will be reversed
 */
void reverse(char input[]);


/**
 * Clears the terminal screen.
 */
void clear();


/**
 * Prompts the user with a menu of actions they can perform with the corresponding numbers to call them.
 */
void menu();

/**
 * prompts the user with a menu of actions they can perform during the process management mode.
 */
void PCB_menu();

/**
 * displays the list of available commands in the process management mode and what they do
 */
void PCB_help();

/**
 * sends the signal to exit the process management mode
 */
int PCB_exit();

#endif

