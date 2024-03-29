
#include <core/serial.h>
#include <core/io.h>
#include <core/tables.h>
#include <string.h>
#include "mpx_R6.h"
#include "polling_helper.h"
#include "pcb_internal.h"
#include "pcb_commands.h"
#include "mpx_supt.h"

u32int dev = COM1;
int level = 4;

dcb *serial_dcb;

u32int original_idt_entry;


//top level handler
void top_handler(){

	if(serial_dcb->port_status == OPEN){
		cli();

		//interrupt id register
		int type = inb(dev+2);

		//skipping bit 0, grabbing bit 1 and 2
		int bit1 = type>>1 & 1;
		int bit2 = type>>2 & 1;

		if(!bit1 && !bit2){
			//modem
			inb(dev + 6);
		}
		else if(bit1 && !bit2){
			//output 
			output_handler();
		}
		else if(!bit1 && bit2){
			//input 
			input_handler();
		}
		else{
			//line
			inb(dev +5 );
		}

		sti();

	}

	outb(0x20,0x20); //clear interrupt
}

//com open function
int com_open(int baud_rate){
	//error checking

	//check baud rate valid
	if(baud_rate <= 0){
		return INVALID_BAUD_DIVISOR;
	}

	//check that port is not already open
	if(serial_dcb->port_status == OPEN){
		return PORT_ALREADY_OPEN;
	}

	//disable maskable ints
	cli();

	//set port status to open
	serial_dcb->port_status = OPEN;

	//set to 1 because not doing operation yet
	serial_dcb->event_flag = SET;

	//set to idle because not reading or writing yet
	serial_dcb->status_code = R6_IDLE;

	//initialize ring buffer
	serial_dcb->ring_input = 0;
	serial_dcb->ring_output = 0;
	serial_dcb->ring_counter = 0;


	original_idt_entry = idt_get_gate(0x24);
	idt_set_gate(0x24,(u32int) top_handler, 0x08,0x8e);

	//baud divisor
	long brd = 115200 / (long)baud_rate;

	//disable interrupts
	outb(dev + 1,0b00000000);

	//set line control register
	outb(dev + 3,0b10000000);

	//Set LSB of baud rate
	outb(dev + 0,brd); 

	//Set MSB of Baud Rate
	outb(dev + 1,brd >> 8);

	//lock divisor 
	outb(dev + 3, 0b00000011);

	//enable FIFO, clear, 14 byte threshold
	outb(dev + 2, 0b11000111);

	//enable PIC level
	outb(0x21, inb(0x21) & ~(1<<level));

	//read interrupt on
	outb(dev + 1, 0b00000001);
	(void) inb(dev);

	outb(dev + 4, 0x0B); 

	//enable maskable ints
	sti();
	return 0;
}

//set the writing interrupt on or off
void set_int(int on){
	if(on){
		
		outb(dev+1, inb(dev+1) | (0x02)); //on
	
	}
	else{
		
		outb(dev+1, inb(dev+1) & ~(0x02)); //
	}
}

//will be our input interrupt handler 
void input_handler(){


	//grab character from COM1
	char i = inb(dev);
	
	//if status is not reading. store into ring buffer
	if(serial_dcb->status_code != R6_READ){


		//if the ring buffer is not full, insert the character into it
		if(serial_dcb->ring_counter < 16){
			serial_dcb->ring_buffer[serial_dcb->ring_counter] = i;
			serial_dcb->ring_counter++;
		}
		else{
			//maybe need to discard character
		}

		return;
	}
	//if the status is reading
	else{
		//concat the letter into the input buffer
		*serial_dcb->input = i;
		serial_dcb->input++;

		//increment the transferred count
		serial_dcb->in_transfer_count++;

		//if count not complete and not a new line
		if(serial_dcb->in_transfer_count < *serial_dcb->input_count && i != '\r'){
			return;
		}
		else{
			//else transfer complete
			serial_dcb->status_code = R6_IDLE;
			serial_dcb->event_flag = SET;
			*serial_dcb->input_count = serial_dcb->in_transfer_count;
			return;
		}
	}
	
}

//second level output handler
void output_handler(){
 

	//if current status is not writing, return
	if(serial_dcb->status_code != R6_WRITE){
		return;
	}	

	//if the transfer count is not yet the expected output count then grab another character and transfer to the output register
	if(serial_dcb->out_transfer_count != *serial_dcb->output_count){
		outb(dev, serial_dcb->output[0]);
		serial_dcb->output++;
		serial_dcb->out_transfer_count++;
		return;
	}
	//if the transfer count is the expeted output count, then SET event flag, set status to idle, turn off the writing ints
	else{
		serial_dcb->status_code = R6_IDLE;
		serial_dcb->event_flag = SET;
		set_int(OFF);
		return;
	}

}

int com_close(){

	cli();

	//make sure port is open
	if(serial_dcb->port_status == CLOSED){
		return PORT_NOT_OPEN;
	}

	//clearing open indicator
	serial_dcb->port_status = CLOSED;

	//turning the writing interrupt off
	set_int(OFF);
	

	//disable PIC_MASK Register
	int mask = inb(PIC_MASK);
	outb(PIC_MASK, mask | (1<<level));


	//load 0's into model and interrupt enable register
	outb(dev + 4, 0b00000000);
	outb(dev + 1, 0b00000000);


	//restore original saved interrupt vector
	//outb(dev, original_idt_entry); 

	sti();

	return 0;
}

int com_read(char *buf_p, int *count_p){
	

	//port not open
	if(serial_dcb->port_status != OPEN){
		return READ_PORT_NOT_OPEN;
	}

	//invalid buffer address
	if(buf_p == NULL){
		return INVALID_BUFFER_ADDRESS;
	}

	//invalid count
	if(count_p == NULL){
		return READ_INVALID_COUNT;
	}

	//device is busy
	if(serial_dcb->status_code != R6_IDLE){
		return DEVICE_BUSY;
	}

	// setting the status code to reading
	serial_dcb->status_code = R6_READ;

	//initialize input buffer variables
	serial_dcb->input = buf_p;
	serial_dcb->input_count = count_p;

	//clear event flag
	serial_dcb->event_flag = CLEARED;


	int i = 0;
	serial_dcb->in_transfer_count = 0;
	while(serial_dcb->ring_buffer[i] != NULL || serial_dcb->in_transfer_count >= *serial_dcb->input_count || serial_dcb->ring_buffer[i] != '\r'){

		// //maybe works

		// //grabbing the character in the ring_buffer
		// char test = serial_dcb.ring_buffer[i];

		// //creating a pointer to it
		// char* ptr = &test;

		// //concat that char to the input buffer
		// strcat(serial_dcb.input,ptr);

		// //clear the char of the ring_buffer
		// memset(ptr,'\0',1);

		//grabbing the letter
		char letter = serial_dcb->ring_buffer[i];

		//putting it in the input buffer
		*serial_dcb->input = letter;

		//incrementing the pointer
		serial_dcb->input++;

		i++;

		serial_dcb->in_transfer_count++;
	}

	//check if more characters are needed
	if(serial_dcb->in_transfer_count < *serial_dcb->input_count){
		return 0;
	}

	//set device code to idle
	serial_dcb->status_code = R6_IDLE;

	//set the event flag
	serial_dcb->event_flag = SET;

	//enable

	sti();

	return serial_dcb->in_transfer_count;
}

int com_write(char *buf_p, int *count_p){

	//port not open
	if(serial_dcb->port_status != OPEN){
		return WRITE_PORT_NOT_OPEN;
	}

	//invalid buffer address
	if(buf_p == NULL){
		return WRITE_INVALID_BUFFER_ADDRESS;
	}

	//invalid count
	if(count_p == NULL){
		return WRITE_INVALID_COUNT;
	}

	//device is busy
	if(serial_dcb->status_code != R6_IDLE){
		return WRITE_DEVICE_BUSY;
	}

	// setting the status code to writing
	serial_dcb->status_code = R6_WRITE;


	//initialize output buffer variables
	serial_dcb->output = buf_p;
	serial_dcb->output_count = count_p;

	//clear event flag
	serial_dcb->event_flag = CLEARED;

	//reset transfer count
	serial_dcb->out_transfer_count = 0;

	//set the writing interrupt to on
	set_int(ON);

	//grab character from requestor's buffer and store in output register
	outb(dev, serial_dcb->output[0]);
	
	
	serial_dcb->output++;
	serial_dcb->out_transfer_count++;
	
	return 0;

}
