
#include "timer.h"
#include "../drivers/screen.h"
#include "../drivers/ports.h"
#include "../kernel/util.h"
#include "isr.h"

u32 tick = 0;

static void timer_cb(registers_t UNUSED(regs)) {
    tick++;
    kprint("Tick: ");
    char tick_ascii[256];

    int_to_ascii(tick, tick_ascii);
    kprint(tick_ascii);
    kprint("\n");
    
}


void init_timer(u32 freq) {

    register_interrupt_handler(IRQ0, timer_cb);

    // Get PIT value
    u32 divisor = 1193180 / freq;
    
    u8 low = LOW_8(divisor);
    u8 high = HIGH_8(divisor);

    // Send the command
    port_byte_out(CMD_PORT, CMD_BYTE);
    // Set the frequency divisor
    port_byte_out(DATA_PORT_CHAN0, low);
    port_byte_out(DATA_PORT_CHAN0, high);
    
}
