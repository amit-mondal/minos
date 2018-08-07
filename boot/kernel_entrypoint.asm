
	[bits 32]
	[extern kmain] 		; calling point to enter kernel

	call kmain
	jmp $
