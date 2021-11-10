
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
u32int heapSize = 0;

void init_heap(u32int size) {
  memory_start = kmalloc(size + sizeof(cmcb));

  cmcb *firstCmcb = (cmcb*) memory_start;
  firstCmcb->type = free;
  firstCmcb->size = size;
  firstCmcb->beginAddr = (u32int) memory_start + sizeof(cmcb);

  heapPtr.head = firstCmcb;
  heapPtr.head->next = NULL;
  heapPtr.head->prev = NULL;
 
  heapSize += firstCmcb->size;

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

  heapSize-=required_size;

  //case where the heap is empty 
  if(curr == heapPtr.head && curr->next == NULL){
      cmcb *allocate = heapPtr.head;
      cmcb *leftoverFree =(cmcb*) (required_size + sizeof(cmcb));

      allocate->type = allocated;
      allocate->size = size;


      //if there is leftover free memory after the allocation
      if(curr->size > required_size){

          allocate->next = leftoverFree;
          leftoverFree->type = free;
          leftoverFree->beginAddr = required_size + sizeof(cmcb);
          leftoverFree-> size = heapSize;
          leftoverFree->prev = allocate;
          leftoverFree->next = NULL;
      }

      return 0;
  }
      

// case where the curr is at the tail of the heap an the list is not empty
  else if(curr != heapPtr.head && curr->next == NULL){

      cmcb *allocate = curr;
     
      allocate->type = allocated;
      allocate->size = size;
      allocate->prev = curr->prev;
      allocate->beginAddr = (u32int) curr + sizeof(cmcb);
      cmcb *leftoverFree = (cmcb*) (allocate->beginAddr + size + sizeof(cmcb));

      //if there is leftover free memory after the allocation
      if(curr->size > required_size){

          allocate->next = leftoverFree;
          
          leftoverFree->type = free;
          leftoverFree->beginAddr = allocate->beginAddr + size + sizeof(cmcb);
          leftoverFree->size = heapSize;
          leftoverFree->prev = allocate;
          leftoverFree->next = NULL;
      }
      else{
        //if there is no leftover free memory after the allocation
        allocate->next = NULL;
      }

      return 0;
      

  }
  //case where the curr is at the heap head but the list is not empty
  else if(curr == heapPtr.head && curr->next != NULL){
      cmcb *allocate = heapPtr.head;
      

      allocate->type = allocated;
      allocate->size = size;

      cmcb *leftoverFree = (cmcb*) (required_size + sizeof(cmcb));

      //if there is leftover free memory after the allocation
      if(curr->size > required_size){

          allocate->next = leftoverFree;
          leftoverFree->type = free;
          leftoverFree->beginAddr = required_size + sizeof(cmcb);
          leftoverFree-> size = heapSize;
          leftoverFree->prev = allocate;
          leftoverFree->next = heapPtr.head->next;
      }

      return 0;
      
  }
  else{

      //grab the next and prev cmcbs of the allocation
      cmcb *curr_next = curr->next;
      cmcb *curr_prev = curr->prev;


      //create the allocated block
      cmcb *allocate = curr;
      allocate->type = allocated;
      allocate->beginAddr = (u32int) curr + sizeof(cmcb);
      allocate->size = size;

      //leftover free memory after the allocation
      if(curr->size > required_size){

        cmcb *leftoverFree = (cmcb*) (allocate->beginAddr + size + sizeof(cmcb));
        leftoverFree->type = free;
        leftoverFree->beginAddr = allocate->beginAddr + size + sizeof(cmcb);
        leftoverFree->size = heapSize;
        leftoverFree->prev = allocate;
        leftoverFree->next = curr_next;
        allocate->next= leftoverFree;
        allocate->prev = curr_prev;
        curr_prev->next = allocate;
        curr_next->prev = leftoverFree;
      }

      //no leftover free memory after the allocation
      else{

        //assign the next and prev for allocated block
        allocate->next = curr_next;
        allocate->prev = curr_prev;


        //connect the node into the heap
        curr_prev->next = allocate;
        curr_next->prev = allocate;
      }

      return 0;
  }
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
            
            //look right
            if(toBeFreed->next ->type == free){
                toBeFreed->size = toBeFreed->size +toBeFreed->next->size; //merge the two blocks

                toBeFreed->next = toBeFreed -> next->next; //link list back together with new free block

            }
  
            //look to the left
            if(toBeFreed->prev->type == free){
                toBeFreed->size = toBeFreed->size +toBeFreed->prev->size; //merge the two blocks

                toBeFreed->prev = toBeFreed->prev->prev; //link list back together without old free block

            }

            

            return 0;
       
    
}
