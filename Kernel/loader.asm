global		loader
global		intson
global		intsoff
global		hang
extern		main
extern		initializeKernelBinary
extern 		_kdebug
global 		savestack

extern 		main
extern 		initializeKernelBinary

extern 		keyboard_irq_handler
extern 		irq0_handler
extern 		irq80_handler

extern 		task_save_current_stack
extern 		task_get_current_stack
extern 		task_next
extern 		task_get_current

%macro pusha 0 								; ABI Registers: No permiten llamar a funciones en C sin corromper nada
		push 		rax
		push 		rcx
		push 		rdx
		push 		rdi
		push 		rsi
		push 		r8 
		push 		r9 
		push 		r10
		push 		r11
%endmacro

%macro popa 0
		pop 		r11
		pop 		r10
		pop 		r9 
		pop 		r8 
		pop 		rsi
		pop 		rdi
		pop 		rdx
		pop 		rcx
		pop 		rax
%endmacro


loader:
		call 		initializeKernelBinary		; Set up the kernel binary, and get thet stack address

		mov			rsp, 	rax					; Set up the stack with the returned address
		push 		rax

		call 		set_interrupt_handlers
		call 		init_pic
		call		main
		hlt										; Halt hasta la primera interrupcion


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

		;mov 		rdi,	 rsp
		;call 		task_save_current_stack		; Guardamos el stack de la tarea actual en task->rsp
		; mov 		rsp, 	[stack]				; Entramos al stack del kernel

		call 		task_get_current 			; En rax tenemos la tarea actual. Hay que guardar el contexto

		add 		rax, 	8
		mov 		[rax + 0x0], rbp 
		mov 		[rax + 0x8], rbx 
		mov 		[rax + 0x10], r12 
		mov 		[rax + 0x18], r13 
		mov 		[rax + 0x20], r14 
		mov 		[rax + 0x28], r15 
		mov 		[rax + 0x30], rsp 			; Ya guardamos el contexto. Hay que cambiar de tarea y restarurar el anterior

		call 		irq0_handler

		call 		task_next		 			; Cambia current en task.c. Deja la tarea nueva en rax

		add 		rax, 	8
		mov 		rbp, [rax + 0x0]
		mov 		rbx, [rax + 0x8]
		mov 		r12, [rax + 0x10]
		mov 		r13, [rax + 0x18]
		mov 		r14, [rax + 0x20]
		mov 		r15, [rax + 0x28]
		mov 		rsp, [rax + 0x30]

		; mov 		[stack], rsp				; Salimos del stack del kernel
		;call 		task_get_current_stack		; Deja en %rax la direccion del stack de la tarea anterior
		;mov			rsp,	 rax

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
