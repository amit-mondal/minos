
#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/keyboard.h"
#include "../drivers/screen.h"
#include "../libc/stddef.h"

char buf[256];

void kmain() {
    isr_install();
    irq_install();  

    __asm__ __volatile__("sti");

    size_t len = read_raw_kb(buf, 255);
    buf[len] = 0;
    kprint("Received a string: ");
    kprint(buf);
}
