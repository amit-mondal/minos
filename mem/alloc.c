#include "alloc.h"


static void* heap_addr = INITIAL_HEAP_ADDR;

void* memcpy(void* dest, void* src, unsigned long nbytes) {
    unsigned long i;
    uint8_t *src_ptr = src, *dst_ptr = dest;
    for (i = 0;i < nbytes;i++) {
	*(dst_ptr + i) = *(src_ptr + i);
    }
    return dest;
}

void* memset(void* dest, uint8_t val, unsigned long len) {
    uint8_t *temp = (uint8_t *)dest;
    while (len) {
	*temp++ = val;
	len--;
    }
    return dest;
}

void* kmalloc(size_t size, int align, void** phys) {
    if (align && ADDR_NOT_PAGE_ALIGNED(heap_addr)) {
	heap_addr = (void*) (((uintptr_t) heap_addr) & PAGE_MASK); // Clear low bits so address is at page boundary.
	heap_addr += PAGE_SIZE; // Increase to next page boundary.
    }

    if (phys) {
	*phys = heap_addr;
    }
    
    void* tmp = heap_addr;
    // Move up address
    heap_addr += size;
    return tmp;
}
