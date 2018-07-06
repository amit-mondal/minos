
print:
	pusha

start:
	mov al, [bx]
	or al, 0
	jz done

	mov ah, 0x0e
	int 0x10

	add bx, 1
	jmp start

done:
	popa
	ret

print_nl:
	pusha			; save regs
	mov ah, 0x0e 		; tty mode
	mov al, 0x0a		; newline char
	int 0x10		; print interrupt	
	mov al, 0x0d		; cr char
	int 0x10		; print interrupt
	popa			; restore regs
	ret
