

#include "paging.h"
#include "../cpu/isr.h"
#include "../kernel/panic.h"
#include "../drivers/screen.h"
#include "../libc/string.h"

/*
  State for managing physical pages.
*/

uintptr_t stack_phys_loc = STACK_PADDR;
uintptr_t stack_phys_max = STACK_PADDR;

uintptr_t phys_ptr;

/*
  State for managing virtual pages (and mappings).
*/

uintptr_t* page_dir = (uintptr_t *) PAGE_DIR_VADDR;
uintptr_t* page_tables = (uintptr_t*) PAGE_TABLE_VADDR;

static void handle_page_fault (registers_t *regs);

/*
  Function definitions for physical paging.
*/

void paging_init(uintptr_t phys_start) {
    // Physical manager init. Ensure init page allocation is page-aligned.
    phys_ptr = (phys_start + PAGE_SIZE) & PAGE_MASK;

    // Virtual manager init.
    uint32_t i, cr0;

    register_interrupt_handler(INT_PAGE_FAULT, handle_page_fault);
    
}

uintptr_t alloc_phys_page() {
    // Sanity check.
    if (stack_phys_loc == STACK_PADDR) {
	panic("Out of memory: stack physical location == STACK_PHYS_ADDR");
    }

    // Allocate page from stack.
    phys_ptr -= sizeof(intptr_t);
    uintptr_t *stack = (uintptr_t *) stack_phys_loc;

    return *stack;
}


int free_phys_page(uintptr_t p) {
    if (p < phys_ptr) {
	return ENOTINSTACK;
    }

    if (stack_phys_max <= stack_phys_loc) {
    } else {
	uintptr_t* stack = (uintptr_t *) stack_phys_loc;
	*stack = p;
	stack_phys_loc += sizeof(uintptr_t);
    }
    return SUCCESS;
}

/*
  Function definitions for virtual paging.
*/

static void handle_page_fault (registers_t *regs) {
    uint32_t cr2;
    char strbuf[128];
    
    __asm__ __volatile__("mov %%cr2, %0" : "=r" (cr2));

    kprint("Page fault at address ");
    itoa(regs->eip, strbuf);
    kprint(strbuf);
    kprint(" Error code: ");
    itoa(regs->err_code, strbuf);
    kprint(strbuf);
    panic("Page fault complete");
    
}
