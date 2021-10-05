
#include "pcb_commands.h"
#include "pcb_internal.h"
#include "mpx_supt.h"
#include <string.h>
#include "commandhandler.h"

queue *readyQ;
queue *blockedQ;
queue *suspendedReadyQ;
queue *suspendedBlockedQ;

int createPCB(char *name, int classs, int priority){

         if(priority > 9 || priority < 0){//checking for invalid priority
                  char *message = "Error: integer must be between 0-9: Invalid priority.\n";
                  int len = strlen(message);
                  sys_req(WRITE,DEFAULT_DEVICE, message,&len);
                  return 0;
         }
         if(classs >=2 || classs < 0){// checking for correct class
                  char *message = "Error: Invalid class\n";
                  int len = strlen(message);
                  sys_req(WRITE,DEFAULT_DEVICE, message,&len);
                  return 0;
         }
         if(findPCB(name) != NULL){ //checking for correct name
                  char * message = "Error: Name already in use\n";
                  int len = strlen(message);
                  sys_req(WRITE,DEFAULT_DEVICE, message,&len);
                  return 0;
         }
 
         pcb *newpcb = setupPCB(name,classs,priority);
         insertPCB(newpcb);
         return 1;
 }
 
 int deletePCB(char *name){

    if(!error_name_check(name)){
 		return 0;
 	}

     pcb *process = findPCB(name); // finds the pcb
     removePCB(process); // removes it
     freePCB(process); // frees the associated memory
     return 1;
 }
 
 int blockPCB(char *name){

      if(!error_name_check(name)){
 		return 0;
 	}
     int flag = 0;

     pcb *process = findPCB(name);//finds pcb

     if (process->state == suspendedReady){ //check if suspended
        flag = 1;
     }

     removePCB(process);// removes it

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

     if(!error_name_check(name)){
 		return 0;
 	}

     int flag = 0;
     pcb *process = findPCB(name);

     if(process->state == suspendedBlocked){
        flag = 1;
     }
     removePCB(process);

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

 	if(!error_name_check(name)){
 		return 0;
 	}

    int flag = 0;
 	pcb *process = findPCB(name);


    if(process->state == blocked){
        flag = 1;
    }
    removePCB(process);

    if(flag){
        process->state = suspendedBlocked;
    }
    else{
        process->state = suspendedReady;
    }

 	insertPCB(process);
 	return 1;

 }

 int resumePCB(char *name){
 	if(!error_name_check(name)){
 		return 0;
 	}

    int flag = 0;

 	pcb *process = findPCB(name);

    if(process->state == suspendedBlocked){
        flag = 1;
    }

    removePCB(process);
    if(flag){
        process->state = blocked;
    }
    else{
        process->state = ready;
    }
 	
    insertPCB(process);
 	return 1;
 }

 int setPriority(char *name, int priority){

 	if(!error_name_check(name)){
 		return 0;
 	}

 	if(priority > 9 || priority < 0){
 		char* message = "Error: Invalid Priority. Must be an integer from 0-9.\n";
 		int len = strlen(message);
 		sys_req(WRITE,DEFAULT_DEVICE,message,&len);
 		return 0;
 	}

 	pcb *process = findPCB(name);
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
	char numBuffer[2];
	itoa(priority,numBuffer);
	int numLen = strlen(numBuffer);
	sys_req(WRITE,DEFAULT_DEVICE,numBuffer,&numLen);

    memset(str1,'\0',16);
    memset(str2,'\0',16);
    memset(str3,'\0',16);
    memset(str5,'\0',16);

	return 1;

 }

 void showReady(){

 	int i = 0;

    char* msg = "\nReady: \n";
    int msgLen = strlen(msg);
    sys_req(WRITE,DEFAULT_DEVICE,msg,&msgLen);

 	pcb *loop = readyQ->head;
 	while( i < readyQ->count){
 		int len = strlen(loop->name);
 		sys_req(WRITE,DEFAULT_DEVICE,loop->name,&len);
 		sys_req(WRITE,DEFAULT_DEVICE,"\n",&len);

 		if(loop->prev!=NULL){
 			loop = loop->prev;
 		}
 		i++;
 	}
    
   
 }

 void showBlocked(){

 	int i = 0;

    char* msg = "\nBlocked: \n";
    int msgLen = strlen(msg);
    sys_req(WRITE,DEFAULT_DEVICE,msg,&msgLen);

 	pcb *loop = blockedQ->head;
 	while( i < blockedQ->count){
 		int len = strlen(loop->name);
 		sys_req(WRITE,DEFAULT_DEVICE,loop->name,&len);
 		sys_req(WRITE,DEFAULT_DEVICE,"\n",&len);

 		if(loop->prev!=NULL){
 			loop = loop->prev;
 		}
 		i++;
 	}
    
   
 }

  void showSuspendedReady(){

    int i = 0;

    char* msg = "\nSuspended Ready: \n";
    int msgLen = strlen(msg);
    sys_req(WRITE,DEFAULT_DEVICE,msg,&msgLen);

    pcb *loop = suspendedReadyQ->head;
    while( i < suspendedReadyQ->count){
        int len = strlen(loop->name);
        sys_req(WRITE,DEFAULT_DEVICE,loop->name,&len);
        sys_req(WRITE,DEFAULT_DEVICE,"\n",&len);

        if(loop->prev!=NULL){
            loop = loop->prev;
        }
        i++;
    }
    
   
 }

  void showSuspendedBlocked(){

    int i = 0;

    char* msg = "\nSuspended Blocked: \n";
    int msgLen = strlen(msg);
    sys_req(WRITE,DEFAULT_DEVICE,msg,&msgLen);

    pcb *loop = suspendedBlockedQ->head;
    while( i < suspendedBlockedQ->count){
        int len = strlen(loop->name);
        sys_req(WRITE,DEFAULT_DEVICE,loop->name,&len);
        sys_req(WRITE,DEFAULT_DEVICE,"\n",&len);

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
