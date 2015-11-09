#include "paging.h"
#include "video.h"
#include "mem.h"
#include "kernel.h"

#define NEGATE(x) (x == 0 ? 1 : 0)

void wait(){
  int i = 10;
  while(i--);
}

void vmm_initialize() {
  // Bit 31 of CR0, PGE (Bit 7), PAE (Bit 5), and PSE (Bit 4) of CR4 are set in pureg4.asm
  // Create page directories
  // Load CR3
  video_write_string(KERNEL_CONSOLE, "OLD CR3: 0x");
  video_write_hex(KERNEL_CONSOLE, readCR3());
  video_write_nl(KERNEL_CONSOLE);
  writeCR3((uint64_t)new_pml4(0));
  video_write_string(KERNEL_CONSOLE, "NEW CR3: 0x");
  video_write_hex(KERNEL_CONSOLE, readCR3());
  video_write_nl(KERNEL_CONSOLE);
}

/* Creates a new level 4 directory
 * param @us == 0 means kernel, other means user
 */
PM_L4_TABLE* new_pml4(int us) {
  int is_kernel = (us == 0) ? 0 : 1;
  PM_L4_TABLE* l4_table = (PM_L4_TABLE*)pmm_page_alloc();
  PM_L3_TABLE* l3_table = (PM_L3_TABLE*)pmm_page_alloc();
  PM_L2_TABLE* l2_table = (PM_L2_TABLE*)pmm_page_alloc();
  /* Make L4 point to L3 */
  l4_table->table[0].p = 1;
  l4_table->table[0].rw = 1;
  l4_table->table[0].us = is_kernel;
  l4_table->table[0].address = (uint64_t)((uint64_t)l3_table/0x1000);
  /* Make L3 point to L2 */
  l3_table->table[0].p = 1;
  l3_table->table[0].rw = 1;
  l3_table->table[0].us = is_kernel;
  l3_table->table[0].address = (uint64_t)((uint64_t)l2_table/0x1000);
  generic_l2_table(l2_table, us);
  return l4_table;
}

/* This function fills the first 6 entries of an level 2 table that contains the kernel code */
void generic_l2_table(PM_L2_TABLE* table, int us) {
  int is_kernel = (us == 0) ? 0 : 1;
  int i;
  for (i = 0; i < 6; i++) {
    if (i == 4 || i == 3) {
      // Mark the heap of the kernel as writable
      table->table[i].p = 1;
      table->table[i].rw = 1;
      table->table[i].us = 0;
      table->table[i].address = (uint64_t)((uint64_t)identity_l1_map(i, 1, 0)/0x1000);
    } else {
      table->table[i].p = 1;
      table->table[i].rw = NEGATE(is_kernel);
      table->table[i].us = 0;
      table->table[i].address = (uint64_t)((uint64_t)identity_l1_map(i, NEGATE(is_kernel), 0)/0x1000);
    }
  }
}

/* Given a level 2 table, identity page the 2mb that corresponds to that table */
PM_L1_TABLE* identity_l1_map(int first_l2_table_idx, int rw, int us) {
  PM_L1_TABLE* l1_table = (PM_L1_TABLE*)pmm_page_alloc();
  int i;
  for (i = 0; i < 512; i++) {
    l1_table->table[i].p = 1;
    l1_table->table[i].rw = rw;
    l1_table->table[i].us = us;
    l1_table->table[i].address = ((first_l2_table_idx*0x200000) + (i*0x1000))/0x1000;
  }
  return l1_table;
}
/*
US RW  P - Description
0  0  0 - Supervisory process tried to read a non-present page entry
0  0  1 - Supervisory process tried to read a page and caused a protection fault
0  1  0 - Supervisory process tried to write to a non-present page entry
0  1  1 - Supervisory process tried to write a page and caused a protection fault
1  0  0 - User process tried to read a non-present page entry
1  0  1 - User process tried to read a page and caused a protection fault
1  1  0 - User process tried to write to a non-present page entry
1  1  1 - User process tried to write a page and caused a protection fault
*/
void page_fault_handler(uint64_t error_code, uint64_t cr2) {
  intsoff();
  // CR2 contains the virtual address that caused the fault
  video_write_string(KERNEL_CONSOLE, "PAGE FAULT ");
  video_write_string(KERNEL_CONSOLE, "CR2: 0x");
  video_write_hex(KERNEL_CONSOLE, cr2);
  video_write_nl(KERNEL_CONSOLE);
  //PAGE_FAULT_ERROR_CODE* error = (PAGE_FAULT_ERROR_CODE*)error_code;
  video_write_string(KERNEL_CONSOLE, "ERROR: ");
  video_write_hex(KERNEL_CONSOLE, error_code);
  video_write_nl(KERNEL_CONSOLE);
  VirtualAddress* aux;
  aux->physical_offset = cr2 & 0x0000000000000FFF;
  aux->pm_l1_offset    = cr2 & 0x00000000001FF000;
  aux->pm_l2_offset    = cr2 & 0x000000003FE00000;
  aux->pm_l3_offset    = cr2 & 0x0000007FC0000000;
  aux->pm_l4_offset    = cr2 & 0x0000FF8000000000;

  video_write_string(KERNEL_CONSOLE, "physical: ");
  video_write_hex(KERNEL_CONSOLE, aux->physical_offset);
  video_write_nl(KERNEL_CONSOLE);
  video_write_string(KERNEL_CONSOLE, "l1: ");
  video_write_hex(KERNEL_CONSOLE, aux->pm_l1_offset);
  video_write_nl(KERNEL_CONSOLE);
  video_write_string(KERNEL_CONSOLE, "l2: ");
  video_write_hex(KERNEL_CONSOLE, aux->pm_l2_offset);
  video_write_nl(KERNEL_CONSOLE);
  video_write_string(KERNEL_CONSOLE, "l3: ");
  video_write_hex(KERNEL_CONSOLE, aux->pm_l3_offset);
  video_write_nl(KERNEL_CONSOLE);
  video_write_string(KERNEL_CONSOLE, "l4: ");
  video_write_hex(KERNEL_CONSOLE, aux->pm_l4_offset);
  video_write_nl(KERNEL_CONSOLE);
  while(1);

  add_page(aux, 0, 1);
  if (error_code & 0x01){
    if (error_code & 0x02){
      add_page(aux, 0, 1);
    } else {
      if (cr2 >= (22*0x1000) && cr2 < (32*0x1000)){
         //The process is asking for space in the stack or heap
        add_page(aux, 1, 1);
      } else {
        // KEEL DAT PROCESS
        video_write_string(KERNEL_CONSOLE, "TE ESTAS PASANDO DEL STACK!!");
        while(1);
      }
    }
  }
  intson();
}

/* Adds all the unpresent pages of a virtual address */
uint64_t add_page(VirtualAddress* addr, int us, int rw) {
  PM_L4_TABLE* l4_table = (PM_L4_TABLE*)readCR3();
  PM_L3_TABLE* l3_table = get_l3_table(l4_table, addr->pm_l4_offset, us, rw);
  PM_L2_TABLE* l2_table = get_l2_table(l3_table, addr->pm_l3_offset, us, rw);
  PM_L1_TABLE* l1_table = get_l1_table(l2_table, addr->pm_l2_offset, us, rw);
  return add_to_l1_table(l1_table, addr->physical_offset, us, rw);
}

/* Given a level 4 table, returns the level 3 table indicated by idx, if it is not present it creates one */
PM_L3_TABLE* get_l3_table(PM_L4_TABLE* table, uint64_t idx, int us, int rw) {
  if (table->table[idx].p == 0) {
    table->table[idx].p = 1;
    table->table[idx].rw = rw;
    table->table[idx].us = us;
    table->table[idx].address = (uint64_t)((uint64_t)pmm_page_alloc()/0x1000);
  }
   return (PM_L3_TABLE*)((uint64_t)(table->table[idx].address*0x1000));
}

/* Given a level 3 table, returns the level 2 table indicated by idx, if it is not present it creates one */
PM_L2_TABLE* get_l2_table(PM_L3_TABLE* table, uint64_t idx, int us, int rw) {
  if (table->table[idx].p == 0) {
    table->table[idx].p = 1;
    table->table[idx].rw = rw;
    table->table[idx].us = us;
    table->table[idx].address = (uint64_t)((uint64_t)pmm_page_alloc()/0x1000);
  }
   return (PM_L2_TABLE*)((uint64_t)(table->table[idx].address*0x1000));
}

/* Given a level 2 table, returns the level 1 table indicated by idx, if it is not present it creates one */
PM_L1_TABLE* get_l1_table(PM_L2_TABLE* table, uint64_t idx, int us, int rw) {
  if (table->table[idx].p == 0) {
    table->table[idx].p = 1;
    table->table[idx].rw = rw;
    table->table[idx].us = us;
    table->table[idx].address = (uint64_t)((uint64_t)pmm_page_alloc()/0x1000);
  }
   return (PM_L1_TABLE*)((uint64_t)(table->table[idx].address*0x1000));
}

/* Given a level 1 table, returns the physical page indicated by idx, if it is not present it creates one */
uint64_t add_to_l1_table(PM_L1_TABLE* table, uint64_t idx, int us, int rw) {
  if (table->table[idx].p == 0) {
    table->table[idx].p = 1;
    table->table[idx].rw = rw;
    table->table[idx].us = us;
    table->table[idx].address = (uint64_t)((uint64_t)pmm_page_alloc()/0x1000);
  }
  return (uint64_t)(table->table[idx].address*0x1000);
}

/* Given a level 4 directory, frees every page used AND THE LEVEL 4 DIRECTORY TOO */
uint64_t free_l4_table(PM_L4_TABLE* l4_table) {
  uint64_t pages_freed = 0;
  int i;
  for (i = 0; i < 512; i++) {
    PM_L4 current_l4 = l4_table->table[i];
    if (current_l4.p == 1) {
      pages_freed += free_l3_table((PM_L3_TABLE*)(current_l4.address*0x1000));
      current_l4.p = 0;
      pmm_page_free((void*)(current_l4.address*0x1000));
      pages_freed++;
    }
  }
  pmm_page_free((void*)l4_table);
  return pages_freed;
}

/* Given a level 3 directory, frees every page used */
uint64_t free_l3_table(PM_L3_TABLE* l3_table) {
  uint64_t pages_freed = 0;
  int i;
  for (i = 0; i < 512; i++) {
    PM_L3 current_l3 = l3_table->table[i];
    if (current_l3.p == 1) {
      pages_freed += free_l2_table((PM_L2_TABLE*)(current_l3.address*0x1000));
      current_l3.p = 0;
      pmm_page_free((void*)(current_l3.address*0x1000));
      pages_freed++;
    }
  }
  return pages_freed;
}

/* Given a level 2 directory, frees every page used */
uint64_t free_l2_table(PM_L2_TABLE* l2_table) {
  uint64_t pages_freed = 0;
  int i;
  for (i = 0; i < 512; i++) {
    PM_L2 current_l2 = l2_table->table[i];
    if (current_l2.p == 1) {
      pages_freed += free_l1_table((PM_L1_TABLE*)(current_l2.address*0x1000));
      current_l2.p = 0;
      pmm_page_free((void*)(current_l2.address*0x1000));
      pages_freed++;
    }
  }
  return pages_freed;
}

/* Given a level 1 directory, frees every page used */
uint64_t free_l1_table(PM_L1_TABLE* l1_table) {
  uint64_t pages_freed = 0;
  int i;
  for (i = 0; i < 511; i++) {
    PM_L1 current_l1 = l1_table->table[i];
    if (current_l1.p == 1) {
      current_l1.p = 0;
      if ((current_l1.address*0x1000) > (12*0x100000)) {
        /* WE DON'T WANT TO FREE PAGES THAT BELONGS TO THE KERNEL */
        pmm_page_free((void*)(current_l1.address*0x1000));
        pages_freed++;
      }
    }
  }
  return pages_freed;
}

/* Simple function that writes a hex number in screen preceeded by a string passed as an argument */
void hex_log(char* pre, uint64_t num) {
  video_write_string(KERNEL_CONSOLE, pre);
  video_write_string(KERNEL_CONSOLE, "0x");
  video_write_hex(KERNEL_CONSOLE, num);
  video_write_nl(KERNEL_CONSOLE);
}

/* This function tests the creation of a new level 4 directory */
void l4_table_test() {
  PM_L4_TABLE* l4_table = new_pml4(0);
  PM_L3_TABLE* l3_table = (PM_L3_TABLE*)((l4_table->table[0].address)*0x1000);
  PM_L2_TABLE* l2_table = (PM_L2_TABLE*)((l3_table->table[0].address)*0x1000);
  PM_L1_TABLE* _0_to_2_mb = (PM_L1_TABLE*)((l2_table->table[0].address)*0x1000);
  PM_L1_TABLE* _2_to_4_mb = (PM_L1_TABLE*)((l2_table->table[1].address)*0x1000);
  PM_L1_TABLE* _4_to_6_mb = (PM_L1_TABLE*)((l2_table->table[2].address)*0x1000);
  PM_L1_TABLE* _6_to_8_mb = (PM_L1_TABLE*)((l2_table->table[3].address)*0x1000);
  PM_L1_TABLE* _8_to_10_mb = (PM_L1_TABLE*)((l2_table->table[4].address)*0x1000);
  PM_L1_TABLE* _10_to_12_mb = (PM_L1_TABLE*)((l2_table->table[5].address)*0x1000);
  uint64_t* aux1 = (uint64_t*)_0_to_2_mb;
  hex_log(" ", aux1[0]);
  hex_log(" ", aux1[511]);
  uint64_t* aux2 = (uint64_t*)_2_to_4_mb;
  hex_log(" ", aux2[0]);
  hex_log(" ", aux2[511]);
  uint64_t* aux3 = (uint64_t*)_4_to_6_mb;
  hex_log(" ", aux3[0]);
  hex_log(" ", aux3[511]);
  uint64_t* aux4 = (uint64_t*)_6_to_8_mb;
  hex_log(" ", aux4[0]);
  hex_log(" ", aux4[511]);
  uint64_t* aux5 = (uint64_t*)_8_to_10_mb;
  hex_log(" ", aux5[0]);
  hex_log(" ", aux5[511]);
  uint64_t* aux6 = (uint64_t*)_10_to_12_mb;
  hex_log(" ", aux6[0]);
  hex_log(" ", aux6[511]);
}

/* Everytime a process is created asign the result of this function to rsp */
uint64_t alloc_new_process_stack() {
  PM_L4_TABLE* l4_table = (PM_L4_TABLE*)readCR3();
  PM_L3_TABLE* l3_table = get_l3_table(l4_table, 0, 1, 1);
  PM_L2_TABLE* l2_table = get_l2_table(l3_table, 0, 1, 1);
  /* Process stack is mapped from 22 MB to 30 MB and heap from 30 MB to 32 MB */
  uint64_t aux = pmm_page_alloc();
  /* STACK */
  l2_table->table[14].p = 1;
  l2_table->table[14].rw = 1;
  l2_table->table[14].us = 1;
  l2_table->table[14].address = aux/0x1000;
  /* HEAP */
  l2_table->table[15].p = 1;
  l2_table->table[15].rw = 1;
  l2_table->table[15].us = 1;
  l2_table->table[15].address = aux/0x1000;

  PM_L1_TABLE* l1_table = (PM_L1_TABLE*)aux;
  uint64_t first_stack_page = pmm_page_alloc();
  l1_table->table[511].p = 1;
  l1_table->table[511].rw = 1;
  l1_table->table[511].us = 1;
  l1_table->table[511].address = first_stack_page/0x1000;

  return first_stack_page + 0x1000 - 1;
}
