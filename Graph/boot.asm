use16 
org 7C00h 
;**************************** 
; Realmode startup code. 
;**************************** 
;Здесь код для работы с caps lock (возможно)
;in al, 0x92 
;or al, 2
;out 0x92, al 

start:
	push cs
	pop ds
	
	push ds
	pop es
;**************************************************
;Вычисляем адрес сегмента кода для GDT
;**************************************************
	;mov ax, cs
	;mov dl, ah
	;shr dl, 4
	;shl ax, 4
	;Адрес gdt
	;add ax, gdt
	;В случае переполнения
	;adc dl, 0
	
	;mov word[sys_code + 2], ax
	;mov byte[sys_code + 4], dl
;****************************
;Настройка VESA
;****************************
	mov ax, 4f01h 
	mov di, Mode_Info        
	mov cx, 4f15h
	int 10h
	
	mov bx, cx
	mov  ax, 4f02h
	int  10h
;***************************************
;Установка защищенного режима
;***************************************
	cli  
	lgdt  [gdtr] 

	mov eax, cr0 
	or al, 0x1
	mov cr0, eax 

	jmp   0x10: protected 

;************************************
;Начало защищенного режима
;************************************
use32 
protected: 
	mov ax, 0x8  
	mov ds, ax 
	mov es, ax 
	mov ss, ax 
;***************************** 
; Turn floppy off. 
;***************************** 
	mov dx, 3F2h
	xor al, al
	out dx, al
;******************************
;Рисование
;******************************
mov esi, 30		;x * 3
mov edi, 10		;y
mov ebx, 0x00FF00	;color = green
;mov ecx, 50		;width
;push ecx
;mov ecx, 50		;height
;push ecx
call drawChar

mov esi, 54
mov edi, 10
call drawChar

;;;;jmp loadKernel

;mov bx, 410ch
;mov  ax, 4f02h
;int  10h

hlt

;ESI = 3 * X, EDI = Y, EBX = COLOR
drawChar:
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
drawLine:
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
drawRect:
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

;***********************************
;Загрузка ядра (не работает)
;***********************************
loadKernel:
	xor eax, eax
	mov esp, eax
	mov ebx, 500h

	xor ecx, ecx
	mov ch, 0h      ;номер цилиндра - 0
	mov cl, 02h	    ;начальный сектор - 2

	xor edx, edx
	mov dh, 0h	    ;номер головки - 0
	mov dl, 80h	    ;жесткий диск - 80h

	xor eax, eax
	mov al, 02h	    ;кол-во читаемых секторов -1
	mov ah, 02h
	int 13h
	;jmp 0000:0500h	    ;переход на 0000:0500h, куда загружен второй сектор
	hlt

;*************************************
; GDT
;*************************************
;Пустая обязательная запись!
gdt: dw 0x0000, 0x0000, 0x0000, 0x0000
;word(предел сегмента)
;word(младшее слово адреса)
;word(byte(старший байт адреса), byte(доступ))
;word(резерв для i80286. Должно быть равно 0)
sys_data: dw 0xFFFF, 0x0000, 0x9200, 0x00CF
;0x9800
sys_code: dw 0xFFFF, 0x0000, 0x9800, 0x00CF
gdt_end: 
;Длина GDT - 1
gdtr: dw gdt_end - gdt - 1                                           
dd gdt

;============================== VESA ИНФОРМАЦИЯ ===========================================
Mode_Info:               
ModeInfo_ModeAttributes         rw      1 
ModeInfo_WinAAttributes         rb      1 
ModeInfo_WinBAttributes         rb      1 
ModeInfo_WinGranularity         rw      1 
ModeInfo_WinSize                rw      1 
ModeInfo_WinASegment            rw      1 
ModeInfo_WinBSegment            rw      1 
ModeInfo_WinFuncPtr             rd      1 
ModeInfo_BytesPerScanLine       rw      1 
ModeInfo_XResolution            rw      1
ModeInfo_YResolution            rw      1 
ModeInfo_XCharSize              rb      1 
ModeInfo_YCharSize              rb      1 
ModeInfo_NumberOfPlanes         rb      1 
ModeInfo_BitsPerPixel           rb      1 
ModeInfo_NumberOfBanks          rb      1 
ModeInfo_MemoryModel            rb      1 
ModeInfo_BankSize               rb      1 
ModeInfo_NumberOfImagePages     rb      1 
ModeInfo_Reserved_page          rb      1 
ModeInfo_RedMaskSize            rb      1 
ModeInfo_RedMaskPos             rb      1 
ModeInfo_GreenMaskSize          rb      1 
ModeInfo_GreenMaskPos           rb      1 
ModeInfo_BlueMaskSize           rb      1 
ModeInfo_BlueMaskPos            rb      1 
ModeInfo_ReservedMaskSize       rb      1 
ModeInfo_ReservedMaskPos        rb      1 
ModeInfo_DirectColorModeInfo    rb      1 
; VBE 2.0 extensions 
ModeInfo_PhysBasePtr            rd      1
ModeInfo_OffScreenMemOffset     rd      1 
ModeInfo_OffScreenMemSize       rw      1

times 510-($-$$) db 0
dw 0xAA55