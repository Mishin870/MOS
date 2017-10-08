;macro align value { db value-1 - ($ + value-1) mod (value) dup 0 }
;Очистить экран и установить 2 режим
;macro clrscr {
;	mov ax, 0002h
;	int 10h
;}