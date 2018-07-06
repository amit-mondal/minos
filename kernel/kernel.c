
#include "../drivers/ports.h"

void dummy_test_entrypoint() {
}

void main() {

  port_byte_out(0x3d4, 14); // Requesting byte 14: high byte of cursor pos

  int position = port_byte_in(0x3d5);
  position <<= 8; // Get high byte

  port_byte_out(0x3d4, 15); // Low byte;
  position += port_byte_in(0x3d5);

  int offset_from_vga = position * 2;

  char* vga = 0xb8000;
  vga[offset_from_vga] = 'X';
  vga[offset_from_vga + 1] = 0x0f; // white on black;
  
}
