
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
