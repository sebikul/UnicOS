#include "paging.h"
#include "video.h"
#include "mem.h"
#include "task.h"
#include "kernel.h"

#define NEGATE(x) (x == 0 ? 1 : 0)
#define PAGE_SIZE 0x1000
#define MEGA_BYTE 0x100000

static uint64_t originalCR3;
static PM_L1_TABLE* k_0_2_MB;
static PM_L1_TABLE* k_2_4_MB;
static PM_L1_TABLE* k_4_6_MB;
static PM_L1_TABLE* k_6_8_MB;
static PM_L1_TABLE* k_8_10_MB;
static PM_L1_TABLE* k_10_12_MB;

void wait(int i){
  while(i--);
}

void vmm_initialize() {
  // Bit 31 of CR0, PGE (Bit 7), PAE (Bit 5), and PSE (Bit 4) of CR4 are set in pure64.asm
  create_initial_l1_tables();

  video_write_string(KERNEL_CONSOLE, "OLD CR3: 0x");
  video_write_hex(KERNEL_CONSOLE, readCR3());
  video_write_nl(KERNEL_CONSOLE);

  // Save this so we can be sure that loading this will give us identity paging in the first GB
  originalCR3 = (uint64_t)all_memory_identity();
  //originalCR3 = readCR3();
  writeCR3(originalCR3);
  //writeCR4(0x600);

  video_write_string(KERNEL_CONSOLE, "NEW CR3: 0x");
  video_write_hex(KERNEL_CONSOLE, readCR3());
  video_write_nl(KERNEL_CONSOLE);
}

void create_initial_l1_tables() {
  k_0_2_MB = identity_l1_map(0, 1, 0);
  k_2_4_MB = identity_l1_map(1, 1, 0);
  k_4_6_MB = identity_l1_map(2, 1, 1); // This is userland
  k_6_8_MB = identity_l1_map(3, 1, 1);
  k_8_10_MB = identity_l1_map(4, 1, 1);
  k_10_12_MB = identity_l1_map(5, 1, 0);
}

/* Creates the directories that will not change */
PM_L4_TABLE* all_memory_identity() {
  PM_L4_TABLE* k_pml4 = (PM_L4_TABLE*)pmm_page_alloc();
  PM_L3_TABLE* k_pml3 = (PM_L3_TABLE*)pmm_page_alloc();
  PM_L2_TABLE* k_pml2 = (PM_L2_TABLE*)pmm_page_alloc();
  /* Make L4 point to L3 */
  k_pml4->table[0].p = 1;
  k_pml4->table[0].rw = 1;
  k_pml4->table[0].us = 0;
  k_pml4->table[0].address = (uint64_t)((uint64_t)k_pml3/PAGE_SIZE);
  /* Make L3 point to L2 */
  k_pml3->table[0].p = 1;
  k_pml3->table[0].rw = 1;
  k_pml3->table[0].us = 0;
  k_pml3->table[0].address = (uint64_t)((uint64_t)k_pml2/PAGE_SIZE);
  video_write_string(KERNEL_CONSOLE, "CREATING KERNEL PAGE LEVEL 4 DIRECTORY");
  for (int i = 0; i < 257; i++) {
    //video_write_string(KERNEL_CONSOLE, ".");
    uint64_t l1_current = (uint64_t)pmm_page_alloc();
    k_pml2->table[i].p = 1;
    k_pml2->table[i].us = 0;
    k_pml2->table[i].rw = 0;
    k_pml2->table[i].address = (l1_current/PAGE_SIZE);
    for (int j = 0; j < 512; j++) {
      PM_L1_TABLE* l1 = (PM_L1_TABLE*)l1_current;
      l1->table[j].p = 1;
      l1->table[j].us = 0;
      l1->table[j].rw = 0;
      l1->table[j].address = (uint64_t)(((i*2*MEGA_BYTE)+(j*PAGE_SIZE))/PAGE_SIZE);
      //kdebug("ALLOCATED ID PAGE AT: 0x");
      //kdebug_base((uint64_t)(i*2*MEGA_BYTE)+(j*PAGE_SIZE), 16);
      //kdebug_nl();
    }
  }
  video_write_nl(KERNEL_CONSOLE);
  return k_pml4;
}

/* Creates the directories that will not change */
PM_L4_TABLE* create_pml4() {
  PM_L4_TABLE* k_pml4 = (PM_L4_TABLE*)pmm_page_alloc();
  PM_L3_TABLE* k_pml3 = (PM_L3_TABLE*)pmm_page_alloc();
  PM_L2_TABLE* k_pml2 = (PM_L2_TABLE*)pmm_page_alloc();
  /* Make L4 point to L3 */
  k_pml4->table[0].p = 1;
  k_pml4->table[0].rw = 1;
  k_pml4->table[0].us = 0;
  k_pml4->table[0].address = (uint64_t)((uint64_t)k_pml3/PAGE_SIZE);
  /* Make L3 point to L2 */
  k_pml3->table[0].p = 1;
  k_pml3->table[0].rw = 1;
  k_pml3->table[0].us = 0;
  k_pml3->table[0].address = (uint64_t)((uint64_t)k_pml2/PAGE_SIZE);
  generic_l2_table(k_pml2);
  return k_pml4;
}

void generic_l2_table(PM_L2_TABLE* table) {
  for (int i = 0; i < 6; i++) {
    table->table[i].p = 1;
    table->table[i].rw = 1;
    table->table[i].us = 0;
  }
  table->table[0].address = (uint64_t)k_0_2_MB/PAGE_SIZE;
  table->table[1].address = (uint64_t)k_2_4_MB/PAGE_SIZE;
  table->table[2].us = 1;
  table->table[2].address = (uint64_t)k_4_6_MB/PAGE_SIZE;
  table->table[3].address = (uint64_t)k_6_8_MB/PAGE_SIZE;
  table->table[4].address = (uint64_t)k_8_10_MB/PAGE_SIZE;
  table->table[5].address = (uint64_t)k_10_12_MB/PAGE_SIZE;
}

/* Given a level 2 table, identity page the 2mb that corresponds to that table */
PM_L1_TABLE* identity_l1_map(int first_l2_table_idx, int rw, int us) {
  PM_L1_TABLE* l1_table = (PM_L1_TABLE*)pmm_page_alloc();
  int i;
  for (i = 0; i < 512; i++) {
    l1_table->table[i].p = 1;
    l1_table->table[i].rw = rw;
    l1_table->table[i].us = us;
    l1_table->table[i].address = ((first_l2_table_idx*0x200000) + (i*PAGE_SIZE))/PAGE_SIZE;
  }
  return l1_table;
}

/* Returns a new level 4 directory for a user */
PM_L4_TABLE* new_process_cr3() {
  bool ints = kset_ints(FALSE);
  uint64_t saved_cr3 = readCR3();
  writeCR3(originalCR3);

  PM_L4_TABLE* l4_table = create_pml4();

  writeCR3(saved_cr3);
  kset_ints(ints);
  return l4_table;
}

/* Everytime a process is created asign the result of this function to rsp */
uint64_t alloc_new_process_stack(PM_L4_TABLE* l4_table, task_entry_point func, const char* name, int argc, char** argv, uint64_t wrapper) {
  bool ints = kset_ints(FALSE);
  uint64_t saved_cr3 = readCR3();
  writeCR3(originalCR3);

  // PM_L3_TABLE* l3_table = get_l3_table(l4_table, 0, 0, 1);
  // PM_L2_TABLE* l2_table = get_l2_table(l3_table, 0, 0, 1);
  // /* Process stack is mapped from 22 MB to 30 MB and heap from 30 MB to 32 MB */
  // uint64_t from_28_to_30 = pmm_page_alloc();
  // uint64_t from_30_to_32 = pmm_page_alloc();
  // /* STACK */
  // l2_table->table[14].p = 1;
  // l2_table->table[14].rw = 1;
  // l2_table->table[14].us = 1;
  // l2_table->table[14].address = from_28_to_30/PAGE_SIZE;
  // /* HEAP */
  // l2_table->table[15].p = 1;
  // l2_table->table[15].rw = 1;
  // l2_table->table[15].us = 1;
  // l2_table->table[15].address = from_30_to_32/PAGE_SIZE;

  // PM_L1_TABLE* l1_table = (PM_L1_TABLE*)from_30_to_32;
  // uint64_t first_heap_page = pmm_page_alloc();
  // l1_table->table[0].p = 1;
  // l1_table->table[0].rw = 1;
  // l1_table->table[0].us = 1;
  // l1_table->table[0].address = first_heap_page/PAGE_SIZE;

  // l1_table = (PM_L1_TABLE*)from_28_to_30;
  // uint64_t first_stack_page = pmm_page_alloc();
  // l1_table->table[511].p = 1;
  // l1_table->table[511].rw = 1;
  // l1_table->table[511].us = 1;
  // l1_table->table[511].address = first_stack_page/PAGE_SIZE;

  // VirtualAddress* heap_ptr = (VirtualAddress*)malloc(sizeof(VirtualAddress));
  // heap_ptr->pm_l4_offset = 0;
  // heap_ptr->pm_l3_offset = 0;
  // heap_ptr->pm_l2_offset = 15;
  // heap_ptr->pm_l1_offset = 0;
  // add_page((uint64_t)l4_table, heap_ptr, 1, 1);

  // VirtualAddress* stack_ptr = (VirtualAddress*)malloc(sizeof(VirtualAddress));
  // stack_ptr->pm_l4_offset = 0;
  // stack_ptr->pm_l3_offset = 0;
  // stack_ptr->pm_l2_offset = 14;
  // stack_ptr->pm_l1_offset = 511;
  // uint64_t first_stack_page;
  // first_stack_page = add_page((uint64_t)l4_table, stack_ptr, 1, 1);

  uint64_t first_stack_page;
  VirtualAddress* aux = (VirtualAddress*)malloc(sizeof(VirtualAddress));
  int i, j;
  for (i = 11; i <= 15 ; i++) {
    for (j = 0; j < 512; j++) {
      aux->pm_l4_offset = 0;
      aux->pm_l3_offset = 0;
      aux->pm_l2_offset = i;
      aux->pm_l1_offset = j;
      if (i==14 && j ==511){
        first_stack_page = add_page((uint64_t)l4_table, aux, 1, 1);
      } else {
        add_page((uint64_t)l4_table, aux, 1, 1);
      }
    }
  }

  /* Write the registers in the stack */
  uint64_t addr = first_stack_page + PAGE_SIZE - sizeof(context_t);
  context_t* context = (context_t*)addr;

  context->gs = 0x001;
  context->fs = 0x002;
  context->r15 =  0x003;
  context->r14 =  0x004;
  context->r13 =  0x005;
  context->r12 =  0x006;
  context->r11 =  0x007;
  context->r10 =  0x008;
  context->r9 = 0x009;
  context->r8 = 0x00A;
  context->rsi =  (uint64_t)argc;
  context->rdi =  (uint64_t)func;
  context->rbp =  0x00D;
  context->rdx =  (uint64_t)argv;
  context->rcx =  0x00F;
  context->rbx =  0x010;
  context->rax =  0x011;
  context->rip =  (uint64_t)wrapper;
  context->cs = 0x008;
  context->rflags = 0x202;
  context->rsp = (30*MEGA_BYTE);
  context->ss = 0x000;
  context->base = 0x000;

  //l1_table = (PM_L1_TABLE*)first_stack_page;
  //print_l1(l1_table, 0);

  writeCR3(saved_cr3);
  kset_ints(ints);
  //return addr;
  return (30*MEGA_BYTE) - sizeof(context_t);
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
  uint64_t faulty_cr3;
  bool ints = kset_ints(FALSE);

  // CR2 contains the virtual address that caused the fault
  video_write_string(KERNEL_CONSOLE, "-------------------------------------------------------------------------------");
  video_write_nl(KERNEL_CONSOLE);
  video_write_string(KERNEL_CONSOLE, "PAGE FAULT ");
  video_write_string(KERNEL_CONSOLE, "CR2: 0x");
  video_write_hex(KERNEL_CONSOLE, cr2);
  video_write_nl(KERNEL_CONSOLE);
  //PAGE_FAULT_ERROR_CODE* error = (PAGE_FAULT_ERROR_CODE*)error_code;
  video_write_string(KERNEL_CONSOLE, "ERROR: ");
  video_write_hex(KERNEL_CONSOLE, error_code);
  video_write_nl(KERNEL_CONSOLE);
  kdebug("PAGE FAULT CR2: 0x");
  kdebug_base(cr2, 16);
  kdebug_nl();

  VirtualAddress* faulty_address = translate(cr2);

  if (cr2 < (22*MEGA_BYTE)) {
    // KEEL DAT PROCESS
    video_write_string(KERNEL_CONSOLE, "----------- PROCESO PIDIO MEMORIA FUERA DE LO PERMITIDO PARA STACK ------------");
    signal_send(task_get_current(), SIGKILL);
  }
  else if (cr2 >= (22*MEGA_BYTE) && cr2 < (30*MEGA_BYTE)) {
    video_write_string(KERNEL_CONSOLE, "---------------------- PROCESO REQUIERE MEMORIA EN STACK ----------------------");
    video_write_nl(KERNEL_CONSOLE);
    faulty_cr3 = readCR3();
    writeCR3(originalCR3);
    add_page(faulty_cr3, faulty_address, 1, 1);
    writeCR3(faulty_cr3);
  }
  else if (cr2 >= (30*MEGA_BYTE) && cr2 < (32*MEGA_BYTE)) {
    video_write_string(KERNEL_CONSOLE, "---------------------- PROCESO REQUIERE MEMORIA EN HEAP -----------------------");
    video_write_nl(KERNEL_CONSOLE);
    faulty_cr3 = readCR3();
    writeCR3(originalCR3);
    add_page(faulty_cr3, faulty_address, 1, 1);
    writeCR3(faulty_cr3);
  }
  else if (cr2 >= (32*MEGA_BYTE)) {
    // KEEL DAT PROCESS
    video_write_string(KERNEL_CONSOLE, "----------- PROCESO PIDIO MEMORIA FUERA DE LO PERMITIDO PARA HEAP -------------");
    signal_send(task_get_current(), SIGKILL);
  }

  kset_ints(ints);
  return;
}

/* Adds all the unpresent pages of a virtual address */
uint64_t add_page(uint64_t cr3, VirtualAddress* addr, int us, int rw) {
  PM_L4_TABLE* l4_table = (PM_L4_TABLE*)cr3;
  PM_L3_TABLE* l3_table = get_l3_table(l4_table, addr->pm_l4_offset, us, rw);
  PM_L2_TABLE* l2_table = get_l2_table(l3_table, addr->pm_l3_offset, us, rw);
  PM_L1_TABLE* l1_table = get_l1_table(l2_table, addr->pm_l2_offset, us, rw);
  uint64_t page = add_to_l1_table(l1_table, addr->pm_l1_offset, us, rw);
  return page;
}

/* Given a level 4 table, returns the level 3 table indicated by idx, if it is not present it creates one */
PM_L3_TABLE* get_l3_table(PM_L4_TABLE* table, uint64_t idx, int us, int rw) {
  if (table->table[idx].p == 0) {
    table->table[idx].p = 1;
    table->table[idx].rw = rw;
    table->table[idx].us = us;
    table->table[idx].address = (uint64_t)((uint64_t)pmm_page_alloc()/PAGE_SIZE);
    //video_write_string(KERNEL_CONSOLE, "*** PAGINA ALOCADA EN L4");
    //hex_log(" PHYSICAL: ", (uint64_t)(table->table[idx].address*PAGE_SIZE));
  }
  return (PM_L3_TABLE*)((uint64_t)(table->table[idx].address*PAGE_SIZE));
}

/* Given a level 3 table, returns the level 2 table indicated by idx, if it is not present it creates one */
PM_L2_TABLE* get_l2_table(PM_L3_TABLE* table, uint64_t idx, int us, int rw) {
  if (table->table[idx].p == 0) {
    table->table[idx].p = 1;
    table->table[idx].rw = rw;
    table->table[idx].us = us;
    table->table[idx].address = (uint64_t)((uint64_t)pmm_page_alloc()/PAGE_SIZE);
    //video_write_string(KERNEL_CONSOLE, "*** PAGINA ALOCADA EN L3");
    //hex_log(" PHYSICAL: ", (uint64_t)(table->table[idx].address*PAGE_SIZE));
  }
  return (PM_L2_TABLE*)((uint64_t)(table->table[idx].address*PAGE_SIZE));
}

/* Given a level 2 table, returns the level 1 table indicated by idx, if it is not present it creates one */
PM_L1_TABLE* get_l1_table(PM_L2_TABLE* table, uint64_t idx, int us, int rw) {
  //video_write_string(KERNEL_CONSOLE, "INDICE EN L2:");
  //video_write_dec(KERNEL_CONSOLE, idx);
  //video_write_nl(KERNEL_CONSOLE);
  if (table->table[idx].p == 0) {
    table->table[idx].p = 1;
    table->table[idx].rw = rw;
    table->table[idx].us = us;
    table->table[idx].address = (uint64_t)((uint64_t)pmm_page_alloc()/PAGE_SIZE);
    //video_write_string(KERNEL_CONSOLE, "*** PAGINA ALOCADA EN L2");
    //hex_log(" PHYSICAL: ", (uint64_t)(table->table[idx].address*PAGE_SIZE));
  }
  return (PM_L1_TABLE*)((uint64_t)(table->table[idx].address*PAGE_SIZE));
}

/* Given a level 1 table, returns the physical page indicated by idx, if it is not present it creates one */
uint64_t add_to_l1_table(PM_L1_TABLE* table, uint64_t idx, int us, int rw) {
  //video_write_string(KERNEL_CONSOLE, "INDICE EN L1:");
  //video_write_dec(KERNEL_CONSOLE, idx);
  //video_write_nl(KERNEL_CONSOLE);
  if (table->table[idx].p == 0) {
    table->table[idx].p = 1;
    table->table[idx].rw = rw;
    table->table[idx].us = us;
    table->table[idx].address = (uint64_t)((uint64_t)pmm_page_alloc()/PAGE_SIZE);
    //video_write_string(KERNEL_CONSOLE, "*** PAGINA ALOCADA EN L1");
    //hex_log(" PHYSICAL: ", (uint64_t)(table->table[idx].address*PAGE_SIZE));
  }
  return (uint64_t)(table->table[idx].address*PAGE_SIZE);
}

/* Given a level 4 directory, frees every page used AND THE LEVEL 4 DIRECTORY TOO */
uint64_t free_l4_table(PM_L4_TABLE* l4_table) {
  bool ints = kset_ints(FALSE);
  uint64_t saved_cr3 = readCR3();
  writeCR3(originalCR3);

  uint64_t pages_freed = 0;
  int i;
  for (i = 0; i < 512; i++) {
    PM_L4 current_l4 = l4_table->table[i];
    if (current_l4.p == 1) {
      pages_freed += free_l3_table((PM_L3_TABLE*)(current_l4.address*PAGE_SIZE));
      current_l4.p = 0;
      pmm_page_free((void*)(current_l4.address*PAGE_SIZE));
      pages_freed++;
    }
  }
  pmm_page_free((void*)l4_table);

  writeCR3(saved_cr3);
  kset_ints(ints);
  return pages_freed;
}

/* Given a level 3 directory, frees every page used */
uint64_t free_l3_table(PM_L3_TABLE* l3_table) {
  uint64_t pages_freed = 0;
  int i;
  for (i = 0; i < 512; i++) {
    PM_L3 current_l3 = l3_table->table[i];
    if (current_l3.p == 1) {
      pages_freed += free_l2_table((PM_L2_TABLE*)(current_l3.address*PAGE_SIZE));
      current_l3.p = 0;
      pmm_page_free((void*)(current_l3.address*PAGE_SIZE));
      pages_freed++;
    }
  }
  return pages_freed;
}

/* Given a level 2 directory, frees every page used */
uint64_t free_l2_table(PM_L2_TABLE* l2_table) {
  uint64_t pages_freed = 0;
  int i;
  // Start from 6 cause we don't want to free first 12 MB
  for (i = 6; i < 512; i++) {
    PM_L2 current_l2 = l2_table->table[i];
    if (current_l2.p == 1) {
      pages_freed += free_l1_table((PM_L1_TABLE*)(current_l2.address*PAGE_SIZE));
      current_l2.p = 0;
      pmm_page_free((void*)(current_l2.address*PAGE_SIZE));
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
      if ((current_l1.address*PAGE_SIZE) > (12*MEGA_BYTE)) {
        /* WE DON'T WANT TO FREE PAGES THAT BELONGS TO THE KERNEL */
        pmm_page_free((void*)(current_l1.address*PAGE_SIZE));
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
  PM_L4_TABLE* l4_table = create_pml4();
  PM_L3_TABLE* l3_table = (PM_L3_TABLE*)((l4_table->table[0].address)*PAGE_SIZE);
  PM_L2_TABLE* l2_table = (PM_L2_TABLE*)((l3_table->table[0].address)*PAGE_SIZE);
  PM_L1_TABLE* _0_to_2_mb = (PM_L1_TABLE*)((l2_table->table[0].address)*PAGE_SIZE);
  PM_L1_TABLE* _2_to_4_mb = (PM_L1_TABLE*)((l2_table->table[1].address)*PAGE_SIZE);
  PM_L1_TABLE* _4_to_6_mb = (PM_L1_TABLE*)((l2_table->table[2].address)*PAGE_SIZE);
  PM_L1_TABLE* _6_to_8_mb = (PM_L1_TABLE*)((l2_table->table[3].address)*PAGE_SIZE);
  PM_L1_TABLE* _8_to_10_mb = (PM_L1_TABLE*)((l2_table->table[4].address)*PAGE_SIZE);
  PM_L1_TABLE* _10_to_12_mb = (PM_L1_TABLE*)((l2_table->table[5].address)*PAGE_SIZE);
  print_l1(_0_to_2_mb, 1);
  print_l1(_2_to_4_mb, 1);
  print_l1(_4_to_6_mb, 1);
  print_l1(_6_to_8_mb, 1);
  print_l1(_8_to_10_mb, 1);
  print_l1(_10_to_12_mb, 1);
  hex_log("DIR: ", (uint64_t)pmm_page_alloc());
}

void print_l2(PM_L2_TABLE* table, int amount) {
  video_write_string(KERNEL_CONSOLE, "PRIMERAS ENTRADAS DE L2:");
  video_write_nl(KERNEL_CONSOLE);
  uint64_t* l2 = (uint64_t*) table;
  for (int i = 0; i < 16; i++) {
    hex_log("** ", l2[i]);
  }
}

void print_l1(PM_L1_TABLE* table, int amount) {
  uint64_t* aux = (uint64_t*)table;
  for (int i = 488; i < 512; i++) {
    video_write_string(KERNEL_CONSOLE, "L1[");
    video_write_dec(KERNEL_CONSOLE, i);
    hex_log("]: ", aux[i]);
  }
}

VirtualAddress* translate(uint64_t dir) {
  VirtualAddress* aux=malloc(sizeof(VirtualAddress));
  aux->physical_offset = dir & 0x0000000000000FFF;
  aux->pm_l1_offset    = ((dir & 0x00000000001FF000) >> 12) & 0x00000000000001FF;
  aux->pm_l2_offset    = ((dir & 0x000000003FE00000) >> 21) & 0x00000000000001FF;
  aux->pm_l3_offset    = ((dir & 0x0000007FC0000000) >> 30) & 0x00000000000001FF;
  aux->pm_l4_offset    = ((dir & 0x0000FF8000000000) >> 39) & 0x00000000000001FF;
  return aux;
}

uint64_t get_kernel_cr3() {
  return originalCR3;
}
