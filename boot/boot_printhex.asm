

print_hex:
	pusha
	mov cx, 0 		; loop index

hex_loop:
	cmp cx, 4 		; 4 chars at most
	jz end

	mov ax, dx
	and ax, 0x00f
	add al, 0x30 		; convert to ASCII
	cmp al, 0x39 		; check for > 9 for A-F
	jle step2
	add al, 7 		; add 7 to get letter

step2:
	mov bx, HEX_OUT + 5
	sub bx, cx
	mov [bx], al
	shr dx, 4

	add cx, 1
	jmp hex_loop


end:
	mov bx, HEX_OUT
	call print

	popa
	ret

HEX_OUT:
	db '0x0000', 0 		; reserve space for string
