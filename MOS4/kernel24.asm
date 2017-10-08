use32
org 500h

;Загрузка таблицы прерываний и запуск прерываний
lidt [idtr]
sti

;Загрузка информации о VESA из указателя
pop esi
mov edi, Mode_Info
mov ecx, dword[Mode_Info_len]
rep movsb

call r_init
call k_initKeys

;Заливка цветом (P.S. градиентом) (P.P.S. не работает как надо, но даже лучше)
mov eax, 0x00829c82
mov esi, 0x00FFFFFF
call s_grad

call set_int_ctrlr

;Тест отрисовки шрифта
mov ecx, 33
mov esi, 10 * S_BBP
mov eax, 0
fontLoop:
	push ecx
	push esi
	push eax
	shl eax, 3
	add eax, mfont
	;mov esi, 10 * S_BBP
	mov edi, 20
	mov ebx, 0x0000FF00
	mov edx, FONT_WID
	call s_drawChar8
	pop eax
	pop esi
	pop ecx
	add esi, 10 * S_BBP
	inc eax
loop fontLoop

;ESI = 4 * X, EDI = Y, EAX = WIDTH, EDX = HEIGHT
mov esi, 10 * S_BBP
mov edi, 100
mov eax, 150
mov edx, 100
mov ebx, wtitle
mov ecx,  4
call s_drawWindow

int 48

;call k_pause
;mov esi, 300 * S_BBP
;mov edi, 100
;mov eax, 150
;mov edx, 100
;mov ebx, wtitle2
;mov ecx,  11
;call s_drawWindow

;call k_pause
;mov esi, 250 * S_BBP
;mov edi, 180
;mov eax, 150
;mov edx, 100
;mov ebx, wtitle3
;mov ecx,  7
;call s_drawWindow

;keybLoop:
;	call k_getch
;	
;jmp keybLoop

hltLoop:
hlt
jmp hltLoop

include 'random.inc'
include 'screen24.inc'
include 'keyboard.inc'
include 'kernel_cmos.inc'

macro outb pnum, val {
	mov dx, pnum
	mov al, val
	out dx, al
}
set_int_ctrlr:
	;Инициализация
	outb 0x20, 0x11
	outb 0xA0, 0x11
	;Базовые номера векторов
	outb 0x21, 0x20
	outb 0xA1, 0x28
	;...
	outb 0x21, 0x04
	outb 0xA1, 0x02
	
	outb 0x21, 0x01
	outb 0xA1, 0x01
	
	outb 0x21, 0x0
	outb 0xA1, 0x0
ret

test1:
	;pushad
	mov esi, 25 * S_BBP
	mov edi, 200
	mov eax, 150
	mov edx, 100
	mov ebx, wtitle2
	mov ecx,  5
	call s_drawWindow
	;popad
iret

include 'kernel_exc.inc'

showErr:
	mov bx, 10

	xor dx, dx
	div bx
	add dl, 33
	mov byte[werror + 7], dl

	xor dx, dx
	div bx
	add dl, 33
	mov byte[werror + 6], dl

	mov ax, cx
	xor dx, dx
	div bx
	add dl, 33
	mov byte[werror + 11], dl
	
	xor dx, dx
	div bx
	add dl, 33
	mov byte[werror + 10], dl

	xor dx, dx
	div bx
	add dl, 33
	mov byte[werror + 9], dl

	xor dx, dx
	div bx
	add dl, 33
	mov byte[werror + 8], dl
	
	mov esi, 500 * S_BBP

	mov edi, dword[currY]
	add edi, 20
	mov dword[currY], edi

	mov eax, 150
	mov edx, 100
	mov ebx, werror
	mov ecx,  12
	call s_drawWindow
	popad
iret

include 'kernel_ext.inc'

showExt:
	mov bx, 10

	xor dx, dx
	div bx
	add dl, 33
	mov byte[wext + 8], dl

	xor dx, dx
	div bx
	add dl, 33
	mov byte[wext + 7], dl

	mov esi, 250 * S_BBP

	mov edi, dword[currEY]
	add edi, 20
	mov dword[currEY], edi

	mov eax, 150
	mov edx, 100
	mov ebx, wext
	mov ecx,  9
	call s_drawWindow
	
	outb 0x20, 0x20
	
	popad
iret

showExt2:
	mov bx, 10

	xor dx, dx
	div bx
	add dl, 33
	mov byte[wext + 8], dl

	xor dx, dx
	div bx
	add dl, 33
	mov byte[wext + 7], dl

	mov esi, 250 * S_BBP

	mov edi, dword[currEY]
	add edi, 20
	mov dword[currEY], edi

	mov eax, 150
	mov edx, 100
	mov ebx, wext
	mov ecx,  9
	call s_drawWindow
	
	outb 0xa0, 0x20
	
	popad
iret

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
Mode_Info_len:
dd Mode_Info_len - Mode_Info

;================IDT====================
ACC_PRESENT = 10000000b
ACC_INT_GATE = 00001110b
ACC_TRAP_GATE = 00001111b

idt:
macro exception func {
	dw ((func shl 0x30) shr 0x30)
	dw 0x8
	db 0, ACC_PRESENT OR ACC_TRAP_GATE
	dw (func shr 0x10)
}
	exception exc0
	exception exc1
	exception exc2
	exception exc3
	exception exc4
	exception exc5
	exception exc6
	exception exc7
	exception exc8
	exception exc9
	exception exc10
	exception exc11
	exception exc12
	exception exc13
	exception exc14
	exception exc15
	exception exc16
	exception exc17
	exception exc18
	exception exc19
	exception exc20
	exception exc21
	exception exc22
	exception exc23
	exception exc24
	exception exc25
	exception exc26
	exception exc27
	exception exc28
	exception exc29
	exception exc30
	exception exc31

macro external func {
	dw ((func shl 0x30) shr 0x30)
	dw 0x8
	db 0, ACC_PRESENT OR ACC_INT_GATE
	dw (func shr 0x10)
}
	external ext0
	external ext1
	external ext2
	external ext3
	external ext4
	external ext5
	external ext6
	external ext7
	external ext8
	external ext9
	external ext10
	external ext11
	external ext12
	external ext13
	external ext14
	external ext15
	
	test_func:
	dw ((test1 shl 0x30) shr 0x30)		;Низшая часть адреса
	dw 0x8					;Селектор
	db 0, ACC_PRESENT OR ACC_INT_GATE	;Тип (доступ)
	dw (test1 shr 0x10)				;Высшая часть адреса
idt_end:
;Длина IDT - 1
idtr: dw 187h
dd idt
;=======================================

currY dd 200
currEY dd 20
wtitle db 19, 5, 18, 19 ;4
wtitle2 db 15, 11, 14, 15, 35 ;5
werror db 15, 25, 9, 1, 11, 0, ?, ?, ?, ?, ?, ? ;12
wext db 2, 14, 5, 25, 14, 5, 5, ?, ? ;9
wmsglen dw 0
wmsg db 20 dup(?)
mfont: file "mfont.data"