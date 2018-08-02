
#ifndef MEM_H
#define MEM_H

#include <stdint.h>

void* memcpy(void* dest, void* src, unsigned long nbytes);
void* memset(void* dest, uint8_t val, unsigned long len);

#endif
