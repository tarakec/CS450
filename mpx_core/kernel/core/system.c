#include <string.h>
#include <system.h>
#include <../modules/pcb_internal.h>
#include <../modules/pcb_commands.h>
#include <core/serial.h>
#include <../modules/mpx_supt.h>

// pcb* cop; //currently operating process
// context *globalContext;
// extern queue *readyQ;


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


// u32int *sys_call(context *registers){
//   pcb* head = readyQ->head;
//   if(cop == NULL){
//     globalContext = registers;
//   }
//   else{
//     if(params.op_code == IDLE){
//       // serial_println("idlinfg");
//       cop->stackTop = (unsigned char*) registers;
//       cop -> state = ready;
//       insertPCB(cop);
//     }
//     else if(params.op_code == EXIT){
//       freePCB(cop);
//     }
//   }

//   if(head != NULL){
//     // serial_println("head not empth");
//     // pcb* readyHead = readyQ->head;
//     removePCB(head);
//     head->state = running;
//     cop = head;
//     return(u32int*) cop->stackTop;
//   }

//   return (u32int*) globalContext;
// }
