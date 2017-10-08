FONT_WID = 392
S_BBP = 3
wincol dd 0x0000007B

;EAX = COLOR
s_clear:
	mov edi, dword[ModeInfo_PhysBasePtr]
	mov dx, word[ModeInfo_XResolution]
	movzx ebx, word[ModeInfo_YResolution]
	.clearLoop:
		mov cx, dx
		.repLoop:
			mov [edi], eax
			add edi, 3
		loop .repLoop
		dec ebx
	jnz .clearLoop
ret

;AL - COLOR CODE
s_setWinCol:
	.c0:
		cmp al, 0
		jne .c1
		mov dword[wincol], 0x0000007B
		jmp .end
	.c1:
		cmp al, 1
		jne .c2
		mov dword[wincol], 0x00007B00
		jmp .end
	.c2:
		cmp al, 2
		jne .c3
		mov dword[wincol], 0x007B0000
		jmp .end
	.c3:
		cmp al, 3
		jne .c4
		mov dword[wincol], 0x007B007B
		jmp .end
	.c4:
		cmp al, 4
		jne .c5
		mov dword[wincol], 0x00007B7B
		jmp .end
	.c5:
		cmp al, 5
		jne .c7
		mov dword[wincol], 0x007B7B00
		jmp .end
	.c7:
		cmp al, 7
		jne .end
		mov dword[wincol], 0x007B7B7B
		jmp .end
	.end:
ret

GRAD_LEN = 3
;EAX = START, ESI = END
s_grad:
	mov edi, dword[ModeInfo_PhysBasePtr]
	mov dx, word[ModeInfo_XResolution]
	movzx ebx, word[ModeInfo_YResolution]
	mov ecx, GRAD_LEN
	.gradLoop:
		push ecx
		mov cx, dx
		.repLoop:
			mov [edi], eax
			add edi, 3
		loop .repLoop
		pop ecx
		dec ecx
		jnz .skip
		add eax, 0x010100
		cmp eax, esi
		jnb .cont
			mov eax, esi
		.cont:
		mov ecx, GRAD_LEN
		.skip:
		dec ebx
	jnz .gradLoop
ret

;ESI = 3 * X, EDI = Y, EAX = WIDTH, EDX = HEIGHT, EBX = TITLE, ECX = TITLE LEN
s_drawWindow:
	push ebx
	push ecx
	;main rect
	push esi
	push edi
	push eax
	push edx
		mov ebx, 0x00bdbdbd
		push eax
		push edx
		call s_drawRect
	pop edx
	pop eax
	pop edi
	pop esi

	;title
	push esi
	push edi
	push eax
	push edx
	add esi, 9 ;4 * 3
	add edi, 3
	sub eax, 6
	mov edx, 18
		mov ebx, dword[wincol]
		push eax
		push edx
		call s_drawRect
	pop edx
	pop eax
	pop edi
	pop esi

	;down
	push edi
	push eax
	push edx
	add edi, edx
	dec edi
		mov ebx, 0
		mov ecx, eax
		call s_drawLine
	pop edx
	pop eax
	pop edi

	;right
	push esi
	push edi
	push eax
	push edx
		dec eax
		push edx
		mov ecx, 3
		mul ecx
		pop edx
	add esi, eax
		mov ebx, 0
		mov ecx, edx
		call s_drawLineVer
	pop edx
	pop eax
	pop edi
	pop esi

	;up
	push esi
	push edi
	push eax
	push edx
	inc edi
	add esi, 3
	sub eax, 2
		mov ebx, 0x00FFFFFF
		mov ecx, eax
		call s_drawLine
	pop edx
	pop eax
	pop edi
	pop esi

	;left
	push esi
	push edi
	push eax
	push edx
	add esi, 3
	sub edx, 2
		mov ebx, 0x00FFFFFF
		mov ecx, edx
		call s_drawLineVer
	pop edx
	pop eax
	pop edi
	pop esi

	;down2
	push esi
	push edi
	push eax
	push edx
	add edi, edx
	sub edi, 2
	add esi, 3
	sub eax, 2
		mov ebx, 0x007b7b7b
		mov ecx, eax
		call s_drawLine
	pop edx
	pop eax
	pop edi
	pop esi

	;right2
	push esi
	push edi
	push eax
	push edx
		sub eax, 2
		push edx
		mov ecx, 3
		mul ecx
		pop edx
	add esi, eax
	inc edi
	sub edx, 2
		mov ebx, 0x007b7b7b
		mov ecx, edx
		call s_drawLineVer
	pop edx
	pop eax
	pop edi
	pop esi

	pop ecx
	pop ebx
	xor eax, eax
	mov edx, ebx
	mov ebx, esi
	add ebx, 6 * S_BBP
	add edi, 7
	.drawTitle:
		push esi
		mov esi, edx
		mov al, byte[esi]
		inc edx
		pop esi

		push edx
		push ebx
		push ecx
		push esi
		push eax
		push edi
		shl eax, 3
		add eax, mfont
		mov esi, ebx
		mov ebx, 0x00FFFFFF
		mov edx, FONT_WID
		call s_drawChar8
		pop edi
		pop eax
		pop esi
		pop ecx
		pop ebx
		pop edx

		add ebx, 9 * S_BBP
		inc esi
	loop .drawTitle
ret

;EAX = FONT CHAR OFFSET+POINTER, ESI = 3 * X, EDI = Y, EBX = COLOR, EDX = FONT MAIN WIDTH
s_drawChar8:
	push edx
	push eax
	movzx eax, [ModeInfo_BytesPerScanLine]
	mul edi
	add eax, esi
	mov edi, dword[ModeInfo_PhysBasePtr]
	add edi, eax
	movzx eax, [ModeInfo_BytesPerScanLine]
	pop esi
	pop edx
	;dec esi

	mov ecx, 8
	.heightLoop:
		push ax
		push ecx
		mov ecx, 8
		.widthLoop:
			mov al, byte[esi]
			test al, al
			jz .skip
				mov dword[edi], ebx
			.skip:
			add edi, 3
			inc esi
		loop .widthLoop
		pop ecx
		pop ax
		sub edi, 24
		sub esi, 8
		add edi, eax
		add esi, edx
	loop .heightLoop
ret

;ESI = 3 * X, EDI = Y, ECX = LEN, EBX = COLOR
s_drawLine:
	movzx eax, [ModeInfo_BytesPerScanLine]
	mul edi
	add eax, esi
	mov edi, dword[ModeInfo_PhysBasePtr]
	add edi, eax
	.drawLoop:
		mov [edi], ebx
		add edi, 3
	loop .drawLoop
ret

;ESI = 3 * X, EDI = Y, ECX = LEN, EBX = COLOR
s_drawLineVer:
	movzx eax, [ModeInfo_BytesPerScanLine]
	mul edi
	add eax, esi
	mov edi, dword[ModeInfo_PhysBasePtr]
	add edi, eax
	movzx eax, [ModeInfo_BytesPerScanLine]
	.drawLoop:
		mov [edi], ebx
		add edi, eax
	loop .drawLoop
ret

;ESI = 3 * X, EDI = Y, EBX = COLOR, STACK = "push width, push height"
s_drawRect:
	movzx eax, [ModeInfo_BytesPerScanLine]
	mul edi
	add eax, esi
	mov edi, dword[ModeInfo_PhysBasePtr]
	add edi, eax

	pop eax		;eax = caller address
	pop ecx		;ecx = height
	pop esi		;esi = width
	push eax

	mov edx, edi
	movzx eax, [ModeInfo_BytesPerScanLine]
	.drawYLoop:
		push ecx
		mov ecx, esi
		.drawXLoop:
			mov [edi], ebx
			add edi, 3
		loop .drawXLoop
		pop ecx

		mov edi, edx
		add edi, eax
		mov edx, edi
	loop .drawYLoop
ret