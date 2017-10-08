macro align value { db value-1 - ($ + value-1) mod (value) dup 0 }
HEADS = 1
;34 сектора по 512 байт
SPT = 34
Begin:
	file "boot.bin", 512
	file "kernel24.bin"
	align 8192	;16 секторов
	file "kernel32.bin"
	align 8192	;16 секторов
	align HEADS*SPT*512