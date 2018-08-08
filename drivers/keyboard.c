#include "keyboard.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include "../libc/stddef.h"
#include "screen.h"


// Keyboard input sent to this buffer.
static char kb_input_buf[KB_INPUT_BUFFER_SIZE];
static size_t kb_buf_len = 0;

// If this is ever non-null, someone is waiting on a read.
static void* dest_buffer = NULL;
static int recvd_return = 0;
static size_t read_len = 0;

// String mapping for scan code.
const char *sc_str[] = { "ERROR", "Esc", "1", "2", "3", "4", "5", "6", 
			 "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E", 
			 "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl", 
			 "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`", 
			 "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".", 
			 "/", "RShift", "Keypad *", "LAlt", "Spacebar"};

// Char mapping for scan code.
const char sc_char[] = { '?', '?', '1', '2', '3', '4', '5', '6',     
			 '7', '8', '9', '0', '-', '=', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y', 
			 'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G', 
			 'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V', 
			 'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};

static void keyboard_cb(registers_t regs UNUSED) {

    // If no one's waiting on the input, just return;
    if (!dest_buffer) {
	return;
    }

    uint8_t scancode = port_byte_in(PIC_SCANCODE_PORT);

    // Ignore unknown scancodes.
    if (scancode > SCANCODE_MAX) {
	return;
    }

    switch(scancode) {
    case KB_BACKSPACE:
	trimlast(kb_input_buf);
	if (kb_buf_len) {
	    kb_buf_len--;
	}
	kprint_backspace();
	break;
    case KB_ENTER:
	kprint("\n");
	if (dest_buffer) {
	    recvd_return = 1;
	    memcpy(dest_buffer, kb_input_buf, kb_buf_len);
	    read_len = kb_buf_len;
	}
	kb_input_buf[0] = '\0';
	kb_buf_len = 0;
	break;
    default: {
	if (kb_buf_len < KB_INPUT_BUFFER_SIZE) {	    
	    char char_val = sc_char[(int) scancode];
	    char str[2] = {char_val, '\0'};
	    append(kb_input_buf, char_val);
	    kb_buf_len++;
	    kprint(str);
	}
    }	
    }
    
}

size_t read_raw_kb(void* buf, size_t len) {    
    dest_buffer = buf;
    size_t ret_read_len;
    size_t current_kb_buf_len = kb_buf_len;
    while ((kb_buf_len - current_kb_buf_len) < len &&
	   !recvd_return) {
	// Spin until we have len bytes to return, or until we get
	// a return char.      
    }
    if (recvd_return) {      
	ret_read_len = read_len;
    } else {
	memcpy(buf, kb_input_buf, len);
	ret_read_len = len;
    }
    // Reset read vars.
    read_len = 0;
    recvd_return = 0;
    dest_buffer = NULL;
    return ret_read_len;
}


void init_keyboard() {
    register_interrupt_handler(IRQ1, keyboard_cb);
}
