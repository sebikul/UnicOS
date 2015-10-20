global		loader
global		intson
global		intsoff
global 		gdt_flush
global 		halt

extern		main
extern		initializeKernelBinary
extern 		_kdebug

extern 		main
extern 		initializeKernelBinary

extern 		keyboard_irq_handler
extern 		irq0_handler
extern 		irq80_handler

extern 		task_save_stack
extern 		task_restore_stack
extern 		task_next
extern 		task_get_current
extern 		video_write_hex

extern 		stack_init
extern 		kernel_stack
extern 		kdebug_base
extern 		kdebug_nl

extern 		scheduler_u2k
extern 		scheduler_k2u

%macro pusha 0 								; ABI Registers: No permiten llamar a funciones en C sin corromper nada
		push		rax
		push		rbx
		push		rcx
		push		rdx
		push		rbp
		push		rdi
		push		rsi
		push		r8
		push		r9
		push		r10
		push		r11
		push		r12
		push		r13
		push		r14
		push		r15
		push		fs
		push		gs
%endmacro

%macro popa 0
		pop			gs
		pop			fs
		pop			r15
		pop			r14
		pop			r13
		pop			r12
		pop			r11
		pop			r10
		pop			r9
		pop			r8
		pop			rsi
		pop			rdi
		pop			rbp
		pop			rdx
		pop			rcx
		pop			rbx
		pop			rax
%endmacro


loader:
		cli

		call 		initializeKernelBinary		; Set up the kernel binary, and get thet stack address

		call 		stack_init
		mov 		rsp, rax
		push 		rax

		call 		set_interrupt_handlers
		call 		init_pic
		call		main
		sti
halt:
		hlt
		jmp halt
	;	hlt										; Halt hasta la primera interrupcion

IDTR64:											; Interrupt Descriptor Table Register
		dw 			256*16-1					; limit of IDT (size minus one) (4096 bytes - 1)
		dq 			0x0000000000000000			; linear address of IDT


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
		pusha

		call 		irq80_handler

		push 		rax
		mov 		al, 	0x20				; Acknowledge the IRQ
		out 		0x20, 	al
		pop 		rax
	
		popa

		iretq

msg: 		db 'START PIT',10 ,0
msg1: 		db 'END PIT',10 ,0


pit_handler:
		pusha
		cli

		mov rdi, msg
		call _kdebug

		mov 		rdi,	 rsp
		call 		scheduler_u2k
		mov 		rsp, 	rax

		call 		irq0_handler

		call  		scheduler_k2u
		mov			rsp,	 rax

		mov rdi, msg1
		call _kdebug

		mov			al, 	0x20				; Acknowledge the IRQ
		out 		0x20, 	al

		sti
		popa
		iretq

align 16
keyboard:
		push 		rdi
		push 		rax

		xor			eax, 	eax

		in 			al, 	0x60				; Get the scancode from the keyboard
		cmp 		al, 	0xE0

		jnz 		keyboard_scancode_read
		xchg 		al, 	ah
		in 			al, 	0x60

keyboard_scancode_read:
		mov 		rdi,	 rax
		call 		keyboard_irq_handler

keyboard_done:
		mov			al, 	0x20				; Acknowledge the IRQ
		out 		0x20, 	al

		pop 		rax
		pop 		rdi
		iretq

init_pic:										; Enable specific interrupts
		in 			al, 	0x21
		mov 		al, 	11111000b			; Enable Cascade, Keyboard
		out 		0x21,	 al

		;call 		intson						; Enable interrupts
		ret

intson:
		sti
		ret

intsoff:
		cli
		ret

gdt_flush:
	lgdt [rdi]
	ret