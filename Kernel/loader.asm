global		loader
global		intson
global		intsoff
global		hang
extern		main
extern		initializeKernelBinary
extern 		_kdebug

extern 		main
extern 		initializeKernelBinary

extern 		keyboard_irq_handler
extern 		irq0_handler
extern 		irq80_handler

extern 		task_save_current_stack
extern 		task_get_current_stack
extern 		task_reschedule

%macro pusha 0
		push 		r15
		push 		r14
		push 		r13
		push 		r12
		push 		r11
		push 		r10
		push 		r9
		push 		r8
		push 		rbp
		push 		rax
		push 		rbx
		push 		rcx
		push 		rdx
		push 		rsi
		push 		rdi
%endmacro

%macro popa 0
		pop 		rdi
		pop 		rsi
		pop 		rdx
		pop 		rcx
		pop 		rbx
		pop 		rax
		pop 		rbp
		pop 		r8
		pop 		r9
		pop 		r10
		pop 		r11
		pop 		r12
		pop 		r13
		pop 		r14
		pop 		r15
%endmacro


loader:
		call 		initializeKernelBinary		; Set up the kernel binary, and get thet stack address

		mov			rsp, 	rax					; Set up the stack with the returned address
		mov 		[stack],  rsp				; Direccion del stack del kernel
		push 		rax

		call 		set_interrupt_handlers
		call 		init_pic
		call		main

stack 	resq	0

hang:
		call 		intsoff
		hlt										; halt machine should kernel return
		jmp 		hang


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

align 16
pit_handler:
		pusha

		mov 		rdi,	 rsp
		call 		task_save_current_stack		; Guardamos el stack de la tarea actual en task->rsp

		mov 		rsp, 	[stack]

		call 		irq0_handler

		call 		task_reschedule 			; Cambia current en task.c

		mov 		[stack], 	rsp

		call 		task_get_current_stack		; Deja en %rax la direccion del stack de la tarea anterior
		mov			rsp,	 rax

		mov			al, 	0x20				; Acknowledge the IRQ
		out 		0x20, 	al

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
