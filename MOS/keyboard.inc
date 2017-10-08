;2 слева бит: 1 - отпущена, 0 - нажата
k_pause:
	.key_loop:
		in al, 0x60
		and al, 01000000b
		test al, al
	jnz .key_loop
	xor al, al
	out 0x60, al
ret