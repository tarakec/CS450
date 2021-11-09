
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
#include <../include/mem/heap.h>

list heapPtr;
u32int memory_start;


void init_heap(u32int size) {
  memory_start = kmalloc(size + sizeof(cmcb));

  cmcb *firstCmcb = (cmcb*) memory_start;
  firstCmcb->type = free;
  firstCmcb->size = size;
  firstCmcb->beginAddr = (u32int) memory_start + sizeof(cmcb);

  heapPtr.head = firstCmcb;
  heapPtr.head->next = NULL;
  heapPtr.head->prev = NULL;
 


}

//return 0 for error, 1 for success ?
int allocateMemory(u32int size) {
  cmcb *curr =heapPtr.head;


  //grabbing the required size for the entire block allocation
  u32int required_size = size + sizeof(cmcb);


  //while there is a curr 
  while(curr){ 

    //if the current node's size is greater than the required size and if the current node is free
    if(curr->size>= required_size && curr->type == free){

     //break out of the loop
      break;
    }

    //traverse
    curr = curr->next;


    //if reach the end of the heap and still not broke from loop, then not enough memory for allocation
    if (curr->next == NULL){
        serial_print("Not enough memory for the allocation.");
        return 1;
    }
    
  }

  

  //case where the heap is empty 
  cmcb *allocate = curr;
  allocate->type = allocated;


  //if there is leftover free memory after the allocation
  if(curr->size > required_size + 100){

      cmcb *leftoverFree =(cmcb*) (required_size + (u32int) allocate);
     
      leftoverFree->type = free;
      leftoverFree->beginAddr = required_size + sizeof(cmcb) + (u32int) allocate;

      //size ???
      leftoverFree-> size = curr->size - sizeof(cmcb) - size;
      leftoverFree->prev = allocate;
      leftoverFree->next = curr->next;
      allocate->next = leftoverFree;
      if (leftoverFree->next != NULL){
        leftoverFree->next->prev = leftoverFree;
      }
      allocate->size = size;
  }

      return 0;
}

//return int for error checking?
int freeMemory(cmcb * toBeFreed){
    cmcb *currHeap = heapPtr.head;
    
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

            return 0;
       
    
}
