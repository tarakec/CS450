#include "pcb_internal.h"
#include "mpx_supt.h"
#include <string.h>




queue *readyQ;
queue *blockedQ;
queue *suspendedReadyQ;
queue *suspendedBlockedQ;


pcb* allocatePCB(){
		pcb* process = (pcb*) sys_alloc_mem(sizeof(pcb));
		return process;
		}


int freePCB(pcb *toBeFreed){
		int i;
		i = sys_free_mem(toBeFreed);

		return i;
    }   


pcb* setupPCB(char *name, int class, int priority){
		pcb* process = allocatePCB();

			if(strlen(name) < 8){
                return process;
				}
			if(priority<0 || priority>9){
                return process;   
        		}
				
			strcpy(process->name, name);
			process-> class = class;
			process -> priority = priority;
            process -> state = ready;

		return process;
	}

    pcb* findPCB(char *name){
		int i = 0;
        pcb *readyTail = readyQ -> tail; //tail node of pointer readyQ queue
        
        //loop through readyQ queue
        while(i < readyQ->count){
            if(strcmp(readyTail->name, name) == 0){
                return readyTail;
            }
            if (readyTail->next != NULL){
                readyTail = readyTail-> next;
            }
            i++;
        }//not in the readyQ queue

        //loop through blockedQqueue
        i = 0;
        pcb* blockedTail = blockedQ->tail; //tail node of blockedQqueue
        while(i < blockedQ->count){
            if(strcmp(blockedTail->name, name) == 0){
                return blockedTail;
            }
            if(blockedTail->next != NULL){
                blockedTail = blockedTail->next;
            }
            i++;
        }//not in blocked

        //loop through suspended ready
        i = 0;
        pcb *suspReadyTail = suspendedReadyQ->tail;
        while(i < suspendedReadyQ->count){
            if(strcmp(suspReadyTail->name, name) == 0){
                return suspReadyTail;
            }
            if(suspReadyTail->next != NULL){
                suspReadyTail = suspReadyTail-> next;
            }
            i++;
        }// not in suspended ready

        //loop through suspended blocked
        i = 0;
        pcb *suspBlockedTail = suspendedBlockedQ->tail;
        while(i < suspendedBlockedQ->count){
            if(strcmp(suspBlockedTail->name, name) == 0){
                return suspBlockedTail;
            if(suspBlockedTail->next != NULL){
                suspBlockedTail = suspBlockedTail->next;
            }
            i++;
        }//not in suspeneded blocked

        
	}
    //not found -- return NULL
        return NULL;
}
void insertPCB(pcb *process){
    if(process -> state == ready){ //priority queue
        pcb *tmp = readyQ -> head;
        if(readyQ->count == 0){ //if it is an empty queue
            readyQ -> head = process;
            readyQ -> tail =process;
            process -> next = NULL;
            process -> prev = NULL;
            readyQ -> count += 1;
        }
        else if(readyQ -> count == 1){ //one thing in queue
            if(tmp ->priority < process->priority){ //check priority
               //the priority of the existing process is less than process we're trying to insert
            //the new process is inserted after the existing process in the queue and pcb
                process -> prev = tmp;
                process -> next = NULL;
                tmp -> next = process;
                readyQ -> head = process;
            }
            else{
                //the priority of existing process is greater than the process we're trying to insert
                //new process inserted before the existing one
                process -> next = tmp;
                process->prev = process;
                readyQ -> tail = process;
            }
            readyQ->count+=1;
        }
        else{ //more than one thing in the queue
            int i = 0;

            //loops through readyQ queue and finds the appropriate spot if the process's priority is larger than the readyQ queue tail
            while (i<readyQ->count){ 
                if(process->priority > tmp -> priority){
                    if(tmp->next != NULL){
                        tmp = tmp ->next;
                    }
                }
                else{
                    if(tmp->prev != NULL){
                        tmp = tmp->prev;
                    }
                }
                i++;
            }
            //checks to see if we are at head node
            if(tmp->prev == NULL){
                tmp->prev = process;
                process -> next = tmp;
                process -> prev = NULL;
                readyQ -> head = process;
                  }
            else if(tmp->next == NULL){// check if tail
                tmp->next = process;
                process->prev = tmp;
                process->next = NULL;
                readyQ -> tail = process;

            }
            else{
                pcb *next = tmp->next;
                process->prev = tmp;
                process -> next = next;
                tmp->next = process;
                //loop to find end of pbc
                pcb *looper = process;
                while(looper->next != NULL){
                    
                        looper = looper->next;
        

                }//tmp (looper) should now equal its last process and have the included new process with it

                readyQ ->tail = looper;
            }
            
        }
        readyQ ->count +=1;
    } 
    else if(process -> state == suspendedReady){ //priority queue
        pcb *tmp = suspendedReadyQ -> head;
        if(suspendedReadyQ->count == 0){ //if it is an empty queue
            suspendedReadyQ -> head = process;
            suspendedReadyQ -> tail =process;
            process -> next = NULL;
            process -> prev = NULL;
            suspendedReadyQ -> count += 1;
        }
        else if(suspendedReadyQ -> count == 1){ //one thing in queue
            if(tmp ->priority < process->priority){ //check priority
               //the priority of the existing process is less than process we're trying to insert
            //the new process is inserted after the existing process in the queue and pcb
                process -> prev = tmp;
                process -> next = NULL;
                tmp -> next = process;
                suspendedReadyQ -> head = process;
            }
            else{
                //the priority of existing process is greater than the process we're trying to insert
                //new process inserted before the existing one
                process -> next = tmp;
                process->prev = process;
                suspendedReadyQ -> tail = process;
            }
            suspendedReadyQ->count+=1;
        }
        else{ //more than one thing in the queue
            int i = 0;

            //loops through suspendedReadyQQ queue and finds the appropriate spot if the process's priority is larger than the readyQ queue tail
            while (i<suspendedReadyQ->count){ 
                if(process->priority > tmp -> priority){
                    if(tmp->next != NULL){
                        tmp = tmp ->next;
                    }
                }
                else{
                    if(tmp->prev != NULL){
                        tmp = tmp->prev;
                    }
                }
                i++;
            }
            //checks to see if we are at head node
            if(tmp->prev == NULL){
                tmp->prev = process;
                process -> next = tmp;
                process -> prev = NULL;
                suspendedReadyQ -> head = process;
                  }
            else if(tmp->next == NULL){// check if tail
                tmp->next = process;
                process->prev = tmp;
                process->next = NULL;
                suspendedReadyQ -> tail = process;

            }
            else{
                pcb *next = tmp->next;
                process->prev = tmp;
                process -> next = next;
                tmp->next = process;
                //loop to find end of pbc
                
                pcb *looper = process;
                while(looper->next != NULL){
                   
                    looper = looper->next;
                    
                }//tmp should now equal its last process and have the included new process with it

                suspendedReadyQ ->tail = looper;
            }
            
        }
        suspendedReadyQ ->count +=1;
    }
    else if(process -> state == blocked){ //FIFO queue
        if(blockedQ->count == 0){//nothing in the queue
            blockedQ-> tail = process;
            blockedQ-> head = process;
            process -> next = NULL;
            process -> prev = NULL;

            blockedQ-> count += 1;
        } 
        else{ //other things in queue insert in front of queue
            blockedQ->tail->prev = process;
            process -> next = blockedQ-> tail;
            process -> prev = NULL;
            blockedQ-> tail = process;
            blockedQ-> count +=1;
        }


    }
    else if(process -> state == suspendedBlocked){ //FIFO queue
        if(suspendedBlockedQ->count == 0){//nothing in the queue
            suspendedBlockedQ-> tail = process;
            suspendedBlockedQ-> head = process;
            process -> next = NULL;
            process -> prev = NULL;

            suspendedBlockedQ-> count += 1;
        } 
        else{ //other things in queue insert in front of queue
            suspendedBlockedQ->tail->prev = process;
            process -> next = suspendedBlockedQ-> tail;
            process -> prev = NULL;
            suspendedBlockedQ-> tail = process;
            suspendedBlockedQ-> count +=1;
        }


    }
}


int removePCB(pcb *process){
    if(process->state == ready){ //remove from readyQ queue
        if(readyQ-> count == 1){ //one thing in queue
            readyQ->head = NULL;
            readyQ->tail = NULL;
            process -> next = NULL;
            process -> prev = NULL;
            readyQ -> count -=1;
            return 1; //sucess
        }
        else if(process->next == NULL){//check tail 
            pcb *tmp = process -> prev;
            process -> prev = NULL;
            tmp -> next = NULL;
            readyQ -> tail = tmp;
            readyQ -> count -=1;
            return 1;//sucess

        }
        else if(process->prev == NULL){//check head
            pcb *tmp = process ->next;
            process -> next = NULL;
            tmp->prev = NULL;
            process -> prev = NULL;
            readyQ -> head = tmp;
            readyQ -> count -=1;
            return 1; 
        }
        else{//in middle somwhere
            pcb *prev = process -> prev;
            pcb *next = process -> next;
            prev->next = process ->next;
            next->prev = process ->prev;
            process -> prev = NULL;
            process -> next = NULL;
            readyQ -> count -=1;
            return 1; //sucess
        }
    }
    else if(process->state == suspendedReady){ //remove from suspendedreadyQ queue
        if(suspendedReadyQ-> count == 1){ //one thing in queue
            suspendedReadyQ->head = NULL;
            suspendedReadyQ->tail = NULL;
            process -> next = NULL;
            process -> prev = NULL;
            suspendedReadyQ -> count -=1;
            return 1; //sucess
        }
        else if(process->next == NULL){//check tail 
            pcb *tmp = process -> prev;
            process -> prev = NULL;
            tmp -> next = NULL;
            suspendedReadyQ -> tail = tmp;
            suspendedReadyQ -> count -=1;
            return 1;//sucess

        }
        else if(process->prev == NULL){//check head
            pcb *tmp = process ->next;
            process -> next = NULL;
            tmp->prev = NULL;
            process->prev = NULL;
            suspendedReadyQ -> head = tmp;
            suspendedReadyQ -> count -=1;
            return 1;
        }
        else{//in middle somwhere
            pcb *prev = process -> prev;
            pcb *next = process -> next;
            prev->next = process ->next;
            next->prev = process ->prev;
            process -> prev = NULL;
            process -> next = NULL;
            readyQ -> count -=1;
            return 1; //sucess
        }
    }
    else if(process->state == blocked){
        if(blockedQ-> count == 1){ //one thing in queue
            blockedQ->head = NULL;
            blockedQ->tail = NULL;
            process -> next = NULL;
            process -> prev = NULL;
            blockedQ -> count -=1;
            return 1; //sucess
        }
        else if(process->next == NULL){//check tail 
            pcb *tmp = process -> prev;
            process -> prev = NULL;
            tmp -> next = NULL;
            blockedQ -> tail = tmp;
            blockedQ -> count -=1;
            return 1;//sucess

        }
        else if(process->prev == NULL){//check head
            pcb *tmp = process ->next;
            process -> next = NULL;
            tmp->prev = NULL;
            process->prev = NULL;
            blockedQ -> head = tmp;
            blockedQ -> count -=1;
            return 1;
        }
        else{//in middle somwhere
            pcb *prev = process -> prev;
            pcb *next = process -> next;
            prev->next = process ->next;
            next->prev = process ->prev;
            process -> prev = NULL;
            process -> next = NULL;
            readyQ -> count -=1;
            return 1; //sucess
        } 
    }
    else if(process->state == suspendedBlocked){ 
       if(suspendedBlockedQ-> count == 1){ //one thing in queue
            suspendedBlockedQ->head = NULL;
            suspendedBlockedQ->tail = NULL;
            process -> next = NULL;
            process -> prev = NULL;
            suspendedReadyQ -> count -=1;
            return 1; //sucess
        }
        else if(process->next == NULL){//check tail 
            pcb *tmp = process -> prev;
            process -> prev = NULL;
            tmp -> next = NULL;
            suspendedBlockedQ -> tail = tmp;
            suspendedBlockedQ -> count -=1;
            return 1;//sucess

        }
        else if(process->prev == NULL){//check head
            pcb *tmp = process ->next;
            process -> next = NULL;
            tmp->prev = NULL;
            process->prev = NULL;
            suspendedBlockedQ -> head = tmp;
            suspendedBlockedQ -> count -=1;
            return 1;
        }
        else{//in middle somwhere
            pcb *prev = process -> prev;
            pcb *next = process -> next;
            prev->next = process ->next;
            next->prev = process ->prev;
            process -> prev = NULL;
            process -> next = NULL;
            readyQ -> count -=1;
            return 1; //sucess
        }
    }
    else{
        return 0; //error
    }

    return 0;
}