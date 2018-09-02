C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c mem/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h libc/*.h)
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o}

CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
LD = /usr/local/i386elfgcc/bin/i386-elf-ld
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb

CFLAGS = -g -m32 -ffreestanding -fno-exceptions \
		 -Wall -Wextra -Werror

# First rule run by default
image.bin: boot/bootloader.bin kernel.bin
	cat $^ > image.bin

# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case
kernel.bin: boot/kernel_entrypoint.o ${OBJ}
	$(LD) -o $@ -Ttext 0x1000 $^ --oformat binary

# Used for debugging purposes
kernel.elf: boot/kernel_entrypoint.o ${OBJ}
	$(LD) -o $@ -Ttext 0x1000 $^

run: image.bin
	qemu-system-i386 -fda image.bin

# Open the connection to qemu and load our kernel-object file with symbols
debug: image.bin kernel.elf
	qemu-system-i386 -s -fda image.bin -d guest_errors,int &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf *.bin *.dis *.o image.bin *.elf *~
	for dir in kernel boot cpu drivers libc mem; do \
		rm -rf $$dir/*.o $$dir/*~ $$dir/*.bin ;\
	done
