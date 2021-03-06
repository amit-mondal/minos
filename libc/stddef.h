
#ifndef STDEF_H
#define STDEF_H

#define NULL 0x0

#if (__GNUC__ > 2) || (__GNUC__ == 2 && __GNUC_MINOR__ > 4)
#define UNUSED         __attribute__((__unused__))
#else
#define UNUSED
#endif

#define LOW_16(address) (uint16_t)(address & 0xFFFF)
#define HIGH_16(address) (uint16_t)((address >> 16) & 0xFFFF)

#define LOW_8(address) (uint8_t)(address & 0xFF)
#define HIGH_8(address) (uint8_t)((address >> 8)) & 0xFF

typedef unsigned long size_t;

#endif
