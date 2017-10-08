NEWLLEN = 5

org 500h		;этот сектор будет загружаться по адресу 0000:0500h

message:
	mov ax, 0002h   ;очищаем экран
	int 10h
 
	xor dx, dx	;Курсор в 0
	call SetCursorPos
	
	mov bp, title
	mov cx, 24
	call PrintMes

	add dh, 1h
	call SetCursorPos
	
	mov bp, msg
	mov cx, 21
	call PrintMes
	 
	add dh, 1h
	call SetCursorPos
	
	call NewLine

	xor si, si
	 
MainLoop:
	mov ah, 10h
	int 16h
	cmp ah, 0Eh	;BackSpase
	jz Delete_symbol
	cmp al, 0Dh	;Enter
	jz Input_Command
	mov [string + si], al
	inc si
	mov ah, 09h
	mov bl, [color]
	mov cx, 1
	int 10h
	add dl, 1
	call SetCursorPos
	jmp MainLoop
	 
Input_Command:	    ;Если нажат Enter, то переходим в третий сектор
	mov ax, cs
	mov ds, ax
	mov es, ax
	push si	;так как содержание регистра si меняется, сохраним в стеке

	include 'kernelCmds.asm'	

	.ErrCmd:
		call NewLineSimple
		mov bp, cmderr
		mov cx, 18
		call PrintMes
	.PostCmd:

	pop si
	call NewLine
	jmp MainLoop
	 
Delete_symbol:
	cmp dl, NEWLLEN
	jz MainLoop
	sub dl, 1		;Двигаем каретку влево
	call SetCursorPos
	mov al, 20h		;Пробел
	mov [string + si], al		;Стираем символ
	mov ah, 09h
	mov bx, 000fh	;Цвет курсора!
	mov cx, 1
	int 10h
	dec si		;уменьшаем кол-во напечатанных символов
	jmp MainLoop
 
SetCursorPos:	     ;установка курсора
	mov ah, 2h
	xor bh, bh
	int 10h 
	ret

PrintMes:				;в регистре  bp - строка, в регистре cx - длина этой строки
	mov bl, [color]	    ;в регистре  bl - цвет
	mov ax, 1301h	    ;функция 13h прерывания 10h
	int 10h
	ret

NewLine:
	xor dl, dl
	add dh, 1
	call SetCursorPos
	mov bp, newl
	mov cx, NEWLLEN
	call PrintMes
	mov dl, NEWLLEN
	call SetCursorPos
	xor si, si
	ret
NewLineSimple:
	xor dl, dl
	add dh, 1
	call SetCursorPos
	ret

;=======КОМАНДЫ=======
endcmd db 'end', 0
write db 'write', 0
help db 'help', 0
echo db 'echo', 0
reboot db 'reboot', 0
clr db 'clr', 0
color_cmd db 'color', 0
hello db 'hello', 0
date_cmd db 'date', 0
;=====================
cmderr db 'Command not found!', 0 ;18
color_succ db 'Color changed!', 0 ;14
help1 db 'writer - start text writer', 0 ;26
help2 db 'help - show this help', 0 ;21
help3 db 'end - shutdown OS', 0 ;17
help4 db 'echo %txt - print text', 0 ;22
help5 db 'reboot - reboot OS', 0 ;18
help6 db 'clr - clear screen', 0 ;18
help7 db 'color %c - set color', 0 ;20
help8 db 'date - show date', 0 ;16
hello_world db 'Hello, world!', 0 ;13
title db 'Mishin870 OS Kernel v1.0', 0 ;24
msg db 'For help press "help"', 0 ;21
newl db 'MOS> ', 0 ;5
string db 20 dup(?)		;буфер для ввода команды
color db 2h		;цвет вывода
null db '0'
date_is db 'Date is: ', 0 ;9
date db 8 dup(?)		;буфер даты

times(2048-($-500h)) db 0