

#ifndef _MPX_R6_H
#define _MPX_R6_H


//for the port status
#define OPEN 0
#define CLOSED 1

//for the status code
#define IDLE 0
#define READ 1
#define WRITE 2


//COM1 Base Address
#define BASE 0x03F8


typedef struct DeviceControlBlock{
	int port_status; //whether the port is open 
	int* event flag_pointer; //pointer to the event flag
	int status_code; //idle, read, or write
	int input_count; //counter for the input buffer
	char *input_buffer; //pointer to the input buffer
	int output_count; //counter for the output buffer
	char *output__buffer;//pointer to the output buffer
	char ring_buffer[256]; 
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
}

int com_open(int *eflag, int baud_rate);

int com_close(void);

int com_read(char *buffer, int *count);

int com_write(char* buffer, int* count);

void disable();

void enable();

#endif