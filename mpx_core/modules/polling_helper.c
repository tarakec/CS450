#include "polling_helper.h"
#include <../include/core/serial.h>



int special_keys(char* buffer, int* count, char letter, int* sizePtr, int* cursorPtr){

	//utilization of a switch statement
	//depending on the value of the letter from I/O
	switch (letter){

		//carriage return
		case RETURN:
			return -1; //carriage return will break the while loop in the polling function

		case UP:
			break; //nothing

		case DOWN:
			break; //nothing

		case RIGHT:
			//if the position of the cursor is greater than or equal to the size
			if (*cursorPtr > 0){
				break;
			}		
			else{
				serial_print("\033[C"); //send to the output the octal escape followed by [C
				(*cursorPtr)++; //moving right so increment cursor
				break;
			}
		case LEFT:
			//if the position of the cursor is less than 0
			if(*cursorPtr <  0){
				break;
			}
			else{
				serial_print("\b"); //send to the output a backspace without removing the from the buffer
				(*cursorPtr)--; //decrement the cursor since moving left
				break;
			}
		case DELETE:
			backspace(buffer, count, sizePtr, cursorPtr); //call backspace function
			break;
			
		case BACKSPACE:
			backspace(buffer, count, sizePtr, cursorPtr);// call backspace function
			break;
			
		//default case is a printable character
		default:

			//preventing buffer overflow
			if (*sizePtr > 95){
				//do nothing
			}
			else{
				//buffer at cursor position is equal to the letter
				buffer[*cursorPtr] = letter;

				//send to the output the letter we just assigned to the buffer at the cursor postion
				serial_print(&buffer[*cursorPtr]);

				//increment variables
				(*count)++;
				(*sizePtr)++;
				(*cursorPtr)++;

			}
	}
	return 0;
}


void backspace(char *buffer, int *count, int *sizePtr, int *cursorPtr) {
	
	//first make sure that the size is greater than 0 and that the cursor isn't at the 0 position.
	if (*sizePtr > 0 && *cursorPtr > 0){
		buffer[*cursorPtr - 1] = 0;
		(*count)--;
		(*sizePtr)--;
		(*cursorPtr)--;
		
		//send to the output a visual backspace 
		serial_print("\b");
		serial_print(" ");
		serial_print("\b");
	}
}
