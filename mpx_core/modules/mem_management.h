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
 * Checks against the the list of times and runs the appropriate process
 * @param size the alarm selected for deletion.
 */
void init_heap(u32int size);

int allocateMemory(u32int size);

int freeMemory(cmcb* toBeFreed);

int showFree();

int showAllocated();

int isEmpty();

cmcb* addressCheck(u32int address);



#endif
