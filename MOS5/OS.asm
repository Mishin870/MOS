macro align value { db value-1 - ($ + value-1) mod (value) dup 0 }
HEADS = 1
;35 сектора по 512 байт
SPT = 35
Begin:
	file "boot.bin", 512
	file "kernel24.bin"
	align 8192	;16 секторов
	file "kernel32.bin"
	align 8192	;16 секторов
	file "TestProgram/test.bin"
	align 512		;1 сектор
	align HEADS * SPT * 512