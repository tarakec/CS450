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

