

#ifndef _PAGING_H_
#define _PAGING_H_

#include <stdint.h>

/*
  For physical management
*/

#define PAGE_SIZE 0x1000
#define PAGE_DIR_SIZE PAGE_SIZE
#define STACK_PADDR 0xFF000000

enum pmem_status {
		  SUCCESS = 0,
		  ENOTINSTACK = 1,
};
		

/*
  For virtual management
*/

#define PAGE_DIR_VADDR 0xFFBFF000
#define PAGE_TABLE_VADDR 0xFFC00000
#define PAGE_DIR_INDEX(x) ((uint32_t) (x) % 1024)

enum vmem_status {
		  ENOMAPPING = 1
};

/*
  Flags for page table entries.
*/

#define CR0_PAGING_ENABLE_BIT 0x80000000

#define FL_PAGE_PRESENT 0x1
#define FL_PAGE_RW 0x2
#define FL_PAGE_USER 0x4

#define PAGE_MASK 0xFFFFF000
#define PAGE_TABLE_NUM_ENTRIES 1024

typedef uintptr_t page_dir_t;


void paging_init(uintptr_t phys_start);

/*
  Physical page management functions
*/

uintptr_t alloc_phys_page();
int free_phys_page(uintptr_t p);

/*
  Virtual page management functions
*/

void enable_paging();
void switch_page_dir(page_dir_t* page_dir);
void vpage_map(uintptr_t virt_addr, uintptr_t phys_addr, uint32_t flags);
void vpage_unmap (uintptr_t virt_addr);

int vpage_get_mapping(uintptr_t virt_addr, uintptr_t* phys_addr);



#endif
