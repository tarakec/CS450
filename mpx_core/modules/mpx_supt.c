/*************************************************************
*	This C file contains the MPX support functions
*	which will be used through out the semester, many set
*	flags or methods that will allow us to modify
*	The behavior of MPX as it progresses throughout
* 	the semester.
**************************************************************/
#include "mpx_supt.h"
#include <mem/heap.h>
#include <string.h>
#include <core/serial.h>
#include "pcb_internal.h"
#include "mem_management.h"
#include "mpx_R6.h"

// global variable containing parameter used when making
// system calls via sys_req
param params;
pcb* cop; //currently operating process
context *globalContext;
extern queue *readyQ;
q *com1Queue;
extern dcb *serial_dcb;

// global for the current module
int current_module = -1;
static int io_module_active = 0;
static int mem_module_active = 0;

// If a student created heap manager is implemented this
// is a pointer to the student's "malloc" operation.
u32int (*student_malloc)(u32int);

// if a student created heap manager is implemented this
// is a pointer to the student's "free" operation.
int (*student_free)(cmcb *);




/* *********************************************
*	This function is use to issue system requests
*	for service.
*
*	Parameters:  op_code:  Requested Operation, one of
*					READ, WRITE, IDLE, EXIT
*			  device_id:  For READ & WRITE this is the
*					  device to which the request is
*					  sent.  One of DEFAULT_DEVICE or
*					   COM_PORT
*			   buffer_ptr:  pointer to a character buffer
*					to be used with READ & WRITE request
*			   count_ptr:  pointer to an integer variable
*					 containing the number of characters
*					 to be read or written
*
*************************************************/
int sys_req( 	int  op_code,
			int device_id,
			char *buffer_ptr,
			int *count_ptr )

{
	int return_code =0;

  if (op_code == IDLE || op_code == EXIT){
    // store the process's operation request
    // triger interrupt 60h to invoke
    params.op_code = op_code;
  	asm volatile ("int $60");
  }// idle or exit

  else if (op_code == READ || op_code == WRITE) {
    // validate buffer pointer and count pointer
    if (buffer_ptr == NULL)
      return_code = INVALID_BUFFER;
    else if (count_ptr == NULL || *count_ptr <= 0)
      return_code = INVALID_COUNT;

    // if parameters are valid store in the params structure
    if ( return_code == 0){
      params.op_code = op_code;
      params.device_id = device_id;
      params.buffer_ptr = buffer_ptr;
      params.count_ptr = count_ptr;

      if (!io_module_active){
        // if default device
        if (op_code == READ)
          return_code = *(polling(buffer_ptr, count_ptr));

        else //must be WRITE
          return_code = serial_print(buffer_ptr);

      } else {// I/O module is implemented
        asm volatile ("int $60");
      } // NOT IO_MODULE
    }
  } else return_code = INVALID_OPERATION;

  return return_code;
}// end of sys_req

/*
  Procedure..: mpx_init
  Description..: Initialize MPX support software, based
			on the current module.  The operation of
			MPX will changed based on the module selected.
			THIS must be called as the first executable
			statement inside your command handler.

  Params..: int cur_mod (symbolic constants MODULE_R1, MODULE_R2, 			etc.  These constants can be found inside
			mpx_supt.h
*/
void mpx_init(int cur_mod)
{

  current_module = cur_mod;
  if (cur_mod == MEM_MODULE)
		mem_module_active = TRUE;

  if (cur_mod == IO_MODULE)
		io_module_active = TRUE;
}



/*
  Procedure..: sys_set_malloc
  Description..: Sets the memory allocation function for sys_alloc_mem
  Params..: Function pointer
*/
void sys_set_malloc(u32int (*func)(u32int))
{
  student_malloc = func;
}

/*
  Procedure..: sys_set_free
  Description..: Sets the memory free function for sys_free_mem
  Params..: s1-destination, s2-source
*/
void sys_set_free(int (*func)(cmcb*))
{
  student_free = func;
}

/*
  Procedure..: sys_alloc_mem
  Description..: Allocates a block of memory (similar to malloc)
  Params..: Number of bytes to allocate
*/
void *sys_alloc_mem(u32int size)
{
  if (!mem_module_active)
    return (void *) kmalloc(size);
  else
    return (cmcb*) (*student_malloc)(size);
}


/*
  Procedure..: sys_free_mem
  Description..: Frees memory
  Params..: Pointer to block of memory to free
*/
int sys_free_mem(void *ptr)
{
  if (mem_module_active)
    return (*student_free)(ptr);
  // otherwise we don't free anything
  return -1;
}

/*
  Procedure..: idle
  Description..: The idle process, used in dispatching
			it will only be dispatched if NO other
			processes are available to execute.
  Params..: None
*/
void idle()
{
  char msg[30];
  int count=0;

	memset( msg, '\0', sizeof(msg));
	strcpy(msg, "IDLE PROCESS EXECUTING.\n");
	count = strlen(msg);

  while(1){
	  sys_req( WRITE, DEFAULT_DEVICE, msg, &count);
    sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
  }
}


u32int *sys_call(context *registers){
  pcb* readyHead = readyQ->head;
  if(cop == NULL){
    globalContext = registers;
  }
  else{
    if(params.op_code == IDLE){
      cop->stackTop = (unsigned char*) registers;
      cop -> state = ready;
      insertPCB(cop);
    }
    else if(params.op_code == EXIT){
      freePCB(cop);
    }

    else if(params.op_code == READ || params.op_code == WRITE ){
      cop->stackTop = (unsigned char*) registers;

    IOScheduler();
  }
}

  if(readyHead != NULL){
    // serial_println("head not empth");
    // pcb* readyHead = readyQ->head;
    removePCB(readyHead);
    readyHead->state = running;
    cop = readyHead;
    return(u32int*) cop->stackTop;
  }

  return (u32int*) globalContext;
  }


  int IOScheduler(){
  klogv("IOScheduler");
    if(params.op_code != READ && params.op_code != WRITE){
      return INVALID_OPERATION;
    }

      iocb *ptr = sys_alloc_mem(sizeof(iocb));

    
      //if the device is not busy
      if(serial_dcb->status_code == R6_IDLE){

        //place cop in blocked state
        cop->state = blocked;
        insertPCB(cop); 

        //place information in iocb
        ptr->requestor = cop;
        ptr->dcb = serial_dcb;
        ptr->operation = params.op_code;
        ptr->buff_ptr = params.buffer_ptr;
        ptr->count_ptr = params.count_ptr;
        ptr->next = NULL;



        if(ptr->operation == READ){
          com_read(ptr->buff_ptr, ptr->count_ptr);
      }
      else{
          com_write(ptr->buff_ptr, ptr->count_ptr);
        }
      }
    
    //if it is busy, then add to the queue
    else{
      
      cop->state = blocked;
      insertPCB(cop);
      
      ptr->requestor = cop;
      ptr->dcb = serial_dcb;
      ptr->operation = params.op_code;

     //insert iocb at the head
      if(com1Queue->head == NULL){
        com1Queue->head = ptr;
        ptr->next = com1Queue->tail;
        com1Queue->tail = NULL;
        com1Queue->count++;
      }
        //if not at the head, then at the tail
      else{
        com1Queue->tail = ptr;
        ptr->next = com1Queue->tail;
        com1Queue->tail = NULL;
        com1Queue->count++;
      }

    
    }
    return dispatcher();
  }

  int dispatcher(){

    klogv("Dispatcher");
    //dispatcher
    iocb* head = com1Queue->head;
    klogv("TEST1");
    //if the event flag is set and there is an iocb at the head
    if(head->dcb->event_flag == SET && head != NULL){
       klogv("TEST2");
      //remove pcb from blocked to ready queue
      removePCB(head->requestor);
      head->requestor->state = ready;
      insertPCB(head->requestor);
   
      if(head->operation == READ){
        com_read(head->buff_ptr, head->count_ptr);
      }
      else if(head->operation == WRITE){
        com_write(head->buff_ptr, head->count_ptr);
      }
      
      //remove from IOCB queue
      iocb *temp = head;
      head = head->next;
      sys_free_mem(temp);
      com1Queue->count--;

      return 0;
    }
    klogv("test3");
    return ERROR;
  }