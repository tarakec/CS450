

#include "pcb_commands.h"
#include "pcb_internal.h"
#include "mpx_supt.h"
#include <string.h>
#include "commandhandler.h"
#include "procsr3.h"
#include "../include/core/serial.h"

queue *readyQ;
queue *blockedQ;
queue *suspendedReadyQ;
queue *suspendedBlockedQ;

pcb* createPCB(char *name, int classs, int priority){

         if(priority > 9 || priority < 0){//checking for invalid priority
                  char *message = "Error: Priority must be integer between 0-9.\n";
                  int len = strlen(message);
                  sys_req(WRITE,DEFAULT_DEVICE, message,&len);
                  return 0;
         }
         if(classs >=2 || classs < 0){// checking for correct class
                  char *message = "Error: Invalid class.\n";
                  int len = strlen(message);
                  sys_req(WRITE,DEFAULT_DEVICE, message,&len);
                  return 0;
         }
         if(findPCB(name) != NULL){ //checking for correct name
                  char * message = "Error: Name already in use,\n";
                  int len = strlen(message);
                  sys_req(WRITE,DEFAULT_DEVICE, message,&len);
                  return 0;
         }
 
         pcb *newpcb = setupPCB(name,classs,priority);
         insertPCB(newpcb);
         return newpcb;
 }
 
 int deletePCB(char *name){

    if(!error_name_check(name)){ //name check
 		return 0;
 	}

     pcb *process = findPCB(name); // finds the pcb

     if(!strcmp(name,"idle")){
        if(process->state == ready){
            serial_print("Cannot delete idle process in the ready state.\n");
            return 0;
        }
     }
     if(removePCB(process) == 1 && (freePCB(process) != -1)){ //removes and frees it
        return 1;
     }
     else{
        return 0;
     }
}
 
 int blockPCB(char *name){

      if(!error_name_check(name)){  //name check
 		return 0;
 	}

     int flag = 0;

     pcb *process = findPCB(name);//finds pcb

     if((process->state == suspendedBlocked) || (process->state == blocked)){ //if it is already blocked just return
        return 1;
     }
     removePCB(process);// removes it


     if (process->state == suspendedReady){ 
        flag = 1;
     }

     if(flag){
        process->state = suspendedBlocked; //suspended blocked
     }
     else{
        process->state = blocked;// blocked
     }
     
     insertPCB(process); // reinserts it into the appropriate queue
     return 1;
 }
 
 int unblockPCB(char *name){

     if(!error_name_check(name)){ //name check
 		return 0;
 	}

     int flag = 0;

     pcb *process = findPCB(name); //find pcb

    if((process->state == suspendedReady) || (process->state == ready)){ //if already unblocked just return
        return 1;
     }
     removePCB(process);

     if(process->state == suspendedBlocked){ 
        flag = 1;
     }

     if(flag){
        process->state = suspendedReady; //suspended ready
     }
     else{
         process->state = ready; //ready
     }
    
     insertPCB(process);
     return 1;
 }

 int suspendPCB(char* name){

 	if(!error_name_check(name)){ //name check
 		return 0;
 	}

    int flag = 0;

 	pcb *process = findPCB(name); //find the pcb

    if((process->state == suspendedBlocked) || (process->state == suspendedReady)){ //if already suspended just return
        return 1;
     }
    removePCB(process);

    if(process->state == blocked){
        flag = 1;
    }
    
    if(flag){
        process->state = suspendedBlocked; //suspended blocked
    }
    else{
        process->state = suspendedReady; //suspended ready
    }

 	insertPCB(process);
 	return 1;

 }

 int resumePCB(char *name){
 	if(!error_name_check(name)){
 		return 0;
 	}

    int flag = 0;

 	pcb *process = findPCB(name); // find pcb

    if((process->state == blocked) || (process->state == ready)){ //if already unsuspended just return
        return 1;
     }
    removePCB(process);

    if(process->state == suspendedBlocked){
        flag = 1;
    }

    if(flag){
        process->state = blocked; //blocked
    }
    else{
        process->state = ready; //ready
    }
 	
    insertPCB(process);
 	return 1;
 }

 int setPriority(char *name, int priority){

 	if(!error_name_check(name)){
 		return 0;
 	}

 	if(priority > 9 || priority < 0){
 		char* message = "Error: Invalid Priority. Must be an integer from 0-9.\n"; //priority check
 		int len = strlen(message);
 		sys_req(WRITE,DEFAULT_DEVICE,message,&len);
 		return 0;
 	}

 	pcb *process = findPCB(name); //assigning the priority
 	removePCB(process);
 	process->priority = priority;
 	insertPCB(process);
 	return 1;
 }

 int showPCB(char* name){

 	if(!error_name_check(name)){
 		return 0;
 	}

 	pcb *process = findPCB(name);
    printPCB(process);
	return 1;

 }

 void showReady(){

 	int i = 0;

    char* msg = F_YELLOW "\nReady: \n" RESET;
    int msgLen = strlen(msg);
    sys_req(WRITE,DEFAULT_DEVICE,msg,&msgLen);

 	pcb *loop = readyQ->head;
 	while( i < readyQ->count){
        printPCB(loop);


 	if(loop->prev!=NULL){
 		loop = loop->prev;
 	}
 		i++;
 	}
 }

 void showBlocked(){

 	int i = 0;

    char* msg = F_YELLOW"\nBlocked: \n"RESET;
    int msgLen = strlen(msg);
    sys_req(WRITE,DEFAULT_DEVICE,msg,&msgLen);

 	pcb *loop = blockedQ->head;
 	while( i < blockedQ->count){
 		printPCB(loop);

 		if(loop->prev!=NULL){
 			loop = loop->prev;
 		}
 		i++;
 	}
 }

  void showSuspendedReady(){

    int i = 0;

    char* msg = F_YELLOW "\nSuspended Ready: \n" RESET;
    int msgLen = strlen(msg);
    sys_req(WRITE,DEFAULT_DEVICE,msg,&msgLen);

    pcb *loop = suspendedReadyQ->head;
    while( i < suspendedReadyQ->count){
        printPCB(loop);

        if(loop->prev!=NULL){
            loop = loop->prev;
        }
        i++;
    }
 }

  void showSuspendedBlocked(){

    int i = 0;

    char* msg = F_YELLOW "\nSuspended Blocked: \n" RESET;
    int msgLen = strlen(msg);
    sys_req(WRITE,DEFAULT_DEVICE,msg,&msgLen);

    pcb *loop = suspendedBlockedQ->head;

    while(i < suspendedBlockedQ->count){

        printPCB(loop);

        if(loop->prev!=NULL){
            loop = loop->prev;
        }
        i++;
    }
 }

 void showAll(){
 	showReady();
 	showBlocked();
    showSuspendedReady();
    showSuspendedBlocked();
 }

int error_name_check(char *name){
	if(findPCB(name) == NULL){
 		char *message = "Error: The name is invalid.\n";
 		int len = strlen(message);
 		sys_req(WRITE,DEFAULT_DEVICE,message,&len);
 		return 0;
 	}
 	else
 		return 1;
}

void printPCB(pcb *process){

    char str1[16] = "\nProcess Name: ";
    int str1Len = strlen(str1);
    int nameLen = strlen(process->name);
    sys_req(WRITE,DEFAULT_DEVICE,str1,&str1Len);
    sys_req(WRITE,DEFAULT_DEVICE,process->name,&nameLen);

    char str2[16] = "\nState: ";
    int str2Len;
 
    if(process->state == ready){
        strcat(str2,"Ready");
        str2Len = strlen(str2);
        sys_req(WRITE,DEFAULT_DEVICE,str2,&str2Len);
    }
    else if(process->state == blocked){
        strcat(str2,"Blocked");
        str2Len = strlen(str2);
        sys_req(WRITE,DEFAULT_DEVICE,str2,&str2Len);
    }
    else if(process->state == suspendedBlocked){
        strcat(str2,"Suspended Blocked");
        str2Len = strlen(str2);
        sys_req(WRITE,DEFAULT_DEVICE,str2,&str2Len);
    }
    else if(process->state == suspendedReady){
        strcat(str2,"Suspended Ready");
        str2Len = strlen(str2);
        sys_req(WRITE,DEFAULT_DEVICE,str2,&str2Len);
    }
    else{
        strcat(str2,"Running");
        str2Len = strlen(str2);
        sys_req(WRITE,DEFAULT_DEVICE,str2,&str2Len);
    }

    char str3[16] = "\nClass: ";
    int str3Len;
    
    if((process->class_) == system){
        strcat(str3, "System");
        str3Len = strlen(str3);
        sys_req(WRITE,DEFAULT_DEVICE,str3,&str3Len);
        
    }else{
        strcat(str3, "Application");
        str3Len = strlen(str3);
        sys_req(WRITE,DEFAULT_DEVICE,str3,&str3Len);
    }

    char str5[16] = "\nPriority: ";
    int slen5 = strlen(str5);
    sys_req(WRITE,DEFAULT_DEVICE,str5,&slen5);

    int priority = process->priority;
    char numBuffer[3];
    itoa(priority,numBuffer);
    int numLen = strlen(numBuffer);
    sys_req(WRITE,DEFAULT_DEVICE,numBuffer,&numLen);
    sys_req(WRITE,DEFAULT_DEVICE,"\n\n",&numLen);

    memset(str1,'\0',16);
    memset(str2,'\0',16);
    memset(str3,'\0',16);
    memset(str5,'\0',16);
}
pcb* load_proc(char *name, void(*func)(void)) {
    pcb *newPcb = setupPCB(name, 1,1);
    context *cp = (context*)(newPcb->stackTop);
    newPcb->state = suspendedReady;
    memset(cp,0,sizeof(context));
    cp->fs = 0x10;
    cp->gs = 0x10;
    cp->ds = 0x10;
    cp->es = 0x10;
    cp->cs = 0x8;
    cp->ebp = (u32int)(newPcb->stack);
    cp->esp = (u32int)(newPcb->stackTop);
    cp->eip = (u32int) func;
    cp->eflags = 0x202;
    return newPcb;
}


pcb* loadComIdle(char *name, void(*func)(void)) {
    pcb *newPcb = createPCB(name, 1,1);
    context *cp = (context*)(newPcb->stackTop);
    newPcb->state = ready;
    memset(cp,0,sizeof(context));
    cp->fs = 0x10;
    cp->gs = 0x10;
    cp->ds = 0x10;
    cp->es = 0x10;
    cp->cs = 0x8;
    cp->ebp = (u32int)(newPcb->stack);
    cp->esp = (u32int)(newPcb->stackTop);
    cp->eip = (u32int) func;
    cp->eflags = 0x202;
    return newPcb;
}

void loadr3(){
     int size = 32;
     insertPCB(load_proc("proc1", &proc1)); // needs processes but not sure who is initizalizing that 
     sys_req(WRITE, DEFAULT_DEVICE, "Process 1 has been loaded!\n", &size);
     insertPCB(load_proc("proc2", &proc2));
     sys_req(WRITE, DEFAULT_DEVICE, "Process 2 has been loaded!\n", &size);
     insertPCB(load_proc("proc3", &proc3));
     sys_req(WRITE, DEFAULT_DEVICE, "Process 3 has been loaded!\n", &size);
     insertPCB(load_proc("proc4", &proc4));
     sys_req(WRITE, DEFAULT_DEVICE, "Process 4 has been loaded!\n", &size);
     insertPCB(load_proc("proc5", &proc5));
     sys_req(WRITE, DEFAULT_DEVICE, "Process 5 has been loaded!\n", &size);
}

void yield() {
     asm volatile("int $60");
}

void allocateQueues(){
    blockedQ = sys_alloc_mem(sizeof(struct queue));
    blockedQ->count = 0;
    readyQ = sys_alloc_mem(sizeof(struct queue));
    readyQ->count = 0;
    suspendedBlockedQ = sys_alloc_mem(sizeof(struct queue));
    suspendedBlockedQ->count = 0;
    suspendedReadyQ = sys_alloc_mem(sizeof(struct queue));
    suspendedReadyQ->count = 0;
}

