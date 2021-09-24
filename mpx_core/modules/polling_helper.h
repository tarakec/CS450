
/** file polling_helper.h
 * Aids the polling function in processing the user input.
 */

#ifndef _POLLING_HELPER_H
#define _POLLING_HELPER_H

#define TAB 9
#define RETURN 13
#define UP 65
#define DOWN 66
#define RIGHT 67
#define LEFT 68
#define BACKSPACE 8
#define MACDELETE 127

#define PROMPT "\x1b[32m" "Your Choice: " "\x1b[0m"


/**
 * special_keys deals with keystrokes to the terminal and processing them. It deals with the special keys, tab, return
 * up, down, right, left, backspace, and delete. It also deals with normal printable characters. 
 * @param buffer, reads in the user input
 * @param count, the size of the buffer
 * @param letter, the keystroke that was processed by the polling function
 * @param sizePtr, the actual size of the command in the terminal
 * @param cursorPtr, where the cursor is in the terminal
 */
int special_keys(char* buffer, int* count, char letter, int* sizePtr, int* cursorPtr);


/**
 * Enables the user to delete/backspace in the terminal while removing it from the buffer.
 * @param bugger, collects the user input
 * @param count, the size of the buffer
 * @param sizePtr, the actual size of the command in the terminal
 * @param cursorPtr, where the cursor is in the terminal
 */

void backspace(char* buffer, int *count, int* sizePtr, int *cursorPtr);

void deleteKey(char* buffer, int *count, int* sizePtr, int *cursorPtr);

void print_buffer(char* buffer, int* cursorPtr);

#endif
