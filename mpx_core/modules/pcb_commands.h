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
int createPCB(char *name, int classs, int priority);

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

int suspendPCB(char *name);

int resumePCB(char *name);

int setPriority(char *name, int priority);

int showPCB(char *name);

void showReady();

void showBlocked();

void showSuspendedReady();

void showSuspendedBlocked();

void showAll();

int error_name_check(char *name);

#endif
