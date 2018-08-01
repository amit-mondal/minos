

#ifndef KERNEL_UTIL_H
#define KERNEL_UTIL_H

#include "../cpu/types.h"

void kmemory_copy(char* src, char* dest, int nbytes);
void kmemory_set(u8* dest, u8 val, u32 len);
void int_to_ascii(int n, char str[]);
void reverse(char s[]);
int string_len(char s[]);

#ifdef __GNUC__
#define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#else
#define UNUSED(x) UNUSED_ ## x
#endif

#endif
