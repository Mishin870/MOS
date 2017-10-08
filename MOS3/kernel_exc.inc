exc0:
pushad
mov ax, 0
jmp showErr

exc1:
pushad
mov ax, 1
jmp showErr

exc2:
pushad
mov ax, 2
jmp showErr

exc3:
pushad
mov ax, 3
jmp showErr

exc4:
pushad
mov ax, 4
jmp showErr

exc5:
pushad
mov ax, 5
jmp showErr

exc6:
pushad
mov ax, 6
jmp showErr

exc7:
pushad
mov ax, 7
jmp showErr

exc8:
pushad
mov ax, 8
jmp showErr

exc9:
pushad
mov ax, 9
jmp showErr

exc10:
pushad
mov ax, 10
jmp showErr

exc11:
pushad
mov ax, 11
jmp showErr

exc12:
pushad
mov ax, 12
jmp showErr

exc13:
pushad
pop ecx
mov ax, 13
jmp showErr

exc14:
pushad
mov ax, 14
jmp showErr

exc15:
pushad
mov ax, 15
jmp showErr

exc16:
pushad
mov ax, 16
jmp showErr

exc17:
pushad
mov ax, 17
jmp showErr

exc18:
pushad
mov ax, 18
jmp showErr

exc19:
pushad
mov ax, 19
jmp showErr

exc20:
pushad
mov ax, 20
jmp showErr

exc21:
pushad
mov ax, 21
jmp showErr

exc22:
pushad
mov ax, 22
jmp showErr

exc23:
pushad
mov ax, 23
jmp showErr

exc24:
pushad
mov ax, 24
jmp showErr

exc25:
pushad
mov ax, 25
jmp showErr

exc26:
pushad
mov ax, 26
jmp showErr

exc27:
pushad
mov ax, 27
jmp showErr

exc28:
pushad
mov ax, 28
jmp showErr

exc29:
pushad
mov ax, 29
jmp showErr

exc30:
pushad
mov ax, 30
jmp showErr

exc31:
pushad
mov ax, 31
jmp showErr

showErr:
	mov bx, 10

	xor dx, dx
	div bx
	add dl, 33
	mov byte[werror + 7], dl

	xor dx, dx
	div bx
	add dl, 33
	mov byte[werror + 6], dl

	mov ax, cx
	xor dx, dx
	div bx
	add dl, 33
	mov byte[werror + 11], dl
	
	xor dx, dx
	div bx
	add dl, 33
	mov byte[werror + 10], dl

	xor dx, dx
	div bx
	add dl, 33
	mov byte[werror + 9], dl

	xor dx, dx
	div bx
	add dl, 33
	mov byte[werror + 8], dl
	
	mov esi, 500 * S_BBP

	mov edi, dword[currY]
	add edi, 20
	mov dword[currY], edi

	mov eax, 150
	mov edx, 100
	mov ebx, werror
	mov ecx,  12
	call s_drawWindow
	popad
iret