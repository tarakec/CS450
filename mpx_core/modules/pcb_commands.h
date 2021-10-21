#include "pcb_internal.h"

#ifndef _PCBCMDS_H
#define _PCBCMDS_H
/** \file pcbcommands.h
 * Creates the interaction and user commands to acess the functionality of the process control blocks. Displays all the necessary information regarding the PCB system within the operating system.
 */

/**
 Creates a process control block
 @param *name the identifying name of the pcb
 @param class the type of pcb, either an application or system process
 @param priority the priority assigned to the process
 */
pcb* createPCB(char *name, int classs, int priority);

/**
 Deletes the process control block given from the respected queue
 @param *the name of the process control block
 */
int deletePCB(char *name);

/**
 Finds the process control block given and sets its state to blocked and reinserts it into the appropriate queue
 @param *the name of the process control block
 */
int blockPCB(char *name);

/**
 Finds the given process control block and sets its state to ready and reinserts it into the appropriate queue
 @param *the name of the process control block
 */
int unblockPCB(char *name);

/**
 Plsces the process control block in the suspended state and then reinsets it into the appropriate queue
 @param *the name of the process control block
 */
int suspendPCB(char *name);

/**
 Places the process control block in the state that is not suspended and reinserts it into the appropriate queue
 @param *the name of the process control block
 */
int resumePCB(char *name);

/**
 Sets the process control block's priority and then reinserts the process into the correct position in the correct queue
 @param *the name of the process control block and the new priority
 */
int setPriority(char *name, int priority);

/**
Displays the process name, class, state, suspended status, and priority of the given process control block
 @param *the name of the process control block
 */
int showPCB(char *name);

/**
Displays the process name, class, state, suspended status, and priority of the given process control block in the ready queue
 @param *None
 */
void showReady();

/**
Displays the process name, class, state, suspended status, and priority of the given process control block in the block queue
 @param *None
 */
void showBlocked();

/**
Displays the process name, class, state, suspended status, and priority of the given process control block in the ready queue of the suspended state
 @param *None
 */
void showSuspendedReady();

/**
Displays the process name, class, state, suspended status, and priority of the given process control block in the block queue of the suspended state
 @param *None
 */
void showSuspendedBlocked();
/**
Displays the process name, class, state, suspended status, and priority of the given process control block for the ready and blocked queues.
 @param *None
 */
void showAll();

/**
 * checks the queues to make sure that the name can be found within them. If not, displays an error message.
 * @param char* name, name of the process
 */
int error_name_check(char *name);

/**
 * prints the valuable information about the process
 * @param pcb* process
 */

void printPCB(pcb* process);

pcb* load_proc(char *name, void(*func)(void));

void loadr3();

void yield();

#endif