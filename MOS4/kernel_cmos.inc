;outb (0x70, (NMI_disable_bit << 7) | (selected CMOS register number));
;val_8bit = inb (0x71);

;Читает из CMOS памяти. AL - регистр cmos
cmos_read:
	push dx
	mov dx, 0x70
	out dx, al
	mov dx, 0x71
	in al, dx
	pop dx
ret