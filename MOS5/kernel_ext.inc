;===========МОДУЛЬ СЛУШАТЕЛЕЙ ВНЕШНИХ ПРЕРЫВАНИЙ========

;это прерывание внутреннего таймера
;тикает через какой-то интервал
;здесь была попытка заложить основы многопоточности
;но заброшено потому, что не получилось настроить таблиу LDT
ext0:
	;pushad
	;mov ax, 0
	;jmp showExt
	;Здесь получаем текущий поток и врубаем его
	mov al, byte[countThreads]
	test al, al
	jz .endThreading
	;Конец
	
	.endThreading:
	push ax
	push dx
	outb 0x20, 0x20
	pop dx
	pop ax
iret

;предполагается, что это прерывание клавиатуры ps/2
ext1:
	pushad
	in al, 0x60
	xor ah, ah
	mov bl, al
	and bl, 10000000b
	jnz .kRel
	;mov ax, 1
	jmp showText
	.kRel:
	outb 0x20, 0x20
	popad
iret

;да, очень странное расположение данной функции
;эта функция обрабатывает текст, который был введен посредством клавиатурных прерываний
;(после нажатия на enter)
;плюс бонус - проверка команд
showText:
	mov si, word[wmsglen]

	cmp al, KEY_ENTER
	je .showWin

	cmp al, KEY_BACKSPACE
	je .del
	
	call k_getFontCode
	cmp si, 19
	jg .endWin
	mov byte[wmsg + si], al
	inc si
	mov word[wmsglen], si

	;EAX = FONT CHAR OFFSET+POINTER, ESI = S_BBP * X, EDI = Y, EBX = COLOR, EDX = FONT MAIN WIDTH
	push eax
	push esi
	push edi
	push ebx
	push edx

	push eax
	mov eax, esi
	mov ecx, 9 * S_BBP
	mul ecx
	xor esi, esi
	add esi, eax
	pop eax

	shl eax, 3
	add eax, mfont
	mov edx, FONT_WID
	mov edi, 50
	mov ebx, 0x00FF0000
	call s_drawChar8
	pop edx
	pop ebx
	pop edi
	pop esi
	pop eax
	jmp .endWin
	
	;Удаление одного символа
	.del:
		test si, si
		jz .endWin
		
		dec si
		mov word[wmsglen], si
		
		push esi
		push edi
		push ebx
		push eax
		mov edi, 50
		
		;Ширина и высота 1 символа
		mov eax, 9
		push eax
		mov eax, 8
		push eax
		
		mov eax, 9 * S_BBP
		mov bx, si
		inc bx
		mul bx
		mov esi, eax
		mov ebx, 0x00829c82
		call s_drawRect
		pop eax
		pop ebx
		pop edi
		pop esi
	jmp .endWin
	
	;Показ самого окна
	.showWin:
	test si, si
	jz .endWin
	
	mov al, byte[wmsg]
	cmp al, '/'
	jne .endCmd
		;Односимвольные команды!
		cmp si, 2
		jne .endCmd

		mov al, byte[wmsg + 1]
		
		;Рандом
		.cmdRnd:
		cmp al, 17
		jne .cmdClr
		mov ebx, 0
		mov ecx, 9
		call r_randomFT
		add al, 33
		mov byte[wmsg], al
		mov word[wmsglen], 1
		
		;Очистка
		.cmdClr:
		cmp al, 11
		jne .endCmd
		mov edi, 20
		mov dword[currEY], edi
	.endCmd:
	;ESI = S_BBP * X, EDI = Y, EBX = COLOR, STACK = "push width, push height"
	push esi
	push edi
	push ebx
	push eax
	mov edi, 50

	mov eax, esi
	mov bx, 9
	mul bx
	push eax
	mov eax, 8
	push eax

	mov esi, 8 * S_BBP
	mov ebx, 0x00829c82
	call s_drawRect
	pop eax
	pop ebx
	pop edi
	pop esi
	
	mov esi, 250 * S_BBP

	mov edi, dword[currEY]
	add edi, 20
	mov dword[currEY], edi

	mov eax, 150
	mov edx, 100
	mov ebx, wmsg
	movzx ecx,  word[wmsglen]
	mov word[wmsglen], 0
	call s_drawWindow
	.endWin:
	
	outb 0x20, 0x20
	
	popad
iret

ext2:
pushad
mov ax, 2
jmp showExt

ext3:
pushad
mov ax, 3
jmp showExt

ext4:
pushad
mov ax, 4
jmp showExt

ext5:
pushad
mov ax, 5
jmp showExt

ext6:
pushad
mov ax, 6
jmp showExt

ext7:
pushad
mov ax, 7
jmp showExt

ext8:
pushad
mov ax, 8
jmp showExt2

ext9:
pushad
mov ax, 9
jmp showExt2

ext10:
pushad
mov ax, 10
jmp showExt2

ext11:
pushad
mov ax, 11
jmp showExt2

ext12:
pushad
mov ax, 12
jmp showExt2

ext13:
pushad
mov ax, 13
jmp showExt2

ext14:
pushad
mov ax, 14
jmp showExt2

ext15:
pushad
mov ax, 15
jmp showExt2

;Функция, которая отрисовывает дебаговое окно с заголовком, содержащим код прерывания
;*а кто-то до сих пор дебажит веб-страницы алертами*
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

;не могу вспомнить, для чего это было
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