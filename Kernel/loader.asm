global loader
extern main
extern initializeKernelBinary

extern 		main
extern 		initializeKernelBinary

extern		video_write_line
extern 		video_write_nl
extern 		keyboard_irq_handler
extern 		irq0_handler

;SYSCALLS
extern 		sys_write
extern 		sys_read
extern 		sys_rtc_get
extern 		sys_rtc_set
extern 		sys_malloc
extern 		sys_calloc
extern 		sys_free
extern 		sys_keyboard_catch
extern 		sys_clear_indexed_line
extern 		sys_keyboard_replace_buffer
extern 		sys_get_color
extern 		sys_set_color
extern 		sys_kbd_set_distribution
extern 		sys_set_screensaver_timer
extern		sys_clear_screen
extern 		sys_screensaver_trigger

loader:

		call 		initializeKernelBinary		; Set up the kernel binary, and get thet stack address

		mov			rsp, 	rax					; Set up the stack with the returned address
		push 		rax

		;mov 		rdi, msg_init 				;Inicializando IDT
		;call 		video_write_line
		;call 		video_write_nl

		call 		set_interrupt_handlers
		call 		init_pic
		call		main


hang:
		cli
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

		mov 		rdi, 	0x20				; Set up Keyboard handler
		mov 		rax,	pit_handler
		call 		create_gate

		lidt 		[IDTR64]					; load IDT register

		ret

align 16
soft_interrupt:									; Interrupciones de software, int 80h
		push 		rdi
		;push 		rax

		cmp 		rdi,	1
		jz			int_sys_rtc

		cmp 		rdi,	2
		jz			int_sys_rtc_set

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
		jz			int_sys_get_color

		cmp 		rdi,	12
		jz			int_sys_set_color

		cmp 		rdi,	14
		jz			int_sys_kbd_set_distribution

		cmp 		rdi, 	15
		jz 			int_sys_set_screensaver_timer

		cmp 		rdi, 	16
		jz 			int_sys_screensaver_trigger

		cmp			rdi,	17
		jz			int_sys_clear_screen

		cmp			rdi,	18
		jz			hang

		jmp 		soft_interrupt_done 		; La syscall no existe

int_sys_rtc:
		call 		prepare_params
		call 		sys_rtc_get
		jmp			soft_interrupt_done

int_sys_rtc_set:
		call 		prepare_params
		call 		sys_rtc_set
		jmp 		soft_interrupt_done

int_sys_write:
		call 		prepare_params
		call 		sys_write
		jmp 		soft_interrupt_done
int_sys_read:
		call 		prepare_params
		call 		sys_read
		jmp 		soft_interrupt_done

int_malloc:
		call 		prepare_params
		call 		sys_malloc
		jmp 		soft_interrupt_done
int_calloc:
		call 		prepare_params
		call 		sys_calloc
		jmp 		soft_interrupt_done
int_free:
		call 		prepare_params
		call 		sys_free
		jmp 		soft_interrupt_done

int_keyboard_catch:
		call 		prepare_params
		call 		sys_keyboard_catch
		jmp 		soft_interrupt_done

int_video_clr_indexed_line:
		call 		prepare_params
		call 		sys_clear_indexed_line
		jmp 		soft_interrupt_done

int_keyboard_replace_buffer:
		call 		prepare_params
		call 		sys_keyboard_replace_buffer
		jmp 		soft_interrupt_done

int_sys_get_color:
		call 		prepare_params
		call		sys_get_color
		jmp			soft_interrupt_done

int_sys_set_color:
		call 		prepare_params
		call		sys_set_color
		jmp 		soft_interrupt_done

int_sys_kbd_set_distribution:
		call 		prepare_params
		call		sys_kbd_set_distribution
		jmp 		soft_interrupt_done

int_sys_set_screensaver_timer:
		call 		prepare_params
		call		sys_set_screensaver_timer
		jmp 		soft_interrupt_done

int_sys_clear_screen:
		call 		prepare_params
		call 		sys_clear_screen
		jmp			soft_interrupt_done

int_sys_screensaver_trigger:
		call 		prepare_params
		call		sys_screensaver_trigger
		jmp 		soft_interrupt_done

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

pit_handler:
		push 		rdi
		push 		rax

		call 		irq0_handler

		mov			al, 	0x20				; Acknowledge the IRQ
		out 		0x20, 	al

		pop 		rax
		pop 		rdi
		iretq

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

init_pic:
	; Enable specific interrupts
	in al, 0x21
	mov al, 11111000b		; Enable Cascade, Keyboard
	out 0x21, al

	sti				; Enable interrupts

	ret
