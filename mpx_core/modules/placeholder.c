list heap;
u32int memory_start;
u32int heapSize = 0;

void init_heap(u32int size) {
  memory_start = kmalloc(size + sizeof(cmcb));

  cmcb *firstCmcb = (cmcb*) memory_start;
  firstCmcb->type = free;
  firstCmcb->size = size;
  firstCmcb->beginAddr = (u32int) memory_start + sizeof(cmcb);

  heap.head = firstCmcb;
  heap.next = NULL;
  heap.prev = NULL;
 
  heapSize += firstCmcb->size;

}

//return 0 for error, 1 for success ?
int allocateMemory(u32int size) {
  cmcb *curr = heap.head;


  //grabbing the required size for the entire block allocation
  u32int required_size = size + sizeof(cmcb);
  u32int curr_size;


  //while there is a curr 
  while(curr){ 

    //if the current node's size is greater than the required size and if the current node is free
    if(curr->size>= required_size && curr->type == free){

        //grab the size and break out the loop
      curr_size = curr->size;
      break;
    }

    //traverse
    curr = curr.next;


    //if reach the end of the heap and still not broke from loop, then not enough memory for allocation
    if (curr.next == NULL){
        serial_print("Not enough memory for the allocation.");
        return 0;
    }
    
  }

  heapSize-=required_size;

  //case where the heap is empty 
  if(curr == heap.head && curr.next == NULL){
      cmcb *allocated = heap.head;
      cmcb *leftoverFree;

      allocated->type = allocated;
      allocated->size = size;


      //if there is leftover free memory after the allocation
      if(curr->size > required_size){

          allocated->next = leftoverFree;
          leftoverFree->type = free;
          leftoverFree->beginAddr = required_size + sizeof(cmcb);
          leftoverFree-> size = heapSize;
          leftoverFree->prev = allocated;
          leftoverFree->next = NULL;
      }
      else{
        //if there is no leftover free memory after the allocation
        allocated->next == NULL
      }
  }
      

// case where the curr is at the tail of the heap an the list is not empty
  else if(curr != heap.head && curr->next == NULL){

      cmcb *allocated = curr;
      cmcb *leftoverFree;

      allocated->type = allocated;
      allocated->size = size;
      allocated->prev = curr->prev;
      allocated->beginAddr = (u32int) curr + sizeof(cmcb);

      //if there is leftover free memory after the allocation
      if(curr->size > required_size){

          allocated->next = leftoverFree;
          
          leftoverFree->type = free;
          leftoverFree->beginAddr = allocated->beginAddr + size + sizeof(cmcb);
          leftoverFree->size = heapSize;
          leftoverFree->prev = allocated;
          leftoverFree->next = NULL;
      }
      else{
        //if there is no leftover free memory after the allocation
        allocated->next = NULL;
      }
      

  }
  //case where the curr is at the heap head but the list is not empty
  else if(curr == heap.head && curr->next != NULL){
      cmcb *allocated = heap.head;
      cmcb *leftoverFree;

      allocated->type = allocated;
      allocated->size = size;


      //if there is leftover free memory after the allocation
      if(curr->size > required_size){

          allocated->next = leftoverFree;
          leftoverFree->type = free;
          leftoverFree->beginAddr = required_size + sizeof(cmcb);
          leftoverFree-> size = heapSize;
          leftoverFree->prev = allocated;
          leftoverFree->next = heap.head->next;
      }
      else{
        //if there is no leftover free memory after the allocation
        allocated->next == heap.head->next
      }
      
  }
  else{

      //grab the next and prev cmcbs of the allocation
      cmcb *curr_next = curr->next;
      cmcb *curr_prev = curr->prev;


      //create the allocated block
      cmcb *allocated = curr;
      allocated->type = allocated;
      allocated->beginAddr = (u32int) curr + sizeof(cmcb);
      allocated->size = size;

      //leftover free memory after the allocation
      if(curr->size > required_size){

        cmcb leftoverFree;
        leftoverFree->type = free;
        leftoverFree->beginAddr = allocated->beginAddr + size + sizeof(cmcb);
        leftoverFree->size = heapSize;
        leftoverFree->prev = allocated;
        leftoverFree->next = curr_next;
        allocated->next= leftoverFree;
        allocated->prev = curr_prev;
        curr_prev->next = allocated;
        curr_next->prev = leftoverFree;
      }

      //no leftover free memory after the allocation
      else{

        //assign the next and prev for allocated block
        allocated->next = curr_next;
        allocated->prev = curr_prev;


        //connect the node into the heap
        curr_prev->next = allocated;
        curr_next->prev = allocated;
      }
  }
}