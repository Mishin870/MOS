;============МОДУЛЬ РАБОТЫ С ЖЕСТКИМ ДИСКОМ=========
;один из самых напряжных модулей. работает не везде и не всегда

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

;=============================================================================
; ATA read sectors (LBA mode) 
;
; @param EAX Logical Block Address of sector
; @param CL  Number of sectors to read
; @param EDI The address of buffer to put data obtained from disk
;
; @return None
;=============================================================================
d_read:
	and eax, 0x0FFFFFFF

	mov ebx, eax	;Save LBA in RBX
 
	mov edx, 0x01F6	;Port to send drive and bit 24 - 27 of LBA
	shr eax, 24	;Get bit 24 - 27 in al
	or al, 11100000b	;Set bit 6 in al for LBA mode
	out dx, al

	mov edx, 0x01F2	;Port to send number of sectors
	mov al, cl	;Get number of sectors from CL
	out dx, al

	mov edx, 0x1F3	;Port to send bit 0 - 7 of LBA
	mov eax, ebx	;Get LBA from EBX
	out dx, al

	mov edx, 0x1F4	;Port to send bit 8 - 15 of LBA
	mov eax, ebx	;Get LBA from EBX
	shr eax, 8	;Get bit 8 - 15 in AL
	out dx, al

	mov edx, 0x1F5	;Port to send bit 16 - 23 of LBA
	mov eax, ebx	;Get LBA from EBX
	shr eax, 16	;Get bit 16 - 23 in AL
	out dx, al

	mov edx, 0x1F7	;Command port
	mov al, 0x20	;Read with retry.
	out dx, al

	.still_going:
		in al, dx
		test al, 8		;the sector buffer requires servicing.
	jz .still_going	;until the sector buffer is ready.

	mov eax, 256	;to read 256 words = 1 sector
	xor bx, bx
	mov bl, cl	;read CL sectors
	mul bx
	mov ecx, eax	;RCX is counter for INSW
	mov edx, 0x1F0	;Data port, in and out
	rep insw		;in to [RDI]
ret