
#ifndef DRIVERS_SCREEN_H
#define DRIVERS_SCREEN_H

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define SCREEN_END MAX_ROWS * MAX_COLS * 2
#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4

#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

#define CURSOR_HIGH_BYTE 14
#define CURSOR_LOW_BYTE 15


void clear_screen();
void kprint_at(char *message, int col, int row);
void kprint(char *message);

#endif
