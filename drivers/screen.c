#include "screen.h"
#include "../cpu/ports.h"
#include "../libc/mem.h"


int get_cursor_offset();
void set_cursor_offset(int offset);
int print_char(char c, int col, int row, char attr);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);


/**
 * Print a message on the specified location
 * If col, row, are negative, we will use the current offset
 */
void kprint_at(const char *message, int col, int row) {
    /* Set cursor if col/row are negative */
    int offset;
    if (col >= 0 && row >= 0)
        offset = get_offset(col, row);
    else {
        offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }

    /* Loop through message and print it */
    int i = 0;
    while (message[i] != 0) {
        offset = print_char(message[i++], col, row, WHITE_ON_BLACK);
        /* Compute row/col for next iteration */
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}

void kprint(const char *message) {
    kprint_at(message, -1, -1);
}

void kprint_backspace() {
    int cursor_offset = get_cursor_offset();
    int row = get_offset_row(cursor_offset - 2);
    int col = get_offset_col(cursor_offset - 2);
    // We're going to use the delete char like UNIXes, although
    // we could use backspace like Windows.
    print_char(DELETE_SCR, col, row, WHITE_ON_BLACK);
}


int print_char(char c, int col, int row, char attr) {
    uint8_t *vidmem = (unsigned char*) VIDEO_ADDRESS;
    
    if (!attr) {
	attr = WHITE_ON_BLACK;
    }

    /* Error control: print a red 'E' if the coords aren't right */
    if (col >= MAX_COLS || row >= MAX_ROWS) {
        vidmem[2*(MAX_COLS)*(MAX_ROWS)-2] = 'E';
        vidmem[2*(MAX_COLS)*(MAX_ROWS)-1] = RED_ON_WHITE;
        return get_offset(col, row);
    }

    int offset;
    if (col >= 0 && row >= 0) {
	offset = get_offset(col, row);
    }
    else {
	offset = get_cursor_offset();
    }

    switch(c) {
    case NEWLINE:
	row = get_offset_row(offset);
	offset = get_offset(0, row + 1);
	break;
    case BACKSPACE_SCR:
    case DELETE_SCR:
	vidmem[offset] = EMPTY;
	vidmem[offset + 1] = attr;
	break;
    default:
        vidmem[offset] = c;
        vidmem[offset+1] = attr;
        offset += 2;	
    }     

    // Scrolling support by checking cursor is past end of screen
    if (offset >= SCREEN_END) {
	int i;
	for (i = 1;i < MAX_ROWS;i++) {
	    // Copy all of video memory up.
	    memcpy((char*) get_offset(0, i) + VIDEO_ADDRESS,
			 (char*) get_offset(0, i - 1) + VIDEO_ADDRESS,
			 MAX_COLS * 2);
	}

	char* last_line = (char*) get_offset(0, MAX_ROWS - 1) + VIDEO_ADDRESS;
	for (i = 0;i < MAX_COLS * 2;i++) {
	    last_line[i] = 0;
	}

	offset -= 2 * MAX_COLS;
    }
    
    set_cursor_offset(offset);
    return offset;
}

int get_cursor_offset() {
    /* Use the VGA ports to get the current cursor position
     * 1. Ask for high byte of the cursor offset (data 14)
     * 2. Ask for low byte (data 15)
     */
    port_byte_out(REG_SCREEN_CTRL, CURSOR_HIGH_BYTE);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8; /* High byte: << 8 */
    port_byte_out(REG_SCREEN_CTRL, CURSOR_LOW_BYTE);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2; /* Position * size of character cell */
}

void set_cursor_offset(int offset) {
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, CURSOR_HIGH_BYTE);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, CURSOR_LOW_BYTE);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

void clear_screen() {
    int screen_size = MAX_COLS * MAX_ROWS;
    int i;
    char *screen = (char*) VIDEO_ADDRESS;

    for (i = 0; i < screen_size; i++) {
        screen[i*2] = ' ';
        screen[i*2+1] = WHITE_ON_BLACK;
    }
    set_cursor_offset(get_offset(0, 0));
}


int get_offset(int col, int row) {
    return 2 * (row * MAX_COLS + col);
}

int get_offset_row(int offset) {
    return offset / (2 * MAX_COLS);
}

int get_offset_col(int offset) {
    return (offset - (get_offset_row(offset)*2*MAX_COLS))/2;
}
