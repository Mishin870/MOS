COLOR = 02h

org 7c00h

start:
	cli		;запрещаем прерывания
	xor ax,ax	 ;обнуляем регистр ах
	mov ds,ax	 ;настраиваем сегмент данных на нулевой адрес
	mov es,ax	 ;настраиваем сегмент es на нулевой адрес
	mov ss,ax	 ;настраиваем сегмент стека на нулевой адрес
	mov sp, 07C00h	;сегмент sp указывает на текущую вершину стека
	sti	    ;разрешаем прерывания
	
	;Переходим во 2 видеорежим и очищаем экран    
	mov ax, 0002h
	int 10h
	 
	xor dx, dx ;курсор в 0
	call SetCursorPos

	mov bp, msg		
	mov cx, 13
	call PrintMes
	 
	add dh, 1		;переходим на одну строку ниже
	call SetCursorPos
	
	mov bp, press		;Вывод на экран строки Con
	mov cx, 23		;Длина строки
	call PrintMes

ContinueLoop:		 
	mov ah, 10h
	int 16h
	cmp al, 0Dh	;Если нажимаем на Enter, то переходим к загрузке ядра
	jz StartKernel
	jmp ContinueLoop	;Если нет, снова ожидаем нажатия клавиши
		 
StartKernel:
	mov ax, 0000h
	mov es, ax
	mov bx, 500h
	mov ch, 0h      ;номер цилиндра - 0
	mov cl, 02h	    ;начальный сектор - 2
	mov dh, 0h	    ;номер головки - 0
	mov dl, 80h	    ;жесткий диск - 80h
	mov al, 04h	    ;кол-во читаемых секторов -1
	mov ah, 02h
	int 13h
	jmp 0000:0500h	    ;переход на 0000:0500h, куда загружен второй сектор
 
;===================== Подпрограммы ===================================
PrintMes:		    ;в регистре  bp - строка, в регистре cx - длина этой строки
	mov bl, COLOR	    ;в регистре  bl- атрибут
	mov ax, 1301h	    ;функция 13h прерывания 10h
	int 10h
	ret
;----------------------------------
SetCursorPos:	     ;установка курсора : функция 02h прерывания 10h
	mov ah, 2h
	xor bh, bh
	int 10h 
	ret
	    
;===================== выводимые сообщения===================== 
	msg db 'OS Loading...', 0     
	press db 'Press Enter to Continue', 0
  
times(512-2-($-07C00h)) db 0
db 055h, 0AAh	 ;сигнатура, символизирующая о завершении загрузочного сектора