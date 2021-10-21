#include <string.h>
#include <system.h>
#include <../modules/pcb_internal.h>
#include <../modules/pcb_commands.h>
#include <core/serial.h>
#include <../modules/mpx_supt.h>

pcb* cop; //currently operating process
context *globalContext;
param params;
extern queue *readyQ;



/*
  Procedure..: klogv
  Description..: Kernel log messages. Sent to active
      serial device.
*/
void klogv(const char *msg)
{
  char logmsg[64] = {'\0'}, prefix[] = "klogv: ";
  strcat(logmsg, prefix);
  strcat(logmsg, msg);
  serial_println(logmsg);
}

/*
  Procedure..: kpanic
  Description..: Kernel panic. Prints an error message
      and halts.
*/
void kpanic(const char *msg)
{
  cli(); //disable interrupts
  char logmsg[64] = {'\0'}, prefix[] = "Panic: ";
  strcat(logmsg, prefix);
  strcat(logmsg, msg);
  klogv(logmsg);
  hlt(); //halt
}


u32int *sys_call(context *registers){
  if(cop == NULL){
    globalContext = registers;
  }
  else{
    if(params.op_code == IDLE){
      cop->stackTop = (unsigned char*) registers;
      insertPCB(cop);
    }
    else if(params.op_code == EXIT){
      freePCB(cop);
    }
  }

  if(readyQ->head != NULL){
    pcb* readyHead = readyQ->head;
    removePCB(readyHead);
    readyHead->state = running;
    cop = readyHead;
    return(u32int*) cop->stackTop;
  }

  return (u32int*) globalContext;
}
