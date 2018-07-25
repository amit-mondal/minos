#ifndef CPU_IDT_H
#define CPU_IDT_H

#include "types.h"

#define KERNEL_CS 0x08 // CS register value

// Interrupt gate (handler) definition
/*
   Flags:
   Bit 7: Interrupt is present
   Bits 6-5: Privelege level of kernel (0-3)
   Bit 4: Set to 0 for interrupt gates
   Bits 3-0: 1110 = 14 -> 32 bit interrupt gate
*/

typedef struct {
  u16 low_offset;
  u16 sel;
  u8 null_byte;
  u8 flags;
  u16 high_offset;
} __attribute__((packed)) idt_gate_t;


// Interrupt table descriptor

typedef struct {
  u16 limit;
  u32 base;
} __attribute__((packed)) idt_register_t;

#define IDT_ENTRIES 256

idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

void set_idt_gate(int n, u32 handler);
void set_idt();

#endif
