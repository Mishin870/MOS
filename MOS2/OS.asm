macro align value { db value-1 - ($ + value-1) mod (value) dup 0 }
HEADS = 1
SPT = 18 ;4 сектора по 512 байт
Begin:
	file "boot.bin", 512
	file "kernel.bin"
	align 8192	;16 секторов
	align HEADS*SPT*512