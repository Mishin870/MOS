;Генерация рандомного числа
r_random:
	push edx ecx
	mov eax, [random_seed]
	xor edx, edx
	mov ecx, 127773
	div ecx
	mov ecx, eax
	mov eax, 16807
	mul edx
	mov edx, ecx
	mov ecx, eax
	mov eax, 2836
	mul edx
	sub ecx, eax
	xor edx, edx
	mov eax, ecx
	mov [random_seed], ecx
	mov ecx, 100000
	div ecx
	mov eax, edx
	pop ecx edx
ret

;Генерация рандомного числа в диапазоне MIN = EBX, MAX = ECX
r_randomFT:
	push edx
	;mov ecx,[rmax]
	sub ecx, ebx ;MAX - MIN
	inc ecx
	call r_random
	xor edx, edx
	div ecx
	mov eax, edx
	add eax, ebx
	pop edx
ret

;Инициализация рандома
r_init:
	push eax edx
	rdtsc
	xor eax, edx
	mov [random_seed], eax
	pop edx eax
ret

random_seed dd 0