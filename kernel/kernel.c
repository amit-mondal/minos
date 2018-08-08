
#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/keyboard.h"
#include "../drivers/screen.h"
#include "../libc/string.h"
#include "../libc/stddef.h"

char buf[256];

void kmain() {
    
    isr_install();
    irq_install();

    __asm__ __volatile__("sti");

    while (1) {
	size_t len = read_raw_kb(buf, 255);
	buf[len] = 0;
	if (strcmp(buf, "HALT") == 0) {
	    kprint("Halting the CPU.\n");
	    __asm__ __volatile__("hlt");
	} else {
	    kprint("Unknown command");
	    kprint(buf);
	    kprint("\n");
	}
    }
}
