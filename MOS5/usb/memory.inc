;Allocator for fixed-size blocks: allocate a block.
;[ebx - 4] = pointer to the first page, ebx = pointer to MUTEX structure.
proc usb_allocate_common
	push edi ;Save refister to stdcall
	virtual at esp
		dd ? ;Saved edi
		dd ? ;Return address
		.size dd ?
	end virtual

	mov ecx, ebx
	call mutex_lock

	mov edx, ebx
	.pageloop:
		mov eax, edx
		mov edx, [edx - 4]

		test edx, edx
		jz .newpage
		add edx, 0x1000
	@@:
		mov eax, [edx - 8]
		test eax, eax
		jz .pageloop

		mov ecx, [eax]

		lock cmpxchg [edx-8], ecx
		jnz @b
	.return:
		push eax
		mov ecx, ebx
		call mutex_unlock
		pop eax
		pop edi ;Restore used register to be stdcall
		ret 4
	.newpage:
		push eax
		stdcall kernel_alloc, 0x1000
		pop edx

		test eax, eax
		jz .nomemory

		mov [edx - 4], eax

		mov edx, eax
		lea edi, [eax + 0x1000 - 8]
		add edx, [.size]
		mov [edi], edx
		and dword[edi + 4], 0
	@@:
		mov ecx, edx
		add edx, [.size]
		mov [ecx], edx
		cmp edx, edi
		jbe @b
		sub ecx, [.size]
		and dword[ecx], 0

		jmp .return
	.nomemory:
		dbgstr 'no memory for USB descriptor'
		xor eax, eax
		jmp .return
endp

; Allocator for fixed - size blocks: free a block.
proc usb_free_common
	push ecx edx
	virtual at esp
		rd 2 ;Saved registers
		dd ? ;Return address
		.block dd ?
	end virtual

	mov ecx, [.block]
	mov edx, ecx
	or edx, 0xFFF
	@@:
		mov eax, [edx + 1 - 8]
		mov [ecx], eax
		lock cmpxchg [edx + 1 - 8], ecx
		jnz @b
		pop edx ecx
		ret 4
endp

; Helper procedure: translate physical address in ecx
; of some transfer descriptor to linear address.
; in: eax = address of first page
proc usb_td_to_virt
	; Traverse all pages used for transfer descriptors, looking for the one
	; with physical address as in ecx.
	@@:
		test eax, eax
		jz .zero
		push eax
		call get_pg_addr
		sub eax, ecx
		jz .found
		cmp eax, -0x1000
		ja .found
		pop eax
		mov eax, [eax + 0x1000 - 4]
		jmp @b
	.found:
		; When found, combine page address from eax with page offset from ecx.
		pop eax
		and ecx, 0xFFF
		add eax, ecx
	.zero:
		ret
endp