
#include "idt.h"
#include "../libc/stddef.h"

void set_idt_gate(int n, uint32_t handler) {
  idt[n] = (idt_gate_t) {
			 .low_offset = LOW_16(handler),
			 .sel = KERNEL_CS,
			 .null_byte = NULL,
			 .flags = 0x8E,
			 .high_offset = HIGH_16(handler)
  };
}

void set_idt() {
  idt_reg = (idt_register_t) {
	     .base = (uint32_t) &idt,
	     .limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1
  };
  // Now load the idt descriptor
  __asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg));
}
