macro align value { db value-1 - ($ + value-1) mod (value) dup 0 }
HEADS = 1
SPT = 12 ;4 сектора по 512 байт
Begin:
	file "boot.bin", 512
	file "kernel.bin"
	align 4096
	file "data.txt"
	align 512
	align HEADS*SPT*512