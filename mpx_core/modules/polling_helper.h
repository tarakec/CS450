

#ifndef _POLLING_HELPER_H
#define _POLLING_HELPER_H

#define RETURN 13
#define UP 65
#define DOWN 66
#define RIGHT 67
#define LEFT 68
#define BACKSPACE 8
#define DELETE 127

int special_keys(char* buffer, int* count, char letter, int* sizePtr, int* cursorPtr);


void backspace(char* buffer, int *count, int* sizePtr, int *cursorPtr);

#endif
