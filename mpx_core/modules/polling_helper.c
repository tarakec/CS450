#include "polling_helper.h"
#include <../include/core/serial.h>
#include "mpx_supt.h"
#include <stdint.h>
#include <string.h>



int special_keys(char* buffer, int* count, char letter, int* sizePtr, int* cursorPtr){


	int promptLen = strlen(PROMPT);

	//utilization of a switch statement
	//depending on the value of the letter from I/O
	switch (letter){
		case TAB: ; 
		int j = 0;

		while(j < 8){
			buffer[*cursorPtr] = 0;
			(*sizePtr)++;
			(*count)++;
			(*cursorPtr)++;
			j++;
		}

		serial_print("\033[1000D");
		serial_print("\033[0K");
		sys_req(WRITE,DEFAULT_DEVICE,PROMPT,&promptLen);
		serial_print(buffer);
		serial_print("\033[1000D");

		//replacing the cursor back to its position
			int i = 0;
			while (i < *cursorPtr){
					serial_print("\033[C");
					i++;
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

				//if the cursor is at the end of the size, simply add the letter
				if(*cursorPtr == *sizePtr){
					buffer[*cursorPtr] = letter;
				}

				//the other case is the curso is in the middle somewhere
				//if that is the case, shift the buffer accordingly and insert
				else{

					int a  = *sizePtr;

					while( a >= *cursorPtr){
						buffer[a] = buffer[a-1];
						a--;
				}

					buffer[*cursorPtr] = letter;
				}

				

				//printing of the buffer				
				serial_print("\033[1000D");
				serial_print("\033[0K");
				sys_req(WRITE,DEFAULT_DEVICE,PROMPT,&promptLen);
				serial_print(buffer);
				serial_print("\033[1000D");

				int i = 0;

				//increment variables
				(*count)++;
				(*sizePtr)++;
				(*cursorPtr)++;

				//getting the cursor back to its position
				while (i < (*cursorPtr + 13)){
					serial_print("\033[C");
					i++;
				}

			}
	}
	return 0;
}


void backspace(char *buffer, int *count, int *sizePtr, int *cursorPtr) {
	int promptLen = strlen(PROMPT);


	//first make sure that the size is greater than 0 and that the cursor isn't at the 0 position.
	if (*sizePtr > 0 && *cursorPtr > 0){

			

				//if the cursor is somewhere in the middle, then remove the char and shift
				int a = *cursorPtr - 1;

				buffer[*cursorPtr - 1]= 0;

				while(a < *sizePtr){
					buffer[a] = buffer[a + 1];
					a++;
				}


			//decrement
			(*count)--;
			(*sizePtr)--;
			(*cursorPtr)--;


			//printing of the buffer
			serial_print("\033[1000D");
			serial_print("\033[0K");
			sys_req(WRITE,DEFAULT_DEVICE,PROMPT,&promptLen);
			serial_print(buffer);
			serial_print("\033[1000D");

			//replacing the cursor back to its position
			int i = 0;
			while (i < (*cursorPtr + 13)){
					serial_print("\033[C");
					i++;
				}


	}

}
