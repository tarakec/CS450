#include "pcbcommands.h"
#include "pcb.h"
#include "modules/mpx_supt.h"
#include <string.h>
#include "comhand.h"

int createPCB(char *name, int classs, int priority){
         if(priority > 9 || priority < 0){//checking for invalid priority
                  char *message = "Error: integer must be between 0-9: Invalid priority";
                  int len = strlen(message);
                  sys_req(WRITE,DEFAULT_DEVICE, message,&len);
                  return 0;
         }
         if(classs > 2 || classs < 0){// checking for correct class
                  char *message = "Error: Invalid class";
                  int len = strlen(message);
                  sys_req(WRITE,DEFAULT_DEVICE, message,&len);
                  return 0;
         }
         if(findPCB(name) != NULL){ //checking for correct name
                  char * message = "Error: Name already in use";
                  int len = strlen(message);
                  sys_req(WRITE,DEFAULT_DEVICE, message,&len);
                  return 0;
         }
 
         PCB *newpcb = setupPCB(name,class,priority);
         insertPCB(newpcb);
         return 1;
 }
 
 int deletePCB(char *name){
         if(findPCB(name) == NULL){//name must be valid
                  char * message = "Error: Invalid name ";
                  int len = strlen(message);
                  sys_req(WRITE,DEFAULT_DEVICE, message,&len);
                  return 0;
         }
         PCB *process = findPCB(name); // finds the pcb
         removePCB(process) // removes it
         freePCB(process); // frees the associated memory
         return 1;
 }
 
 int blockPCB(char *name){
         if(findPCB(name) == NULL){ //make sure it is a valid name
                  char * message = "Error: Invalid name";
         int len = strlen(message);
                  sys_req(WRITE,DEFAULT_DEVICE, message,&len);
                  return 0;
         }
         PCB *process = findPCB(name);//finds pcb
         removePCB(process);// removes it
         process->state = blocked;// makes removed blocked
         insertPCB(process); // reinserts it into the appropriate queue
         return 1;
 }
 
 int unblockPCB(char *name){
         if(findPCB(name) == NULL){ // makes sure that they 
                  char * message = "Error: Invalid name, process not found";
             int len = strlen(message);
                  sys_req(WRITE,DEFAULT_DEVICE, message,&len);
                  return 0;
                  }
         PCB *process = findPCB(name);
         removePCB(process);
         process->state = ready;
         insertPCB(process);
         return 1;
 }