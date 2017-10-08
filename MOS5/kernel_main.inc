;Загрузка таблицы прерываний и запуск прерываний
lidt [idtr]
sti

;Загрузка информации о VESA (интерфейс для базовых граф адаптеров) из указателя
pop esi
mov edi, Mode_Info
mov ecx, dword[Mode_Info_len]
rep movsb

;инит модуля рандома
call r_init
;инит модуля клавиатуры (ремаппинг клавиш)
call k_initKeys

;Заливка цветом (P.S. градиентом) (P.P.S. не работает как надо, но даже лучше :) )
;P.P.P.S. возможно, что все комментарии на строке выше устарели..
mov eax, 0x00829c82
mov esi, 0x00FFFFFF
call s_clear

;установка слушателей всех прерываний
call set_int_ctrlr

;Тест отрисовки шрифта
;в левом-верхнем углу экрана появится строка со всеми символами шрифта
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

;рисуем одно простейшее окно
;ESI = 4 * X, EDI = Y, EAX = WIDTH, EDX = HEIGHT
mov esi, 10 * S_BBP
mov edi, 100
mov eax, 150
mov edx, 100
mov ebx, wtitle
mov ecx,  4
call s_drawWindow

;int 30h