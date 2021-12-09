#include "pcb_internal.h"

#ifndef _MPX_R6_H
#define _MPX_R6_H

//pic
#define PIC_MASK 0x21

#define ON 1
#define OFF 0

//for the port status
#define OPEN 0
#define CLOSED 1

//event flag codes
#define CLEARED 0
#define SET 1

//for the status code
#define R6_IDLE 0
#define R6_READ 1
#define R6_WRITE 2


//COM1 Base Address
#define BASE 0x03F8

//com_open error codes
#define INVALID_FLAG_PTR -101
#define INVALID_BAUD_DIVISOR -102
#define PORT_ALREADY_OPEN -103

//com_close error codes
#define PORT_NOT_OPEN -201

//com_read error codes
#define READ_PORT_NOT_OPEN -301
#define INVALID_BUFFER_ADDRESS -302
#define READ_INVALID_COUNT -303
#define DEVICE_BUSY -304

//com_write error codes
#define WRITE_PORT_NOT_OPEN -401
#define WRITE_INVALID_BUFFER_ADDRESS -402
#define WRITE_INVALID_COUNT -403
#define WRITE_DEVICE_BUSY -404




typedef struct device{
	int port_status; //whether the port is open 
	int event_flag; //0 at beginning of operation, 1 at end
	int status_code; //idle, read, or write
	int *input_count; //counter for the input buffer
	char *input; //pointer to the input buffer
	int *output_count; //counter for the output buffer
	char *output;//pointer to the output buffer
	char ring_buffer[16]; 
	int ring_input; //amount characters coming into the ring buffer
	int ring_output; // amount characters coming out of the ring buffer
	int ring_counter; //amount of total characters in the ring buffer
	int in_transfer_count; // amount of transferred characters
	int out_transfer_count;
}dcb;

typedef struct IOControlBlock{
	pcb *requestor;
	dcb *dcb;
	int operation; //read or write
	char *buff_ptr; 
	int *count_ptr;
	struct IOControlBlock *next;
}iocb;

typedef struct IOQueue{
	int count;
	iocb *head;
	iocb *tail;
}q;

int com_open(int baud_rate);

void top_handler();

void set_int(int on);

void input_handler();

int com_close();

int com_read(char *buf_p, int *count_p);

void output_handler();

int com_write(char* buff_p, int* count_p);

int com_read(char* buff_p, int* count_p);

void enable();
void disable();

#endif