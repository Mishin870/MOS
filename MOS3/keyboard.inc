;k_enableMouse:
;	mov dx, 0x64
;	mov al, 0xa8
;	out dx, al
;ret

;k_disableMouse:
;	mov dx, 0x64
;	mov al, 0xa7
;	out dx, al
;ret

;1 слева бит: 1 - отпущена, 0 - нажата
;k_pause:	
;	;mov al, 0x20
;	;mov dx, 0x64
;	;out dx, al
;	
;	;mov cl, byte[prev]
;	.key_loop:
;		in al, 0x60
;		cmp al, 0x9C
;		;cmp al, cl
;		;je .key_loop
;		;mov cl, al
;		;and al, 10000000b
;		;test al, al
;	jne .key_loop
;	;mov byte[prev], cl
;ret

;prev db 0xFF

;k_getch:
;	in al, 0x60
;ret

KEY_ENTER = 28		;0x1c
KEY_BACKSPACE = 14	;0x0e
k_remap db 90 dup(0)

k_initKeys:
	;A-Z
	mov [k_remap + 33], 0
	mov [k_remap + 51], 1
	mov [k_remap + 32], 2
	mov [k_remap + 22], 3
	mov [k_remap + 38], 4
	mov [k_remap + 20], 5
	mov [k_remap + 41], 6
	mov [k_remap + 39], 7
	mov [k_remap + 25], 8
	mov [k_remap + 48], 9
	mov [k_remap + 16], 10
	mov [k_remap + 19], 11
	mov [k_remap + 37], 12
	mov [k_remap + 47], 13
	mov [k_remap + 21], 14
	mov [k_remap + 36], 15
	mov [k_remap + 34], 16
	mov [k_remap + 35], 17
	mov [k_remap + 46], 18
	mov [k_remap + 49], 19
	mov [k_remap + 18], 20
	mov [k_remap + 30], 21
	mov [k_remap + 26], 22
	mov [k_remap + 17], 23
	mov [k_remap + 45], 24
	mov [k_remap + 23], 25
	mov [k_remap + 24], 26
	mov [k_remap + 27], 27
	mov [k_remap + 31], 28
	mov [k_remap + 50], 29
	mov [k_remap + 40], 30
	mov [k_remap + 52], 31
	mov [k_remap + 44], 32
	;0-9 main
	mov [k_remap + 11], 33
	mov [k_remap + 02], 34
	mov [k_remap + 03], 35
	mov [k_remap + 04], 36
	mov [k_remap + 05], 37
	mov [k_remap + 06], 38
	mov [k_remap + 07], 39
	mov [k_remap + 08], 40
	mov [k_remap + 09], 41
	mov [k_remap + 10], 42
	;0-9 numpad
	mov [k_remap + 82], 33
	mov [k_remap + 79], 34
	mov [k_remap + 80], 35
	mov [k_remap + 81], 36
	mov [k_remap + 75], 37
	mov [k_remap + 76], 38
	mov [k_remap + 77], 39
	mov [k_remap + 71], 40
	mov [k_remap + 72], 41
	mov [k_remap + 73], 42
	;Symbols
	mov [k_remap + 57], 43
	mov [k_remap + 78], 44
	mov [k_remap + 74], 45
	mov [k_remap + 55], 46
	mov [k_remap + 53], 47
	mov [k_remap + 13], 48
	mov [k_remap + 12], 45
ret

;AX = KEYBOARD CODE
;RETURN AX = FONT CODE
k_getFontCode:
	;cmp al, 0
	;jl .err
	cmp al, 89
	jg .err
	push si
	xor ah, ah
	mov si, ax
	mov al, byte[k_remap + si]
	pop si
ret
	.err:
	mov ax, 0
ret