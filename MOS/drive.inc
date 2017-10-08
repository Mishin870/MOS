;Порт	Название			Использование
;============================================================================
;0x1f0	Data Port				Read/Write PIO data bytes on this port.
;0x1f1	Features / Error Information		Usually used for ATAPI devices.
;0x1f2	Sector Count			Количество секторов на чтение/запись (0 - спец. значение)
;0x1f3	Sector Number / LBAlo		This is CHS / LBA28 / LBA48 specific.
;0x1f4	Cylinder Low / LBAmid		Partial Disk Sector address.
;0x1f5	Cylinder High / LBAhi		Partial Disk Sector address.
;0x1f6	Drive / Head Port			Used to select a drive and/or head. May supports extra address/flag bits.
;0x1f7	Command port / Regular Status port	Используется для отправки команд или чтения текущего статуса

;Статус	Назв.	Описание
;==================================================================================
;0 	ERR 	Indicates an error occurred. Send a new command to clear it (or nuke it with a Software Reset).
;3 	DRQ 	Set when the drive has PIO data to transfer, or is ready to accept PIO data.
;4 	SRV 	Overlapped Mode Service Request.
;5 	DF 	Drive Fault Error (does not set ERR).
;6 	RDY 	Bit is clear when drive is spun down, or after an error. Set otherwise.
;7 	BSY 	Indicates the drive is preparing to send/receive data (wait for it to clear). In case of 'hang' (it never clears), do a software reset. 

;Считать сектор с диска в память
;@param EDI Буффер чтения
;@param CH Номер сектора
;@param CL Количество секторов для чтения
;@param BH Остаток цилиндра
;@param BL Номер цилиндра
d_read:
	xor eax, eax

	mov dx, 0x1f6	;Порт выбора диска и головки  
	mov al, 0xa0	;Диск 0, головка 0  
	out dx, al

	mov dx, 0x1f7
	in al, dx
	mov [edi], al
	
	mov ecx, 20
	mov ebx, 1
	.testLoop:
		inc al
		mov [edi + ebx], al
		inc ebx
	loop .testLoop

	mov al, [dog]
	mov ebx, 21
	mov [edi + ebx], al	

	in al, dx
	add al, [null]
	mov ebx, 22
	mov [edi + ebx], al

	ret
  
	mov dx, 0x1f2	;Порт счетчика секторов  
	mov al, cl	;Прочитать один сектор (fixed: cl секторов)
	out dx, al  

	mov dx, 0x1f3	;Порт номера сектора  
	mov al, ch	;Прочитать сектор #1  (fixed: ch сектор)
	out dx, al

	mov dx, 0x1f4	;Порт младшего значения циллиндра
	mov al, bl	;Циллиндр 0  (0 -> bl)
	out dx, al

	mov dx, 0x1f5	;Порт старшего значения циллиндра  
	mov al, bh	;Остаток циллиндра 0 The rest of the cylinder 0   (0 -> bh)
	out dx, al

	mov dx, 0x1f7	;Коммандный порт
	mov al, 0x20	;Прочитать с попыткой повтора
	out dx, al

	.still_going:  
		in al, dx  
		test al, 8		;Это значит, что буффер сектора требует его обслужить
	jz .still_going	;Не продолжать до тех пор, пока буффер сектора не будет готов.

	mov cx, 512 / 2	;Один сектор / 2
	;mov edi, buffer  
	mov dx, 0x1f0	;Порт данных - они приплывают сюда и уплывают   
	rep insw		;отсюда
	.error:
ret

dog db '@'
null db '0'