
#ifndef LIB_MEM_H
#define LIB_MEM_H

#include <stdint.h>
#include "stddef.h"

void* memcpy(void* dest, void* src, unsigned long nbytes);
void* memset(void* dest, uint8_t val, unsigned long len);

void* kmalloc(size_t size, int align, void** phys);

#define PAGE_SIZE 0x1000
#define PAGE_MASK 0xFFFFF000

#define INITIAL_HEAP_ADDR ((void*) 0x10000)

#define ADDR_NOT_PAGE_ALIGNED(addr) ((uintptr_t) addr & PAGE_MASK)

#endif
