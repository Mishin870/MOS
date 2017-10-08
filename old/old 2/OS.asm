macro align value { db value-1 - ($ + value-1) mod (value) dup 0 }
HEADS = 1
SPT = 16 ;4 сектора по 512 байт
Begin:
	file "boot.bin", 512		;Загрузчик
	file "kernel.bin", 2048	;Первый файл, типа оболочка shell
	file "writer.bin", 1024	;Второй файл - текстовый редатор
	align HEADS*SPT*512