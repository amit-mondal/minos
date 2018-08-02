
#ifndef CPU_TIMER_H
#define CPU_TIMER_H

#include <stdint.h>

#define HW_CLK_FRQ_HZ 1193180
#define CMD_PORT 0x43
#define CMD_BYTE 0x36

#define DATA_PORT_CHAN0 0x40 // Output connected to IRQ0
#define DATA_PORT_CHAN1 0x41 // No longer used
#define DATA_PORT_CHAN2 0x42 // controls the PC speaker

void init_timer(uint32_t freq);

#endif
