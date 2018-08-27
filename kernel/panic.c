

#include "panic.h"
#include "../drivers/screen.h"

void panic (const char* msg) {
    kprint(" SYSTEM PANIC: ");
    kprint(msg);
    kprint("\n");
    while (1);
}
