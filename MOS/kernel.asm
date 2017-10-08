;16-битная адресация, пока мы находимся в реальном режиме
use16
org 500h
start:
	jmp 0x0000:ent		;теперь CS=0, IP=7C00h
ent:
	mov ax, cs
	mov ds, ax

	;открыть A20
	in al, 0x92
	or al, 2
	out 0x92, al

;****************************
;Настройка VESA
	mov ax, 4f01h 
	mov di, Mode_Info        
	mov cx, 4f15h
	int 10h
	
	mov bx, cx
	mov  ax, 4f02h
	int  10h
;****************************

	;Загрузить адрес и размер GDT в GDTR
	lgdt [gdtr]
	;Запретить прерывания
	cli
	;Запретить немаскируемые прерывания
	;in al, 0x70
	;or al, 0x80
	;out 0x70, al

	;Переключиться в защищенный режим
	mov eax, cr0
	or al, 1
	mov  cr0, eax

	;Загрузить в CS:EIP точку входа в защищенный режим
	;3 бита справа не относятся к номеру дескриптора
	jmp 00001000b:pm_entry

;32-битная адресация
use32
;Точка входа в защищенный режим
pm_entry:
	;Загрузить сегментные регистры (кроме SS)
	mov ax, cs
	mov ds, ax
	mov es, ax

;***************************** 
; Turn floppy off. 
	mov dx, 3F2h
	xor al, al
	out dx, al
;***************************** 

	;call s_clear

	;Загрузка данных с диска
	;mov edi, msg	;Адрес буфера
	;mov ch, 9	;Номер начального сектора
	;mov cl, 1	;Количество читаемых секторов
	;xor bx, bx
	;call d_read

	;ESI = 3 * X, EDI = Y, EBX = COLOR
	mov esi, 30
	mov edi, 10
	mov ebx, 0xFF0000
	call s_drawChar

	mov esi, 90
	mov edi, 10
	mov ebx, 0x00FF00
	call s_drawChar

	mov esi, 150
	mov edi, 10
	mov ebx, 0x0000FF
	call s_drawChar

	call k_pause

	mov esi, 30
	mov edi, 30
	mov ebx, 0x00FF00
	call s_drawChar
	
	;mov edi, VIDEO_MEMORY		;Начало видеопамяти в видеорежиме 0x3
	;mov esi, msg		;Выводимое сообщение
	;cld
;Цикл вывода сообщения
;.msg_loop:
	;lodsb			;Считываем очередной символ строки
	;test al, al		;Если встретили 0
	;jz .exit			;Прекращаем вывод
	;stosb			;Иначе выводим очередной символ
	;mov al, 2		;И его атрибут в видеопамять
	;stosb
	;jmp .msg_loop
	
.exit:
	;mov     ax, 0FEh        ; команда отключения
	;out     64h, ax
	
	;.exit_loop:
	;jmp .exit_loop
	hlt

include 'screen.inc'
include 'keyboard.inc'
include 'drive.inc'

;*******************ДАННЫЕ****************************
msg: db 512 dup(0)
zero db '0'
space db ' '

;********Глобальная таблица дескрипторов.**********
;Нулевой дескриптор использовать нельзя!
;word(предел сегмента)
;word(младшее слово адреса)
;byte(старший байт адреса, 3-ий байт)
;byte(доступ)
;word(резерв для i80286. Должно быть равно 0) ИЛИ byte(старшая часть предела и флаги GDXU), byte(4-ый байт адреса)
gdt:
	gdt_null:
		dw 0x0000, 0x0000
		db 0x00, 0x00
		dw 0x0000
	gdt_code:
		dw 0xFFFF, 0x0000
		db 0x00, 10011010b
		db 11001111b, 0x00
gdt_size  equ $ - gdt
     
;данные, загружаемые в регистр GDTR
gdtr:
dw gdt_size - 1
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