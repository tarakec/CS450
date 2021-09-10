#include "polling_helper.h"
#include <../include/core/serial.h>



int special_keys(char* buffer, int* count, char letter, int* sizePtr, int* cursorPtr){

	//utilization of a switch statement
	//depending on the value of the letter from I/O
	switch (letter){


		//implemented tab, which will count as 8 spaces
		case TAB: ;

			int a;
			int b = 0;

			//still can only be placed at the end of the command
			if (*cursorPtr != *sizePtr){
					a = (*sizePtr) - (*cursorPtr);

					while( b < a){
						serial_print("\033[C"); //right
						(*cursorPtr)++;
						b++;
					}
				}
			 int c = 0;

			 //8 spaces plus the increase in size
			while (c < 8){
				serial_print(" ");
				(*cursorPtr)++;
				(*sizePtr)++;
				(*count)++;
				c++;
			}
			break;

		//carriage return
		case RETURN:
			return -1; //carriage return will break the while loop in the polling function

		case UP:
			break; //nothing, will implement later

		case DOWN:
			break; //nothing, will implement later

		case RIGHT:
			//if the position of the cursor is greater than or equal to the size
			if (*cursorPtr >= *sizePtr){
				break;
			}		
			else{
				serial_print("\033[C"); //move to the right one column
				(*cursorPtr)++; //moving right so increment cursor
				break;
			}
		case LEFT:
			//if the position of the cursor is equal to 0
			if(*cursorPtr <=  0){
				break;
			}
			else{
				serial_print("\033[D"); //move to the left one column
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
			if (*sizePtr > 90){

				//do nothing
				
			}
			else{

				int i;
				int j = 0;

				//only want to add characters to the end of the commands
				// move the cursor to the end of the size before adding the chars
				if (*cursorPtr != *sizePtr){
					i = (*sizePtr) - (*cursorPtr);

					while( j < i){
						serial_print("\033[C"); //right
						(*cursorPtr)++;
						j++;
					}
				}

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

	int k;
	int l=0;


	//first make sure that the size is greater than 0 and that the cursor isn't at the 0 position.
	if (*sizePtr > 0 && *cursorPtr > 0){

	
	//want to make sure that we only delete characters at the end of the command
	//if the cursor is not at the end of the command, then move it to the end before deleting/backspacing
		if (*cursorPtr < *sizePtr){
			k = (*sizePtr) - (*cursorPtr);

			while( l < k){
				serial_print("\033[C");
				(*cursorPtr)++;
				l++;
			}
		}	

		//backspace and decrement 
			buffer[*cursorPtr - 1] = 0;
			(*count)--;
			(*sizePtr)--;
			(*cursorPtr)--;
		

		//backspace 
		serial_print("\b");

		//empty out character
		serial_print(" ");

		//backspace
		serial_print("\b");
	}

	else{

		int m;
		int n = 0;


		//case where the cursor is at the beginning and user attempts to backspace
		if (*cursorPtr == 0 && *sizePtr > 0){
			m = *sizePtr;

			while(n < m){
				serial_print("\033[C"); //will move cursor to the end of the command, then backspace
				(*cursorPtr)++;
				n++;
			}

			//backspace and decrement 
			buffer[*cursorPtr - 1] = 0;
			(*count)--;
			(*sizePtr)--;
			(*cursorPtr)--;

			//backspace 
			serial_print("\b");

			//empty out character
			serial_print(" ");

			//backspace
			serial_print("\b");
		}
	}
}
