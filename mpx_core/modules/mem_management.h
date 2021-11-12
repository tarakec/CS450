#ifndef _MEM_H
#define _MEM_H

#define free 0
#define allocated 1

typedef struct cmcb{
    int type;
    u32int beginAddr;
    u32int size;
    struct cmcb *next;
    struct cmcb *prev;
}cmcb;

typedef struct list{
    struct cmcb *head;
}list;

/**
 * Initializes the heap of memory
 * @param size is the total size of the heap to be created.
 */
void init_heap(u32int size);

/**
 * Allocates memory in from the heap
 * @param size the amount of bytes to be allocated from the heap
 */
u32int allocateMemory(u32int size);

/**
 * frees a particular block of memory that was previously allocated
 * @param toBeFreed a pointer to an address in memory
 */
int freeMemory(cmcb* toBeFreed);

/**
 * traverses through a list that will show the address of the block as well as the size of the block for free memory
 * 
 */
int showFree();
/**
 * traverses through a list that will show the address of the block as well as the size of the block for allocated memory
 * 
 */
int showAllocated();

/**
 * will return true or false as to whether the heap is empty/ contains only free memory
 * 
 */
int isEmpty();

/**
 * takes the address and finds the pointer that points at the address
 * @param address an address in memory in the heap
 */
cmcb* addressCheck(u32int address);

#endif
