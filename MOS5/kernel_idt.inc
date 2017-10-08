;инфо: http://wiki.osdev.org/Interrupt_Descriptor_Table

;макрос для Flat ассемблера. вызывается как "outb число1, число2"
;оно посылает значение val на порт pnum для указания действия какому-либо устройству
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

;проверка возможности отрисовать окно на экране из под функции, запущенной прерыванием
;(её использование в конце таблицы IDT, внизу этого файла)
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

;таблица прерываний-исключений
include 'kernel_exc.inc'
;таблица внешних прерываний (устройств ввода)
include 'kernel_ext.inc'

;================IDT====================
;константы различных заготовленных режимов
;см информацию по ссылке сверху
ACC_PRESENT = 10000000b
ACC_INT_GATE = 00001110b
ACC_TRAP_GATE = 00001111b
RING3_ACCESS = 01100000b

;таблица начинается с метки idt:, адрес которой будет подан процессору
;для скармливания этой таблицы. создается на основе макроса
;первая строка макроса - костыль для flat asm'а
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
	
;все исключения и внешние прерывания кончились
;попытка добавить в IDT свою собственную функцию как прерывание
;(спойлер: получилось. но это разрывает все принципы колец защиты процессора)

	
	test_func:
	dw ((test1 shl 0x30) shr 0x30)					;Низшая часть адреса
	dw 0x8								;Селектор
	db 0, ACC_PRESENT OR ACC_INT_GATE OR RING3_ACCESS		;Тип (доступ)
	dw (test1 shr 0x10)							;Высшая часть адреса
idt_end:
;Длина IDT - 1
idtr: dw 187h
dd idt