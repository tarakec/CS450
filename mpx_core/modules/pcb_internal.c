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
        cmcb* transfer = (cmcb*) toBeFreed;
        
		i = sys_free_mem(transfer);

		return i;
    }   


pcb* setupPCB(char *name, int class, int priority){
		pcb* process = allocatePCB();

			if(priority<0 || priority>9){
                return process;   
        		}
				
			strcpy(process->name, name);
			process -> class_= class;
			process -> priority = priority;
            process -> state = ready;
            process -> stackTop = process->stack + stackSize - sizeof(context);

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
            }
            if(suspBlockedTail->next != NULL){
                suspBlockedTail = suspBlockedTail->next;
            }
            i++;
        }//not in suspeneded blocked

         //not found -- return NULL
        return NULL;
	}
   

void insertPCB(pcb *process){
    if(process -> state == ready){ //priority queue

        if(readyQ->count == 0){ //if it is an empty queue
            readyQ -> head = process;
            readyQ -> tail =process;
            process -> next = NULL;
            process -> prev = NULL;
            readyQ -> count += 1;
        }
        else if(readyQ -> count == 1){ //one thing in queue
             pcb* tmp1 = readyQ->head;

            if(tmp1 ->priority < process->priority){ //check priority
               //the priority of the existing process is less than process we're trying to insert
            //the new process is inserted after the existing process in the queue and pcb
                process -> prev = tmp1;
                process -> next = NULL;
                tmp1 -> next = process;
                readyQ -> head = process;
            }
            else if(tmp1-> priority >= process->priority){
                //the priority of existing process is greater than the process we're trying to insert
                //new process inserted before the existing one
                process -> next = tmp1;
                process->prev = NULL;
                tmp1->prev = process;
                readyQ -> tail = process;
            }
            readyQ->count+=1;
            tmp1 = NULL;
        }

        else{ //more than one thing in the queue
            pcb* tmp2 = readyQ->head;
            
            //loops through readyQ queue and finds the appropriate spot if the process's priority is larger than the readyQ queue tail

            while ((process->priority <= tmp2->priority) && tmp2->prev != NULL){
                tmp2 = tmp2->prev;    
            }
            //checks to see if we are at tail node
            if((tmp2->prev == NULL) && (process->priority <= tmp2->priority)){
                tmp2->prev = process;
                process -> next = tmp2;
                process -> prev = NULL;
                readyQ -> tail = process;
                  }

            else if((tmp2->next == NULL) && (process->priority > tmp2->priority)){// check if head
                tmp2->next = process;
                process->prev = tmp2;
                process->next = NULL;
                readyQ -> head = process;

            }
            else{ //somewhere in between
                pcb *next = tmp2->next;
                process->prev = tmp2;
                process -> next = next;
                tmp2->next = process;
                next->prev = process;

                //loop to find end of queue
                pcb *looper = process;
                while(looper->next != NULL){
                    
                        looper = looper->next;
        

                }//tmp (looper) should now equal its last process and have the included new process with it

                readyQ ->head = looper;
                looper = NULL;
            }
            tmp2 = NULL;
            readyQ ->count +=1;
        }
        
    
    } 
    else if(process -> state == suspendedReady){ //priority queue
       
        if(suspendedReadyQ->count == 0){ //if it is an empty queue
            suspendedReadyQ -> head = process;
            suspendedReadyQ -> tail =process;
            process -> next = NULL;
            process -> prev = NULL;
            suspendedReadyQ -> count += 1;
        }
        else if(suspendedReadyQ -> count == 1){ //one thing in queue
            pcb *tmp3 = suspendedReadyQ->head;

            if(tmp3 ->priority < process->priority){ //check priority
               //the priority of the existing process is less than process we're trying to insert
            //the new process is inserted after the existing process in the queue and pcb
                process -> prev = tmp3;
                process -> next = NULL;
                tmp3 -> next = process;
                suspendedReadyQ -> head = process;
            }
            else{
                //the priority of existing process is greater than the process we're trying to insert
                //new process inserted before the existing one
                process -> next = tmp3;
                process->prev = NULL;
                tmp3->prev = process;
                suspendedReadyQ -> tail = process;
            }
            suspendedReadyQ->count+=1;
            tmp3 = NULL;
        }
        else{ //more than one thing in the queue
           
           pcb *tmp4 = suspendedReadyQ->head;

            //loops through suspendedReadyQQ queue and finds the appropriate spot if the process's priority is larger than the readyQ queue tail
            while ((process->priority <= tmp4->priority) && tmp4->prev != NULL){ 
                tmp4 = tmp4->prev;
            }
            //checks to see if we are at tail node
            if((tmp4->prev == NULL) && (process->priority <= tmp4->priority)){
                tmp4->prev = process;
                process -> next = tmp4;
                process -> prev = NULL;
                suspendedReadyQ -> tail = process;
                  }
            else if((tmp4->next == NULL) && (process->priority > tmp4->priority)){// check if head
                tmp4->next = process;
                process->prev = tmp4;
                process->next = NULL;
                suspendedReadyQ -> head = process;

            }
            else{
                pcb *next = tmp4->next;
                process->prev = tmp4;
                process -> next = next;
                tmp4->next = process;
                next->prev = process;
                //loop to find end of pbc
                
                pcb *looper = process;
                while(looper->next != NULL){
                   
                    looper = looper->next;
                    
                }//tmp should now equal its last process and have the included new process with it

                suspendedReadyQ ->head = looper;
                looper = NULL;
            }
            tmp4 = NULL;
            suspendedReadyQ ->count +=1;
        }
        
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
    if(process->state == ready){ //remove from ready queue
        if(readyQ-> count == 1){ //one thing in queue
            readyQ->head = NULL;
            readyQ->tail = NULL;
            process -> next = NULL;
            process -> prev = NULL;
            readyQ -> count -=1;
            return 1; //sucess
        }
        else if(process->next == NULL){//check head
            pcb *tmp = process -> prev;
            process -> prev = NULL;
            tmp -> next = NULL;
            readyQ -> head = tmp;
            readyQ -> count -=1;
            return 1;//sucess

        }
        else if(process->prev == NULL){//check tail
            pcb *tmp = process ->next;
            process -> next = NULL;
            tmp->prev = NULL;
            process -> prev = NULL;
            readyQ -> tail = tmp;
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
        else if(process->next == NULL){//check head
            pcb *tmp = process -> prev;
            process -> prev = NULL;
            tmp -> next = NULL;
            suspendedReadyQ -> head = tmp;
            suspendedReadyQ -> count -=1;
            return 1;//sucess

        }
        else if(process->prev == NULL){//check tail
            pcb *tmp = process ->next;
            process -> next = NULL;
            tmp->prev = NULL;
            process->prev = NULL;
            suspendedReadyQ -> tail = tmp;
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
            suspendedReadyQ -> count -=1;
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
        else if(process->next == NULL){//check head
            pcb *tmp = process -> prev;
            process -> prev = NULL;
            tmp -> next = NULL;
            blockedQ -> head = tmp;
            blockedQ -> count -=1;
            return 1;//sucess

        }
        else if(process->prev == NULL){//check tail
            pcb *tmp = process ->next;
            process -> next = NULL;
            tmp->prev = NULL;
            process->prev = NULL;
            blockedQ -> tail = tmp;
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
            blockedQ -> count -=1;
            return 1; //sucess
        } 
    }
    else if(process->state == suspendedBlocked){ 
       if(suspendedBlockedQ-> count == 1){ //one thing in queue
            suspendedBlockedQ->head = NULL;
            suspendedBlockedQ->tail = NULL;
            process -> next = NULL;
            process -> prev = NULL;
            suspendedBlockedQ -> count -=1;
            return 1; //sucess
        }
        else if(process->next == NULL){//check head
            pcb *tmp = process -> prev;
            process -> prev = NULL;
            tmp -> next = NULL;
            suspendedBlockedQ -> head = tmp;
            suspendedBlockedQ -> count -=1;
            return 1;//sucess

        }
        else if(process->prev == NULL){//check tail
            pcb *tmp = process ->next;
            process -> next = NULL;
            tmp->prev = NULL;
            process->prev = NULL;
            suspendedBlockedQ -> tail = tmp;
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
            suspendedBlockedQ -> count -=1;
            return 1; //sucess
        }
    }
    else{
        return 0; //error
    }

    return 0;
}