

#ifndef _MPX_R6_H
#define _MPX_R6_H


//for the port status
#define OPEN 0
#define CLOSED 1

//for the status code
#define R6_IDLE 0
#define R6_READ 1
#define R6_WRITE 2


//COM1 Base Address
#define BASE 0x03F8


typedef struct device{
	int port_status; //whether the port is open 
	int events; //0 at beginning of operation, 1 at end
	int status_code; //idle, read, or write
	int input_count; //counter for the input buffer
	int input_index;
	unsigned char *input; //pointer to the input buffer
	int output_count; //counter for the output buffer
	unsigned char *output;//pointer to the output buffer
	int output_index;
	unsigned char ring_buffer[16]; 
	int ring_input; //amount characters coming into the ring buffer
	int ring_output; // amount characters coming out of the ring buffer
	int ring_counter; //amount of total characters in the ring buffer
}DCB;

typedef struct IOControlBlock{

}IOCB;

typedef struct IOQueue{
	int count;
	IOCB *head;
	IOCB *tail;
}q;

int com_open(int baud_rate);

void top_handler();

void set_int(int bit, int on);

void input_handler();

#endif