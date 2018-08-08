
#define PIC_SCANCODE_PORT 0x60

#define KB_BACKSPACE 0x0E
#define KB_ENTER 0x1C

#define KB_INPUT_BUFFER_SIZE 1024

#define SCANCODE_MAX 57

#include "../libc/stddef.h"

void init_keyboard();
size_t read_raw_kb(void* buf, size_t len);
