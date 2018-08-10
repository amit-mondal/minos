
#include "timer.h"
#include "ports.h"
#include "../drivers/screen.h"
#include "../libc/string.h"
#include "../libc/stddef.h"
#include "isr.h"

uint32_t tick = 0;

static void timer_cb(registers_t* regs UNUSED) {
    tick++;    
}


void init_timer(uint32_t freq) {

    register_interrupt_handler(IRQ0, timer_cb);

    // Get PIT value
    uint32_t divisor = HW_CLK_FRQ_HZ / freq;
    
    uint8_t low = LOW_8(divisor);
    uint8_t high = HIGH_8(divisor);

    // Send the command
    port_byte_out(CMD_PORT, CMD_BYTE);
    // Set the frequency divisor
    port_byte_out(DATA_PORT_CHAN0, low);
    port_byte_out(DATA_PORT_CHAN0, high);
    
}
