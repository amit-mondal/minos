#include "mem.h"


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
