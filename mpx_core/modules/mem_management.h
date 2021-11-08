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
}cmbc;

typedef struct list{
    struct cmcb *head;
}list;

/**
 * Checks against the the list of times and runs the appropriate process
 * @param size the alarm selected for deletion.
 */
void init_heap();







#endif
