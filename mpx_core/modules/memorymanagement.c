
#include "commandhandler.h"
#include "mpx_supt.h"
#include <stdint.h>
#include <string.h>
#include <../include/core/io.h>
#include <../include/core/serial.h>
#include "polling_helper.h"
#include "pcb_internal.h"
#include "pcb_commands.h"
#include "alarm.h"
#include <string.h>
#include "mem_management.h"
list free_list;
list beginAddr;
list allocated_list;
u32int memory_start;

void init_heap(int size) {
  memory_start = kmalloc(size + sizeof(cmcb));

  cmcb *firstCmcb = (cmcb*) memory_start;
  firstCmcb->type = free;
  firstCmcb->size = size;
  firstCmcb->beginAddr = (int) memory_start + sizeof(cmcb);

  free_list.head = firstCmcb;
  free_list.head->next= NULL;
  free_list.head->prev = NULL;

  allocated_list.head = NULL;
  allocated_list.head->next = NULL;
  allocated_list.head->prev = NULL;


}
void allocate(u32int size) {
  cmcb *curr = free_list.head;

  u32int required_size = size + sizeof(cmcb);
  u32int curr_size;

  while(curr){ 
    if(curr->size>= required_size){
      curr_size = curr->size;
      break;
    }
    
  }

}


//return int for error checking?
int freeMemory(cmcb * toBeFreed){
    cmcb *currHeap = beginAddr.head;
    cmcb * mergePtr;
    //error checking
    if(toBeFreed->type != allocated || currHeap == NULL){
        return 1; //cmcb is not allocated
    }
    
    while(currHeap != NULL){
        if(currHeap == toBeFreed){
            //remove from allocated list
            cmcb * allocPtr = allocated_list.head;
            cmcb * new_alloc;

            if(allocPtr == NULL){
                return 1; //allocated list is empty
            }
            else if(toBeFreed == allocated_list.head){
                allocated_list.head = allocated_list.head->next; //edit sllocated_list directly
            }
            else if(allocPtr->prev == NULL && allocPtr->next == NULL){
                //one thing in allocated list
                allocated_list.head == NULL;
            }
            else{
                //find the right spot in the allocated list
                while(allocPtr!=NULL){
                    if(allocPtr == toBeFreed){
                        break;
                    }
                    allocPtr = allocPtr->next;
                }
                if(allocPtr->next == NULL){
                    //we are at (removing) the tail
                    new_alloc = allocPtr->prev;
                    new_alloc->next = NULL;
                }
                if(allocPtr -> prev == NULL){
                    //we are removing at the head
                    new_alloc = allocPtr->next;
                    new_alloc->prev = NULL;
                }

                else{
                    new_alloc = allocPtr ->next;
                    new_alloc ->prev = allocPtr->prev;
                }

                //reset allocated list
                while(new_alloc->prev!=NULL){
                    new_alloc = new_alloc->prev; //at head of new allocated list
                }

                //set allocated list's head equal to our new list's head
                allocated_list.head = new_alloc;
            }

            
            

            //place into free list
            cmcb * free_ptr = free_list.head;
            cmcb * new_free;
            allocPtr ->type = free;

            if(free_ptr == NULL){ //nothing in free list yet
                free_list.head = allocPtr; //place old node into actual free list
            }
            else{ //something in list
                if(free_ptr ->prev == NULL && free_ptr ->next == NULL){
                    //one thing in the free list - need to determine whether it gets inserted before or after
                    //loop through heap to find where the free block lives
                    cmcb * heapCtrl = beginAddr.head;
                    while(heapCtrl != NULL){
                        if(heapCtrl->type == free){
                            break; //break on the first (should be only) free block found
                        }
                        heapCtrl = heapCtrl->next;
                    }
                    //heap control now equals the free block
                    //look to the left of it
                    if(heapCtrl->prev == toBeFreed){
                        //gets inserted behind the only free block
                        free_list.head->prev = allocPtr;
                        
                    }
                    if(heapCtrl->next == toBeFreed){
                        free_list.head->next = allocPtr;
                    }
                }
                else{//more than one thing in free list
                    //loop through to find the correct freePtr
                    cmcb * heapCtrl = beginAddr.head;
                    while(heapCtrl != NULL){
                        if(heapCtrl == toBeFreed){
                            if(heapCtrl->prev->type == free && heapCtrl->next->type == free){
                                //needs to be inserted between these two in the free list
                                //loop through free list to find where it belongd
                                while(free_ptr!=NULL){
                                    if(free_ptr == heapCtrl->prev){
                                        //insert between these nodes in the free list
                                        new_free = allocPtr;
                                        new_free->prev = free_ptr;
                                        new_free->next = free_ptr->next;
                                        break;
                                    }
                                free_ptr= free_ptr->next;
                                }
                            }
                            else if(heapCtrl->prev->type == free){
                                //needs to be inserted after this node in the free list
                                        while(free_ptr!=NULL){
                                            if(free_ptr == heapCtrl->prev){
                                                //insert after this nodes in the free list
                                                new_free = allocPtr;
                                                new_free->prev = free_ptr;
                                                new_free->next = free_ptr->next;
                                                break;
                                            }
                                            free_ptr= free_ptr->next;
                                        }
                            }
                            else if(heapCtrl->next->type == free){
                                //needs to be inserted before this node in the free list
                                while(free_ptr!=NULL){
                                    if(free_ptr == heapCtrl->next){
                                        //insert before this node in the free list
                                        new_free = allocPtr;
                                        new_free->next = free_ptr;
                                        new_free->prev = free_ptr->prev;
                                        break;
                                    }
                                free_ptr= free_ptr->next;
                                }
                            }
                            else if(heapCtrl ->prev->type != free){
                                //loop left to see if the free block exists before
                                while(heapCtrl!=NULL){
                                    if(heapCtrl->type == free){
                                        break;
                                    }
                                    heapCtrl= heapCtrl->prev;
                                }
                                
                                 while(free_ptr!=NULL){
                                    if(free_ptr == heapCtrl){
                                        //insert after this node in the free list
                                        new_free = allocPtr;
                                        new_free->prev = free_ptr;
                                        new_free->next = free_ptr->prev;
                                        break;
                                    }
                                 free_ptr= free_ptr->next;
                                }

                            }
                            else if(heapCtrl ->next->type != free){
                                //loop right to see if the free block exists after
                                while(heapCtrl!=NULL){
                                    if(heapCtrl->type == free){
                                        break;
                                    }
                                    heapCtrl= heapCtrl->next;
                                }
                                
                                 while(free_ptr!=NULL){
                                    if(free_ptr == heapCtrl){
                                        //insert before this node in the free list
                                        new_free = allocPtr;
                                        new_free->next = free_ptr;
                                        new_free->prev = free_ptr->prev;
                                        break;
                                    }
                                 free_ptr= free_ptr->next;
                                }

                            }
                            
                        }


                        heapCtrl= heapCtrl->next;
                    }

                    //loop to head of new free and reset free list
                    while(new_free->prev !=NULL){
                        new_free= new_free->prev;
                    }

                    free_list.head = new_free;
                }   
             } 

             //reassign the node in the head to be free
             currHeap->type = free;
             //save this pointer for merging
             mergePtr = currHeap;
             //loop to head of the pointer's list
             while(currHeap->prev !=NULL){
                 currHeap = currHeap->prev;
             }

             beginAddr.head = currHeap;

            

        }
        currHeap= currHeap->next; //move on in loop
    }

    merge(mergePtr); //merge all of the lists   
    
}

void merge(cmcb * curr){
    cmcb * currPtr = curr; 
    cmcb * newly_merged;
    cmcb * newly_merged_free;
    u32int merged_size;
    cmcb * freePtr = free_list.head;

    //look to the left of the pointer
        if(curr->prev->type == free){
            newly_merged = curr;
            newly_merged -> size = curr->size + curr->prev->size;

            //merge free list
            
            while(freePtr != NULL){
                if(freePtr == curr){
                    newly_merged_free = freePtr;
                    newly_merged_free -> size = freePtr->size + freePtr->prev->size;
                    break;
                    
                }
                else{
                    freePtr = freePtr -> next;
                }
                
            }
            //merge heap
            newly_merged -> prev = currPtr ->prev->prev;
            newly_merged -> next = currPtr->next;
            
            merge(currPtr);
        }
        else if(curr->next->type == free){//look to the right 
            newly_merged = curr;
            newly_merged ->size = curr->size + curr->next->size;

            //merge free list
            while(freePtr != NULL){
                if(freePtr == curr){
                    newly_merged_free = freePtr;
                }
            }
            newly_merged -> next = currPtr -> next-> next;
            newly_merged -> prev = currPtr -> prev;
            merge(currPtr);
        }
        else{
            //loop to head of the lists
            while(currPtr != NULL){
                currPtr = currPtr -> prev;
            }

            while(freePtr != NULL){
                freePtr = freePtr -> prev;
            }


            beginAddr.head = currPtr;
            free_list.head = freePtr;
        }
}
        void show_list(int type)
        {
            cmcb * curr = NULL;

            if (type == 0){
                curr = free_list.head;
            }
            if (curr == NULL){
                char *msg;
                if (type == 0)
                {
                    msg = "\nThere's nothing in the free memory list!\n";
                }
                int msg_size = strlen(msg);
                sys_req(WRITE, DEFAULT_DEVICE, msg, &msg_size);
            }
            else
            {
                while (curr != NULL)
                {
                    char block_size[30];
                    char block_addr[30];
                    char *msg = "\nsize: "; // change the message to "size:""

                    int msg_size = strlen(msg);
                    sys_req(WRITE, DEFAULT_DEVICE, msg, &msg_size);

                    itoa(curr->size, block_size);
                    int block_msg_size = strlen(block_size);
                    sys_req(WRITE, DEFAULT_DEVICE, block_size, &block_msg_size);

                    char *addr_msg = "\naddress: ";
                    msg_size = strlen(msg);
                    sys_req(WRITE, DEFAULT_DEVICE, addr_msg, &msg_size);

                    itoa(curr->beginAddr, block_addr);
                    int block_addr_size = strlen(block_addr);
                    sys_req(WRITE, DEFAULT_DEVICE, block_addr, &block_addr_size);

                    curr = curr->next;
                }
            }
        }

