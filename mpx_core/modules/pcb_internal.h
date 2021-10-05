#ifndef _PCBINT_H
#define _PCBINT_H


//class defs
#define system 1
#define application 0

//state defs
#define ready 0
#define suspendedReady 2
#define running 1
#define blocked -1
#define suspendedBlocked -2

//stack size def
#define stackSize 1024

//create struct fro pcb
typedef struct pcb{
    char name[99];
    int class_;
    int priority;
    int state;
    int processStack;
    struct pcb *next;
    struct  pcb *prev;
}pcb;

//basic queue struct
typedef struct queue{
    int count;
    pcb *head;
    pcb *tail;
}queue;


/**
 * allocatePCB()
 * allocates memory for pcbs
 * @return pcb pointer or NULL
 */

pcb *allocatePCB();

/**
 * freePCB()
 * frees memory associated with pcb
 * @param toBeFreed the pcb to be freed
 * @return -1 if there was nothing to be freed/ error, anything else is sucesss
 */

int freePCB(pcb *process);


/**
 * setupPcb()
 * sets up the pcb and initializes the name, the class and priority.
 * error checks to see if the name and priority falls within the guidlines
 * every pcb setup with this method is set to ready
 * @param name the name of the process, at least 8 characters
 * @param class the code that tells the system whether or not the process is an
 *              application or a system process
 * @param priority an int 0-9, lower execute after higher priorities
 * @returns pcb pointer initialized with given values
 */
pcb *setupPCB(char *name, int class, int priority);


void insertPCB(pcb *process);
pcb *findPCB(char *processName);
int removePCB(pcb *process);

#endif