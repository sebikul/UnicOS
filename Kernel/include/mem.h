#ifndef MEM_H
#define MEM_H

void* malloc(int len);
void* calloc(int len);
void free(void* m);
void pmm_initialize();
void* pmm_page_alloc();
void pmm_page_free(void* dir);
void dump_last_n_pages(int n);

#endif
