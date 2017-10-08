iglobal
	uhci_service_name: db 'UHCI', 0
	ohci_service_name: db 'OHCI', 0
	ehci_service_name: db 'EHCI', 0
endg

;Инициализация системы USB
proc usb_init
	;Инициализация lock'ов
	mov ecx, usb_controllers_list_mutex
	call mutex_init

	mov esi, pcidev_list
	push 0
	.kickoff:
		mov esi, [esi + PCIDEV.fd]
		cmp esi, pcidev_list
		jz .done_kickoff

		cmp word [esi + PCIDEV.class + 1], 0x0C03
		jnz .kickoff

		mov ebx, uhci_service_name
		cmp byte [esi + PCIDEV.class], 0x00
		jz .do_kickoff

		mov ebx, ohci_service_name
		cmp byte [esi + PCIDEV.class], 0x10
		jz .do_kickoff

		mov ebx, ehci_service_name
		cmp byte [esi + PCIDEV.class], 0x20
		jnz .kickoff
	.do_kickoff:
		inc dword[esp]
		push ebx esi
		stdcall get_service, ebx
		pop esi ebx
		test eax, eax
		jz .driver_fail

		mov edx, [eax + USBSRV.usb_func]
		cmp [edx + usb_hardware_func.Version], USBHC_VERSION
		jnz .driver_invalid

		mov [esi + PCIDEV.owner], eax
		call [edx + usb_hardware_func.BeforeInit]
		jmp .kickoff
	.driver_fail:
		DEBUGF 1, 'K : failed to load driver %s\n', ebx
		jmp .kickoff
	.driver_invalid:
		DEBUGF 1, 'K : driver %s has wrong version\n', ebx
		jmp .kickoff
	.done_kickoff:
		pop eax
		;Если нет контроллеров, выходим. Иначе инит
		test eax, eax
		jz .nothing
		call create_usb_thread
		jz .nothing
	;Инициализация USB контроллеров. запуск иниц. для каждого
		mov eax, pcidev_list
	.scan_ehci:
		mov eax, [eax + PCIDEV.fd]
		cmp eax, pcidev_list
		jz .done_ehci

		cmp [eax + PCIDEV.class], 0x0C0320
		jnz .scan_ehci

		call usb_init_controller
		jmp .scan_ehci
	.done_ehci:
		;Цикл на все usb
		mov eax, pcidev_list
	.scan_usb1:
		mov eax, [eax + PCIDEV.fd]
		cmp eax, pcidev_list
		jz .done_usb1

		cmp [eax + PCIDEV.class], 0x0C0300
		jz @f

		cmp [eax + PCIDEV.class], 0x0C0310
		jnz .scan_usb1
	@@:
		call usb_init_controller
		jmp .scan_usb1
	.done_usb1:
	.nothing:
		ret
endp

uglobal
	align 4
	usb_event dd ?
endg

;Вспомогательная функция к usb_init. Создает usb поток
proc create_usb_thread
	;Создание потока
	push edi
	movi ebx, 1
	mov ecx, usb_thread_proc
	xor edx, edx
	call new_sys_threads
	pop edi
	;Если не получилось
	test eax, eax
	jns @f
	DEBUGF 1, 'K : cannot create kernel thread for USB, error %d\n', eax
	.clear:
		xor eax, eax
		jmp .nothing
	@@:
	;Ждать пока USB поток инициализирует сам себя
	@@:
		call change_task
		cmp [usb_event], 0
		jz @b
	;Если инициализация провалилась
	cmp [usb_event], -1
	jz .clear

	.nothing:
    	ret
endp

;Вспомогательная функция для IRQ обработчиков. Пробуждает USB поток если ebx != 0
proc usb_wakeup_if_needed
	test ebx, ebx
	jz usb_wakeup.nothing

	usb_wakeup:
		xor edx, edx
		mov eax, [usb_event]
		mov ebx, [eax + EVENT.id]
		xor esi, esi
		call raise_event
	.nothing:
		ret
endp

;Главный цикл USB потока
proc usb_thread_proc
	;1. Initialize: create event to allow wakeup by interrupt handlers.
	xor esi, esi
	mov ecx, MANUAL_DESTROY
	call create_event
	test eax, eax
	jnz @f
	;Если не получилось
	dbgstr 'cannot create event for USB thread'
	or [usb_event], -1
	jmp sys_end

	@@:
		mov [usb_event], eax
		push -1 ;Таймаут - беск.
	usb_thread_wait:
	;Главный цикл. Ожидание таймаута
		pop ecx ;Получить таймаут
		mov eax, [usb_event]
		mov ebx, [eax + EVENT.id]
		call wait_event_timeout
		push -1 ;Таймаут - беск.
	;Главная функция. Запрос рабочих функций всех контроллеров
		mov esi, usb_controllers_list
	@@:
		mov esi, [esi + usb_controller.Next]
		cmp esi, usb_controllers_list
		jz .controllers_done
		mov eax, [esi + usb_controller.HardwareFunc]
		call [eax + usb_hardware_func.ProcessDeferred]
		cmp [esp], eax
		jb @b
		mov [esp], eax
		jmp @b
	.controllers_done:
	;4. Main loop: call hub worker function for all hubs,
		mov esi, usb_hubs_list
	@@:
		mov esi, [esi + usb_hub.Next]
		cmp esi, usb_hubs_list
		jz usb_thread_wait
		call usb_hub_process_deferred
		cmp [esp], eax
		jb @b
		mov [esp], eax
		jmp @b
endp

iglobal
	align 4
	usb_controllers_list: 
		dd usb_controllers_list
		dd usb_controllers_list
	usb_hubs_list:
		dd usb_hubs_list
		dd usb_hubs_list
endg

uglobal
	align 4
	usb_controllers_list_mutex MUTEX
endg

include "memory.inc"
include "common.inc"
include "hccommon.inc"
include "pipe.inc"
include "protocol.inc"
include "hub.inc"