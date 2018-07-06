	[bits 16]

switch_to_pm:
	cli			; disable interrupts
	lgdt [gdt_descriptor]
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax 		; set 32-bit mode bit in cr0
	jmp CODE_SEG:init_pm 	; far jump to clear pipeline
	

	[bits 32]

init_pm:
	mov ax, DATA_SEG 	; update segment registers
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov ebp, 0x90000 	; update stack to top of free space
	mov esp, ebp

	call BEGIN_PM 		; go to useful code
