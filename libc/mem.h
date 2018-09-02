
#ifndef LIB_MEM_H
#define LIB_MEM_H

#include <stdint.h>
#include "stddef.h"

void* memcpy(void* dest, void* src, unsigned long nbytes);
void* memset(void* dest, uint8_t val, unsigned long len);

#endif
