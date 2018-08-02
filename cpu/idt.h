#ifndef CPU_IDT_H
#define CPU_IDT_H

#include <stdint.h>

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
  uint16_t low_offset;
  uint16_t sel;
  uint8_t null_byte;
  uint8_t flags;
  uint16_t high_offset;
} __attribute__((packed)) idt_gate_t;


// Interrupt table descriptor

typedef struct {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed)) idt_register_t;

#define IDT_ENTRIES 256

idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

void set_idt_gate(int n, uint32_t handler);
void set_idt();

#endif
