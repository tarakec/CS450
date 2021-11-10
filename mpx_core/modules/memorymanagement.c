
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
    cmcb *currHeap = heap.head;
    
    //error checking
    if(toBeFreed->type != allocated || currHeap == NULL){
        return 1; //cmcb is not allocated
    }
            //change the current node to free
            toBeFreed->type = free;

            //look to the left
            if(toBeFreed->prev->type == free){
                toBeFreed->size = toBeFreed->size +toBeFreed->prev->size; //merge the two blocks

                toBeFreed->prev = toBeFreed->prev->prev; //link list back together without old free block

            }

            //look right
            if(toBeFreed->next ->type == free){
                toBeFreed->size = toBeFreed->size +toBeFreed->next->size; //merge the two blocks

                toBeFreed->next = toBeFreed -> next->next; //link list back together with new free block

            }
       
    
}
<<<<<<< HEAD

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

=======
>>>>>>> 8fae0ca1a4650ff609fb3a143cb9f682c836cc0a
