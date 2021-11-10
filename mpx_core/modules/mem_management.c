
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
    if (curr == NULL){
        serial_print("\nNot enough memory for the allocation.\n\n");
        return 0;
    }
    
  }


  cmcb *allocate = curr;
  allocate->type = allocated;


  //if there is leftover free memory after the allocation
  if(curr->size > required_size + 100){

      cmcb *leftoverFree =(cmcb*) (required_size + (u32int) allocate);
     
      leftoverFree->type = free;
      leftoverFree->beginAddr = required_size + sizeof(cmcb) + (u32int) allocate;

      
      leftoverFree-> size = curr->size - sizeof(cmcb) - size;
      leftoverFree->prev = allocate;
      leftoverFree->next = curr->next;
      allocate->next = leftoverFree;
      if (leftoverFree->next != NULL){
        leftoverFree->next->prev = leftoverFree;
      }
      allocate->size = size;
  }


    serial_print("\nAllocation was sucessfull..\n\n");
      return 1;
}

//return int for error checking?
int freeMemory(cmcb * toBeFreed){
    cmcb *currHeap = heapPtr.head;
    
    //error checking
    if(toBeFreed->type != allocated || currHeap == NULL){

        //serial_print("Memory is not allocated...");
        return 1; //cmcb is not allocated
    }

            //change the current node to free
            toBeFreed->type = free;

            //look to the left
            if(toBeFreed->prev->type == free){
                toBeFreed->size = toBeFreed->size +toBeFreed->prev->size + sizeof(cmcb); //merge the two blocks

                toBeFreed->prev = toBeFreed->prev->prev; //link list back together without old free block

            }
            //look right
            if(toBeFreed->next->type == free){
                toBeFreed->size = toBeFreed->size +toBeFreed->next->size + sizeof(cmcb); //merge the two blocks

                toBeFreed->next = toBeFreed -> next->next; //link list back together with new free block

            }
  
            

            
            serial_print("\nMemory was freed...\n\n");

            return 0;
       
    
}

int showFree(){

    cmcb* curr = heapPtr.head;

    //if the head is equal to NULL, hasn't been created yet
    if (curr == NULL){
        char *msg;
        msg = "\nHeap hasn't been initialized!\n\n";
        int msg_size = strlen(msg);
        sys_req(WRITE, DEFAULT_DEVICE, msg, &msg_size);

        return 0;
     }
    else{

        while (curr != NULL){

            if(curr->type == free){

                int misc = 30;
                char block_size[30];
                char block_addr[30];
                char *msg = "\nSize: "; // change the message to "size:""

                int msg_size = strlen(msg);
                sys_req(WRITE, DEFAULT_DEVICE, msg, &msg_size);

                itoa(curr->size, block_size);
                int block_msg_size = strlen(block_size);
                sys_req(WRITE, DEFAULT_DEVICE, block_size, &block_msg_size);

                char *addr_msg = "\nAddress: ";
                msg_size = strlen(msg);
                sys_req(WRITE, DEFAULT_DEVICE, addr_msg, &msg_size);

                itoa(curr->beginAddr, block_addr);
                int block_addr_size = strlen(block_addr);
                sys_req(WRITE, DEFAULT_DEVICE, block_addr, &block_addr_size);
                sys_req(WRITE,DEFAULT_DEVICE,"\n",&misc);
                sys_req(WRITE,DEFAULT_DEVICE,"\n",&misc);

                curr = curr->next;

                memset(block_size,'\0', 30);
                memset(block_addr,'\0',30);
            }
            else{
                curr = curr->next;
            }
           
        }

        return 1;
    }
}

int showAllocated(){

cmcb *curr = heapPtr.head;
    
    if (curr == NULL){
        char *msg;
        msg = "\nHeap hasn't been initialized!\n\n";
        int msg_size = strlen(msg);
        sys_req(WRITE, DEFAULT_DEVICE, msg, &msg_size);
        return 0;
     }


    //check if empty
      if(isEmpty()){
            char *empty_msg="\nThere is no allocated memory, the heap is empty!\n\n";
            int empty_msg_size = strlen(empty_msg);
            sys_req(WRITE, DEFAULT_DEVICE, empty_msg, &empty_msg_size);
            return 0;
          }

      //otherwise go through  
      else{
        
        while(curr!=NULL){

            if(curr->type==allocated){
              int misc = 30;  
              char block_size[30];
              char block_add[30];

              //print size
              char *size_msg="\nSize: ";
              int size_msg_size = strlen(size_msg);
              sys_req(WRITE, DEFAULT_DEVICE, size_msg, &size_msg_size);

              //print actual size
              itoa(curr->size, block_size);
              int block_size_len = strlen(block_size);
              sys_req(WRITE, DEFAULT_DEVICE, block_size, &block_size_len);

              //print address message
              char *add_msg="\nAddress: ";
              int add_msg_size = strlen(add_msg);
              sys_req(WRITE, DEFAULT_DEVICE, add_msg, &add_msg_size);

              //print actual address
              itoa(curr->beginAddr, block_add);
              int block_add_size = strlen(block_add);
              sys_req(WRITE, DEFAULT_DEVICE, block_add, &block_add_size);
              sys_req(WRITE,DEFAULT_DEVICE,"\n",&misc);
              sys_req(WRITE,DEFAULT_DEVICE,"\n",&misc);

              curr = curr->next;

              memset(block_size,'\0', 30);
              memset(block_add,'\0',30);

              }
              //if type is free move on to next block
            else{
              curr = curr->next;
            }  
        }

        return 1;
    }
}

int isEmpty(){
    if(heapPtr.head->next == NULL && heapPtr.head->type == free){
        return 1;
    }
    else{
        return 0;
    }
}

cmcb* addressCheck(u32int address){
    cmcb* curr = heapPtr.head;

    while(curr!=NULL){
        if (curr->beginAddr == address && curr->type == allocated){
            return curr;
        }
        else{
            curr = curr->next;
        }

       
    }

     curr = NULL;
     serial_print("Couldn't locate the free block with the address given...\n");
     return curr;
}
