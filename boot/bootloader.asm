	[org 0x7c00] ; bootloader offset

KERNEL_OFFSET equ 0x1000 ; used when linking kernel

	mov [BOOT_DRIVE], dl 	; BIOS sets up the boot drive in dl
	mov bp, 0x9000
	mov sp, bp

	mov bx, MSG_REAL_MODE
	call print
	call print_nl

	call load_kernel	; read kernel from disk
	
	call switch_to_pm
	jmp $ 			; unreachable

%include "boot/boot_print.asm"
%include "boot/boot_printhex.asm"
%include "boot/boot_disk.asm"
%include "boot/boot_gdt.asm"
%include "boot/boot_prot_print.asm"
%include "boot/boot_protected_mode.asm"


	[bits 16]

load_kernel:
	mov bx, MSG_LOAD_KERNEL
	call print
	call print_nl

	mov bx, KERNEL_OFFSET ; Read from disk and store in 0x1000
	mov dh, 31	      ; Load 31 sectors
	mov dl, [BOOT_DRIVE]
	call disk_load
	ret

	[bits 32]
	
BEGIN_PM:
	mov ebx, MSG_PROT_MODE	
	call print_string_pm
	call KERNEL_OFFSET 	; hand off to kernel
	jmp $ 			; unreachable

BOOT_DRIVE	db 0		; dl may be overwritten
MSG_REAL_MODE	db "Started in 16-bit real mode", 0
MSG_PROT_MODE	db "Loaded 32-bit protected mode", 0
MSG_LOAD_KERNEL db "Loading kernel into memory", 0
	

times 510-($-$$) db 0
dw 0xaa55
