use16 
org 7C00h 
;*********************************
;Начало в реальном режиме
;*********************************
start:
	push cs
	pop ds
	
	push ds
	pop es

	mov byte[drive], dl
;*******************************
;Включение линии A20
;*******************************
	in al, 0x92
	or al, 2
	out 0x92, al
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

	mov dl, byte[drive]
	mov al, byte[ModeInfo_BitsPerPixel]
	cmp al, 32
	je load32

	load24:
	mov ah, 2h
	xor dh, dh
	xor ch, ch
	mov cl, 2
	mov al, 16
	mov bx, 500h
	int 13h
	jmp endLoad

	load32:
	mov ah, 2h
	xor dh, dh
	xor ch, ch
	mov cl, 17
	mov al, 16
	mov bx, 500h
	int 13h
	jmp endLoad

	endLoad:
	;Загрузка программы в user mode
	mov dl, byte[drive]
	mov ah, 2h
	xor dh, dh
	xor ch, ch
	mov cl, 33
	mov al, 1
	mov bx, 2500h
	int 13h
;***************************************
;Установка защищенного режима
;***************************************
	cli  
	lgdt  [gdtr] 

	mov eax, cr0 
	or al, 0x1
	mov cr0, eax 

	;CS:EIP
	jmp 0x8:protected

;************************************
;Начало защищенного режима
;************************************
use32 
protected:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov sp, 0xffff
;***************************** 
; Turn floppy off.
;***************************** 
	mov dx, 3F2h
	xor al, al
	out dx, al

;***********************************
;Загрузка ядра
;***********************************
	;Вывод отладочной информации в COM1 порт в QEMU
	;mov dx, 0x3F8
	;mov al, 'l'
	;out dx, al


	;mov edi, 500h
	;mov cl, 8
	
	;xor edx, edx
	;mov dl, byte[drive]
	;shl edx, 24
	;mov eax, 1
	;or eax, edx
	
	;call d_read
	
	push Mode_Info
	jmp 0x8:500h
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
;0x8
sys_code: dw 0xFFFF, 0x0000, 0x9A00, 0x00CF
;0x10
sys_data: dw 0xFFFF, 0x0000, 0x9200, 0x00CF
;0x18
;0x9A00 (0xE100) => 0xFA00
user_code: dw 0xFFFF, 0x2500, 0xFA00, 0x00CF
;0x20
;0x9200 => 0xF200
user_data: dw 0xFFFF, 0x2500, 0xF200, 0x00CF

gdt_end: 
;Длина GDT - 1
gdtr: dw gdt_end - gdt - 1                                           
dd gdt

drive db ?

scr_mode db 0
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