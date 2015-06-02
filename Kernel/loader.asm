global loader
extern main
extern initializeKernelBinary

extern 		main
extern 		initializeKernelBinary

extern 		sys_write
extern 		sys_read
extern 		exit
extern		video_write_line
extern 		video_write_nl
extern 		keyboard_irq_handler
extern 		sys_rtc_time
extern 		sys_malloc
extern 		sys_calloc
extern 		sys_free
extern 		sys_keyboard_catch
extern 		sys_clear_indexed_line
extern 		sys_keyboard_replace_buffer
extern 		sys_color

loader:

		call 		initializeKernelBinary		; Set up the kernel binary, and get thet stack address

		mov			rsp, 	rax					; Set up the stack with the returned address
		push 		rax

		mov 		rdi, msg_init 				;Inicializando IDT
		call 		video_write_line
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
		;push 		rax

		cmp 		rdi,	2
		jz			int_sys_rtc

		cmp			rdi, 	3
		jz 			int_sys_read

		cmp			rdi, 	4
		jz			int_sys_write

		cmp 		rdi,	5
		jz 			int_malloc

		cmp 		rdi,	6
		jz 			int_calloc

		cmp 		rdi,	7
		jz 			int_free

		cmp 		rdi, 	8
		jz 			int_keyboard_catch

		cmp 		rdi, 	9
		jz 			int_video_clr_indexed_line

		cmp 		rdi, 	10
		jz 			int_keyboard_replace_buffer

		cmp			rdi,	11
		jz			int_sys_color

		jmp 		soft_interrupt_done 		; La syscall no existe

int_sys_rtc:
		call 		sys_rtc_handler
		jmp			soft_interrupt_done
int_sys_write:
		call 		sys_write_handler
		jmp 		soft_interrupt_done
int_sys_read:
		call 		sys_read_handler
		jmp 		soft_interrupt_done

int_malloc:
		call 		sys_malloc_handler
		jmp 		soft_interrupt_done
int_calloc:
		call 		sys_calloc_handler
		jmp 		soft_interrupt_done
int_free:
		call 		sys_free_handler
		jmp 		soft_interrupt_done

int_keyboard_catch:
		call 		sys_keyboard_catch_handler
		jmp 		soft_interrupt_done

int_video_clr_indexed_line:
		call 		sys_video_clr_indexed_line_handler
		jmp 		soft_interrupt_done

int_keyboard_replace_buffer:
		call 		sys_keyboard_replace_buffer_handler
		jmp 		soft_interrupt_done

int_sys_color:
		call 		sys_color_handler
		jmp			soft_interrupt_done

soft_interrupt_done:
		push 		rax
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

		mov 		rdi,	 rax
		call 		keyboard_irq_handler

keyboard_done:
		mov			al, 	0x20				; Acknowledge the IRQ
		out 		0x20, 	al

		pop 		rax
		pop 		rdi
		iretq

sys_rtc_handler:
		call 		prepare_params
		call 		sys_rtc_time
		ret

sys_write_handler:
		call 		prepare_params
		call 		sys_write
		ret

sys_read_handler:
		call 		prepare_params
		call 		sys_read
		ret

sys_malloc_handler:
		call 		prepare_params
		call 		sys_malloc
		ret

sys_calloc_handler:
		call 		prepare_params
		call 		sys_calloc
		ret

sys_free_handler:
		call 		prepare_params
		call 		sys_free
		ret

sys_keyboard_catch_handler:
		call 		prepare_params
		call 		sys_keyboard_catch
		ret

sys_video_clr_indexed_line_handler:
		call 		prepare_params
		call 		sys_clear_indexed_line
		ret

sys_keyboard_replace_buffer_handler:
		call 		prepare_params
		call 		sys_keyboard_replace_buffer
		ret

sys_color_handler:
		call 		prepare_params
		call		sys_color
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
