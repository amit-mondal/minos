
CC = nasm

bootloader.bin: bootloader.asm
	$(CC) bootloader.asm -o bootloader.bin
