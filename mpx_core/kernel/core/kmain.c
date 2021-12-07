/*
  ----- kmain.c -----

  Description..: Kernel main. The first function called after
      the bootloader. Initialization of hardware, system
      structures, devices, and initial processes happens here.

      Initial Kernel -- by Forrest Desjardin, 2013,
      Modifications by:    Andrew Duncan 2014,  John Jacko 2017
      				Ben Smith 2018, and Alex Wilson 2019
*/

#include <stdint.h>
#include <string.h>
#include <system.h>

#include <core/io.h>
#include <core/serial.h>
#include <core/tables.h>
#include <core/interrupts.h>
#include <mem/heap.h>
#include <mem/paging.h>

#include "modules/mpx_supt.h"
#include "modules/commandhandler.h"
#include "modules/pcb_internal.h"
#include "modules/pcb_commands.h"
#include "modules/mem_management.h"
#include "modules/mpx_R6.h"

#define dev COM1

void kmain(void)
{
   extern uint32_t magic;
   // Uncomment if you want to access the multiboot header
   // extern void *mbd;
   // char *boot_loader_name = (char*)((long*)mbd)[16];


   // 0) Initialize Serial I/O
   // functions to initialize serial I/O can be found in serial.c
   // there are 3 functions to call

   init_serial(COM1);
   set_serial_out(COM1);
   set_serial_in(COM1);

   klogv("Starting MPX boot sequence...");
   klogv("Initialized serial I/O on COM1 device...");

   // 1) Initialize the support software by identifying the current
   //     MPX Module.  This will change with each module.
   // you will need to call mpx_init from the mpx_supt.c

    mpx_init(MEM_MODULE);
   mpx_init(IO_MODULE);


   // 2) Check that the boot was successful and correct when using grub
   // Comment this when booting the kernel directly using QEMU, etc.
   if ( magic != 0x2BADB002 ){
     //kpanic("Boot was not error free. Halting.");
   }

   // 3) Descriptor Tables -- tables.c
   //  you will need to initialize the global
   // this keeps track of allocated segments and pages

   init_gdt();
   init_idt();
   sti();
   

   klogv("Initializing descriptor tables...");

    // 4)  Interrupt vector table --  tables.c
    // this creates and initializes a default interrupt vector table
    // this function is in tables.c

    init_irq();
    init_pic();

    klogv("Interrupt vector table initialized!");


   // 5) Virtual Memory -- paging.c  -- init_paging
   //  this function creates the kernel's heap
   //  from which memory will be allocated when the program calls
   // sys_alloc_mem UNTIL the memory management module  is completed
   // this allocates memory using discrete "pages" of physical memory
   // NOTE:  You will only have about 70000 bytes of dynamic memory
   //




   klogv("Initializing virtual memory...");

   u32int size = 50000;
   init_heap(size);


   com_open(1200);

   sys_set_malloc(allocateMemory);
   sys_set_free(freeMemory); 

   allocateQueues();
   init_paging();


   // 6) Call YOUR command handler -  interface method
   klogv("Transferring control to commhand...");

   

  pcb* c = load_proc("commhand", &command_handler);
  pcb* i = load_proc("idle", &idle);

  c->priority = 9;
  c->state = ready;

  i->priority = 1;
  i->state = ready;

  insertPCB(c);
  insertPCB(i);
  setHours(getHours()-4);
  yield();
  //command_handler();


   // 7) System Shutdown on return from your command handler
   klogv("Starting system shutdown procedure...");

   /* Shutdown Procedure */
   klogv("Shutdown complete. You may now turn off the machine. (QEMU: C-a x)");
   hlt();
}
