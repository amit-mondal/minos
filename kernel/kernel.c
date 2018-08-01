
#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/keyboard.h"

void main() {
  isr_install();

  __asm__ __volatile__("sti");
  init_timer(50);
  init_keyboard();
  
}
