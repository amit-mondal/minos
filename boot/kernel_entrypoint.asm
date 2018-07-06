
	[bits 32]
	[extern main] 		; calling point -> must be C main

	call main
	jmp $
