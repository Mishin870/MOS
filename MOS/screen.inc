;Адрес начала видеопамяти в режиме 3
;VIDEO_MEMORY = 0xb8000

;s_clear:
	;mov edi, VIDEO_MEMORY
	;mov ecx, 2000 ;80 * 25
	;mov al, [space]
	;mov ah, 2
	;rep stosw
;ret

;ESI = 3 * X, EDI = Y, EBX = COLOR
s_drawChar:
	movzx eax, [ModeInfo_BytesPerScanLine]
	mul edi
	add eax, esi
	mov edi, dword[ModeInfo_PhysBasePtr]
	add edi, eax
	movzx eax, [ModeInfo_BytesPerScanLine]

	mov ecx, 7
	.l1:
		mov dword[edi], ebx
		add edi, 3
	loop .l1
	sub edi, 3
	mov ecx, 6
	.l2:
		add edi, eax
		sub edi, 18
		mov dword[edi], ebx
		add edi, 18
		mov dword[edi], ebx
	loop .l2
ret

;ESI = 3 * X, EDI = Y, ECX = LEN, EBX = COLOR
s_drawLine:
	movzx eax, [ModeInfo_BytesPerScanLine]
	mul edi
	add eax, esi
	mov edi, dword[ModeInfo_PhysBasePtr]
	add edi, eax
	.drawLoop:
		mov dword[edi], ebx
		add edi, 3h
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
			mov dword[edi], ebx
			add edi, 3h
		loop .drawXLoop
		pop ecx

		mov edi, edx
		add edi, eax
		mov edx, edi
	loop .drawYLoop
ret