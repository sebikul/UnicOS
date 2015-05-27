global loader
extern main
extern initializeKernelBinary

extern 		main
extern 		initializeKernelBinary

extern 		sys_write 
extern 		sys_read
extern 		exit
extern		video_write_pline
extern 		video_write_nl
extern 		keyboard_irq_handler

loader:

		call 		initializeKernelBinary		; Set up the kernel binary, and get thet stack address

		mov			rsp, 	rax					; Set up the stack with the returned address
		push 		rax

		mov 		rdi, msg_init 				;Inicializando IDT
		call 		video_write_pline
		call 		video_write_nl

		call 		set_interrupt_handlers

		call 		init_pic

		call		main

		; mov 		rdi, 4
		; mov 		rsi, 1
		; mov 		rdx, msg_test
		; mov 		rcx, msg_test_len
		; int 80h




hang:
		hlt										; halt machine should kernel return
		jmp 		hang


IDTR64:											; Interrupt Descriptor Table Register
		dw 			256*16-1					; limit of IDT (size minus one) (4096 bytes - 1)
		dq 			0x0000000000000000			; linear address of IDT


msg_init:			db "Inicializando IDT", 0
msg_test:			db "Mensaje IDT de prueba", 0
msg_test_len		equ $-msg_test

; create_gate
; rax = address of handler
; rdi = gate # to configure
create_gate:
		push 		rdi
		push 		rax

		shl 		rdi,	4					; quickly multiply rdi by 16
		stosw									; store the low word (15..0)
		shr 		rax, 	16
		add 		rdi,	4					; skip the gate marker
		stosw									; store the high word (31..16)
		shr 		rax,	16	
		stosd									; store the high dword (63..32)

		pop 		rax
		pop 		rdi
		ret

set_interrupt_handlers:
		mov 		rdi, 	0x80				; Set up Software Interrups handler
		mov 		rax, 	soft_interrupt
		call 		create_gate

		mov 		rdi, 	0x21				; Set up Keyboard handler
		mov 		rax,	keyboard
		call 		create_gate

		lidt 		[IDTR64]					; load IDT register

		ret

align 16
soft_interrupt:									; Interrupciones de software, int 80h
		push 		rdi
		push 		rax

		cmp			rdi, 	4
		jz			int_sys_write

		cmp			rdi, 	3
		jz 			int_sys_read

		jp 			soft_interrupt_done 		; La syscall no existe

int_sys_write:
		call 		sys_write_handler
		jp 			soft_interrupt_done

int_sys_read:
		call 		sys_read_handler
		jp 			soft_interrupt_done


soft_interrupt_done:
		mov 		al, 	0x20				; Acknowledge the IRQ
		out 		0x20, 	al

		pop 		rax
		pop 		rdi
		iretq

prepare_params:
		mov 		rdi,	rsi
		mov 		rsi,	rdx
		mov 		rdx,	rcx
		mov			rcx,	r8

		ret


align 16
keyboard:
		push 		rdi
		push 		rax

		xor			eax, 	eax

		in 			al, 	0x60				; Get the scancode from the keyboard

		;mov 		[0x000B8000], 	al			; Dump the scancode to the screen

		;and 		rax,	0xF 	

		;int 03h

		mov 		rdi,	 rax
		call 		keyboard_irq_handler

keyboard_done:
		mov			al, 	0x20				; Acknowledge the IRQ
		out 		0x20, 	al

		pop 		rax
		pop 		rdi
		iretq


sys_write_handler:
		call 		prepare_params
		call 		sys_write
		ret

sys_read_handler:
		call 		prepare_params
		call 		sys_read

		ret

init_pic:
	; Enable specific interrupts
	in al, 0x21
	mov al, 11111001b		; Enable Cascade, Keyboard
	out 0x21, al


	sti				; Enable interrupts

	ret


; =============================================================================
; EOF
