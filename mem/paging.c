

#include "paging.h"
#include "../cpu/isr.h"
#include "../kernel/panic.h"
#include "../drivers/screen.h"
#include "../libc/string.h"
#include "../libc/mem.h"

/*
  State for managing physical pages.
*/

// Current position (stack pointer) in physical addr stack.
static uintptr_t stack_phys_loc = STACK_PADDR;
// Max position of current addr stack.
static uintptr_t stack_phys_max = STACK_PADDR;

static uintptr_t phys_ptr;

/*
  State for managing virtual pages (and mappings).
*/

static uintptr_t* page_dir = (uintptr_t *) PAGE_DIR_VADDR;
static uintptr_t* page_tables = (uintptr_t*) PAGE_TABLE_VADDR;

// This is the page directory we're using right now.
static page_dir_t* current_dir;

static int paging_enabled;

static void handle_page_fault (registers_t *regs);

/*
  Function definitions for physical paging.
*/

void paging_init(uintptr_t phys_start) {
    // Physical manager init. Ensure init page allocation is page-aligned.
    phys_ptr = (phys_start + PAGE_SIZE) & PAGE_MASK;

    /*
      Virtual manager init.
    */
    uint32_t i;

    // Register page fault handler.
    register_interrupt_handler(INT_PAGE_FAULT, handle_page_fault);

    // Create and initialize a page directory.
    page_dir_t* dir = (page_dir_t *) alloc_phys_page();
    memset(page_dir, 0, PAGE_DIR_SIZE);

    /*
      First 4 MB are getting identity mapped so our kernel doesn't 
      die at the hands of the MMU.
    */
    dir[0] = alloc_phys_page() | FL_PAGE_PRESENT | FL_PAGE_RW;
    uintptr_t* table = (uintptr_t *) (dir[0] & PAGE_MASK);
    // We're filling up the entire page table.
    for (i = 0; i < PAGE_TABLE_NUM_ENTRIES; i++) {
	// We multiply by PAGE_SIZE to zero out the attribute bits.
	table[i] = (i * PAGE_SIZE) | FL_PAGE_PRESENT | FL_PAGE_RW;
    }

    // Now allocate and initialize the second-last table.
    dir[PAGE_TABLE_NUM_ENTRIES - 2] = alloc_phys_page() | FL_PAGE_PRESENT | FL_PAGE_RW;
    table = (uintptr_t*) (dir[PAGE_TABLE_NUM_ENTRIES - 2] & PAGE_MASK);
    memset(table, 0, PAGE_SIZE);
    
    // Last entry of the second-last table points to the directory itself.
    table[PAGE_TABLE_NUM_ENTRIES - 1] = ((uintptr_t) dir) | FL_PAGE_PRESENT | FL_PAGE_RW;

    switch_page_dir(dir);

    enable_paging();

    /*
      Map the page table that has the physical stack, otherwise the MMU will complain
      when we first try to free a physical page.
    */
    uint32_t table_index = PAGE_DIR_INDEX((STACK_PADDR >> 12));
    page_dir[table_index] = alloc_phys_page() | FL_PAGE_PRESENT | FL_PAGE_RW;
    memset((void*) page_tables[table_index * PAGE_TABLE_NUM_ENTRIES], 0, PAGE_SIZE);

    paging_enabled = 1;	
    
}


uintptr_t alloc_phys_page() {

    /*
      If virtual paging is enabled, use the stack, otherwise
      use the dumb method.
     */
    
    if (paging_enabled) {
    
	// Sanity check.
	if (stack_phys_loc == STACK_PADDR) {
	    panic("Out of memory: stack physical location is at STACK_PHYS_ADDR");
	}

	// Allocate page from stack.
	phys_ptr -= sizeof(intptr_t);
	uintptr_t *stack = (uintptr_t *) stack_phys_loc;

	return *stack;
    } else {
	return phys_ptr += PAGE_SIZE;
    }
}


int free_phys_page(uintptr_t p) {

    /*
      We don't want to free early allocated pages, since
      they contain important structures
    */
    
    if (p < phys_ptr) {
	return ENOTINSTACK;
    }

    if (stack_phys_max <= stack_phys_loc) {

	/*
	  If the stack's out of space, map the page into the
	  current address space for extra stack space.
	*/
	
	vpage_map(stack_phys_max, p, FL_PAGE_PRESENT | FL_PAGE_RW);
	stack_phys_max += PAGE_SIZE;
	
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

void enable_paging() {
    uint32_t cr0;
    __asm__ __volatile__("mov %%cr0, %0" : "=r" (cr0));
    cr0 |= CR0_PAGING_ENABLE_BIT;
    __asm__ __volatile__("mov %0, %%cr0" : : "r" (cr0));    
}

void switch_page_dir(page_dir_t* dir) {
    current_dir = dir;
    __asm__ __volatile__("mov %0, %%cr3" : : "r" (dir));
}

void vpage_map(uintptr_t virt_addr, uintptr_t phys_addr, uint32_t flags) {
    uintptr_t virt_page = virt_addr / PAGE_SIZE;
    uintptr_t table_index = PAGE_DIR_INDEX(virt_page);

    // Find the right page table to put virt_addr in.
    
    if (page_dir[table_index] == 0) {
	// We need to create the page table for this page.
	page_dir[table_index] = alloc_phys_page() | FL_PAGE_PRESENT | FL_PAGE_RW;
	memset((void*) page_tables[table_index * PAGE_TABLE_NUM_ENTRIES], 0, PAGE_SIZE);
    }

    // Now that we know the page table exists, update the page table entry.
    page_tables[virt_page] = (phys_addr & PAGE_MASK) | flags;
}

void vpage_unmap(uintptr_t virt_addr) {
    uintptr_t virt_page = virt_addr / PAGE_SIZE;
    page_tables[virt_page] = 0;

    __asm__ __volatile__("invlpg (%0)" : : "a" (virt_addr));
}

int vpage_get_mapping(uintptr_t virt_addr, uintptr_t* phys_addr) {
    uintptr_t virt_page = virt_addr / PAGE_SIZE;
    uint32_t table_index = PAGE_DIR_INDEX(virt_addr);

    // Find page table for virt_addr.
    if (page_dir[table_index] == 0) {
	return ENOMAPPING;
    }

    if (page_tables[virt_page] != 0) {
	if (phys_addr) {
	    *phys_addr = page_tables[virt_page] & PAGE_MASK;
	}
	return SUCCESS;
    }

    return ENOMAPPING;
}

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
