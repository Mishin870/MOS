.CmdHelp:
	mov di, string
	mov si, help
	mov cx, 4
	rep cmpsb
	jne .CmdHello
		
	call NewLineSimple
	mov bp, help1
	mov cx, 26
	call PrintMes

	call NewLineSimple
	mov bp, help2
	mov cx, 21
	call PrintMes

	call NewLineSimple
	mov bp, help3
	mov cx, 17
	call PrintMes

	call NewLineSimple
	mov bp, help4
	mov cx, 22
	call PrintMes
		
	call NewLineSimple
	mov bp, help5
	mov cx, 18
	call PrintMes

	call NewLineSimple
	mov bp, help6
	mov cx, 18
	call PrintMes

	call NewLineSimple
	mov bp, help7
	mov cx, 20
	call PrintMes

	call NewLineSimple
	mov bp, help8
	mov cx, 16
	call PrintMes
		
	jmp .PostCmd
.CmdHello:
	mov di, string
	mov si, hello
	mov cx, 5
	rep cmpsb
	jne .CmdEnd

	call NewLineSimple
	mov bp, hello_world
	mov cx, 13
	call PrintMes
	jmp .PostCmd
.CmdEnd:
	mov di, string
	mov si, endcmd
	mov cx, 3
	rep cmpsb
	jne .CmdWriter

	mov ah, 53h              ;this is an APM command
	mov al, 07h              ;Set the power state...
	mov bx, 0001h            ;...on all devices to...
	;01h = Standby
	;02h = Suspend
	;03h = Off
	mov cx, 03h    ;see above
	int 15h                 ;call the BIOS function through interrupt 15h
	jmp .PostCmd
.CmdWriter:
	mov di, string
	mov si, write
	mov cx, 5
	rep cmpsb ;сравниваем строки - если не команда write, то переходим далее
	jne .EchoCmd
	xor ax, ax
	mov es, ax
	mov bx, 1300h	    
	mov ch, 0	    ;номер цилиндра - 0
	mov cl, 05h	    ;начальный сектор - 5
	mov dh, 0	    ;номер головки - 0
	mov dl, 80h	    ;жесткий диск - 80h
	mov al, 02h	    ;кол-во читаемых секторов -1
	mov ah, 02h
	int 13h
	jmp 0000:1300h
.EchoCmd:
	mov di, string
	mov si, echo
	mov cx, 4
	rep cmpsb
	jne .RebootCmd
	
	call NewLineSimple
	pop si
	push si
	cmp si, 5
	jl .PostCmd
	mov cx, si
	sub cx, 5		;Длина 'echo '
	mov bp, string
	add bp, 5
	call PrintMes

	jmp .PostCmd
.RebootCmd:
	mov di, string
	mov si, reboot
	mov cx, 6
	rep cmpsb
	jne .ClrCmd

	mov ax, 0040h
	mov ds, ax
	mov WORD [0072h], 1234h
	jmp 0F000h:0FFF0h
.ClrCmd:
	mov di, string
	mov si, clr
	mov cx, 3
	rep cmpsb
	jne .ColorCmd

	mov ax, 0002h
	int 10h
	xor dx, dx
	jmp .PostCmd
.ColorCmd:
	mov di, string
	mov si, color_cmd
	mov cx, 5
	rep cmpsb
	jne .DateCmd
	
	call NewLineSimple
	pop si
	push si
	cmp si, 8
	jl .PostCmd
	xor ax, ax
	xor bx, bx
	mov al, byte[string+si-1]
	mov bl, byte[string+si]
	sub al, byte[null]	;вычитаем символ нуля
	sub bl, byte[null]
	mov dl, 10
	mul dl
	add al, bl
	mov byte[color], al

	xor dl, dl
	call SetCursorPos
	mov cx, 14
	mov bp, color_succ
	call PrintMes

	jmp .PostCmd
.DateCmd:
	mov di, string
	mov si, date_cmd
	mov cx, 4
	rep cmpsb
	jne .ErrCmd

	call NewLineSimple
	mov bp, date_is
	mov cx, 9
	call PrintMes

	mov cx, 8
	.cloop:
		mov si, cx
		cli
		mov al, cl
		out 0x70, al
		in al, 0x71
		sti
		mov byte[date+si], al
	loop .cloop

	call NewLineSimple
	mov bp, date
	mov cx, 8
	call PrintMes

	jmp .PostCmd