
#include <core/serial.h>
#include <core/io.h>
#include <core/tables.h>
#include <string.h>
#include "mpx_R6.h"

u32int dev = COM1;
int level = 4;

DCB serial_dcb;

u32int original_idt_entry;

void top_handler(){

	if(serial_dcb.port_status == OPEN){
		cli();

		int type = inb(dev+2);

		int bit1 = type>>1 & 1;
		int bit2 = type>>2 & 1;

		if(!bit1 && !bit2){
			//modem
			inb(dev + 6);
		}
		else if(bit1 && !bit2){
			//output 
			//call output handler
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


int com_open(int baud_rate){
	//error checking
	//check baud rate valid
	// check that port is not already open

	cli();


	serial_dcb.port_status = OPEN;
	serial_dcb.events = 1;
	serial_dcb.status_code = R6_IDLE;

	original_idt_entry = idt_get_gate(0x24);
	idt_set_gate(0x24,(u32int) top_handler, 0x08,0x8e);


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

	outb(dev + 1, 0b00000001);
	(void) inb(dev);

	sti();
	return 0;
}

void set_int(int bit, int on){
	if(on){
		outb(dev+1, inb(dev+1) | (1<<bit));
	}
	else{
		outb(dev+1, inb(dev+1) & ~(1<<bit));
	}
}

void input_handler(){
	char i = inb(dev);
	outb(dev, i);
}



