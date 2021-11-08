
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
    cmcb *curr = beginAddr.head;

    //error checking
    if(toBeFreed->type != allocated || curr == NULL){
        return 1; //cmcb is not allocated
    }
    
    while(curr != NULL){
        if(curr == toBeFreed){
            //remove from allocated list
            cmcb * alloc = allocated_list.head;
            cmcb * new_alloc;
            while(alloc !=NULL){
                if(alloc == toBeFreed){
                    break;
                }
                //build new list
                new_alloc = alloc;
                alloc = alloc->next;
            }
            if(alloc == NULL){
                return 1; //allocated list is empty
            }
            else if(alloc == allocated_list.head){
                allocated_list.head = allocated_list.head->next;
            }
            else{
                new_alloc->next = alloc->next;
                //traverse back to new_alloc's head to overwrite the allocated_list
                while(new_alloc->prev!=NULL){
                    new_alloc = new_alloc->prev; //at head of new allocated list
                }

                //set allocated list's head equal to our new list's head
                allocated_list.head = new_alloc;
            }

            //place into free list
            cmcb * free_ptr = free_list.head;
            cmcb * new_free;
            while(free_ptr != NULL){
                if(free_ptr->next == curr->next){
                    //if the next nodes are the same the new node (newly feed) belongs after the ptr
                    break;
                }
                free_ptr = free_ptr->next;

            //nothing in free list yet
            if(free_ptr == NULL){
                free_list.head = alloc; //place old node into fee list
            }
            else{
                new_free = free_ptr->next;
                new_free->prev = curr; 
                free_ptr ->next = new_free->prev;

                //traverse to head of new free list
                while(free_ptr->prev!=NULL){
                    free_ptr = free_ptr->prev;
                }
                free_list.head = free_ptr;
            }


            //this might be helpful to merging?
            // cmcb *free_temp = free_list.head;
            // while(free_temp != NULL){
            //     if(free_temp == alloc){
            //         free_temp->size += alloc->size;
            //         break;
            //     }
            //     free_temp = free_temp->next;
            // }

            //merge adjecent free blocks
            cmcb * curr_ptr = curr; //make pointer to curr
            cmcb * merged_free = curr;
            u32int merging_size;

            //look left of the current place in the heap
            while(curr_ptr->prev != NULL){
                if(curr_ptr->prev->type != allocated){
                     merging_size = merging_size + curr_ptr->prev->size;
                     merged_free ->size = merged_free->size + merging_size;

                }
                 curr_ptr=curr_ptr->prev;   
            }

            //look to the right of the current place in the heap
            //reset curr_ptr
            curr_ptr = curr;
            while(curr_ptr->next != NULL){
                if(curr_ptr->next->type != allocated){
                    merging_size = merging_size + curr_ptr->next->size;
                    merged_free ->size = merged_free->size + merging_size;
                }
                curr_ptr = curr_ptr->next;
            }


            //now we have looked left and right and found the total size to add to our new heap
            

            
            

            


        }
    }
}
