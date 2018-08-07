
#define PIC_SCANCODE_PORT 0x60

#define BACKSPACE_KB 0x0E
#define ENTER_KB 0x1C

#define KB_INPUT_BUFFER_SIZE 1024

#define SCANCODE_MAX 57

#include "../libc/stddef.h"

void init_keyboard();
size_t read_raw_kb(void* buf, size_t len);
