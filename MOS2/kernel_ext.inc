ext0:
;pushad
;mov ax, 0
;jmp showExt
push ax
push dx
outb 0x20, 0x20
pop dx
pop ax
iret

ext1:
pushad
in al, 0x60
xor ah, ah
mov bl, al
and bl, 10000000b
jnz .kRel
;mov ax, 1
jmp showText
.kRel:
outb 0x20, 0x20
popad
iret

showText:
	cmp al, KEY_ENTER
	je .showWin
	
	call k_getFontCode
	mov si, word[wmsglen]
	cmp si, 19
	jg .endWin
	mov byte[wmsg + si], al
	inc si
	mov word[wmsglen], si
	jmp .endWin
	
	.showWin:
	mov esi, 250 * S_BBP

	mov edi, dword[currEY]
	add edi, 20
	mov dword[currEY], edi

	mov eax, 150
	mov edx, 100
	mov ebx, wmsg
	movzx ecx,  word[wmsglen]
	mov word[wmsglen], 0
	call s_drawWindow
	.endWin:
	
	outb 0x20, 0x20
	
	popad
iret

ext2:
pushad
mov ax, 2
jmp showExt

ext3:
pushad
mov ax, 3
jmp showExt

ext4:
pushad
mov ax, 4
jmp showExt

ext5:
pushad
mov ax, 5
jmp showExt

ext6:
pushad
mov ax, 6
jmp showExt

ext7:
pushad
mov ax, 7
jmp showExt

ext8:
pushad
mov ax, 8
jmp showExt2

ext9:
pushad
mov ax, 9
jmp showExt2

ext10:
pushad
mov ax, 10
jmp showExt2

ext11:
pushad
mov ax, 11
jmp showExt2

ext12:
pushad
mov ax, 12
jmp showExt2

ext13:
pushad
mov ax, 13
jmp showExt2

ext14:
pushad
mov ax, 14
jmp showExt2

ext15:
pushad
mov ax, 15
jmp showExt2