global		loader
global 		writeCR0
global 		readCR0
global 		writeCR1
global 		readCR1
global 		writeCR2
global 		readCR2
global 		writeCR3
global 		readCR3
global 		writeCR4
global 		readCR4
global		intson
global		intsoff
global 		gdt_flush
global 		halt
global 		reschedule
global 		get_flags

extern 		main
extern 		initializeKernelBinary

extern 		keyboard_irq_handler
extern 		page_fault_handler
extern 		irq0_handler
extern 		irq80_handler
extern 		task_next
extern 		task_getatomic
extern 		task_getquantum
extern 		task_decquantum

extern 		stack_init
extern 		kdebug_base
extern 		kdebug_nl
extern 		kernel_stack

global 		switch_u2k
global 		switch_k2u

extern 		pit_timer

extern 		scheduler_u2k
extern 		scheduler_k2u
global 		kbd_run_handler

%macro pusha 0
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
		jmp halt								; Halt hasta la primera interrupcion

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
		mov 		rdi, 0xE 					; Set up page fault handler
		mov 		rax, INT0E
		call 		create_gate

		mov 		rdi, 	0x80				; Set up Software Interrups handler
		mov 		rax, 	soft_interrupt
		call 		create_gate

		mov 		rdi, 	0x21				; Set up Keyboard handler
		mov 		rax,	keyboard
		call 		create_gate

		mov 		rdi, 	0x20				; Set up PIT handler
		mov 		rax,	pit_handler
		call 		create_gate

		lidt 		[IDTR64]					; load IDT register

		ret

align 16
soft_interrupt:									; Interrupciones de software, int 80h
		pusha
		cli

		call 		irq80_handler

		mov 		QWORD[ret_addr], rax

		sti
		popa
		mov 		rax, QWORD[ret_addr]
		iretq

align 16
pit_handler:
		pusha
		cli

		mov 		rax, 	[pit_timer]
		add 		rax, 	10
		mov 		[pit_timer], rax

		call 		task_getatomic
		cmp 		rax, 	0
		jne 		_eoi

		call 		task_getquantum
		cmp 		rax, 	0
		jne 		_leave_current

		mov 		rdi,	 rsp
		call 		scheduler_u2k
		mov 		rsp, 	rax

		call 		irq0_handler
		call 		task_next

		call  		scheduler_k2u
		mov			rsp,	 rax
		jmp 		_eoi

_leave_current:
		call 		task_decquantum

_eoi:

		mov			al, 	0x20				; Acknowledge the IRQ
		out 		0x20, 	al

		popa
		sti

		iretq

switch_u2k:
		pop 		QWORD[ret_addr] 			;Direccion de retorno

		mov 		QWORD[task_stack], rsp
		mov 		rsp, QWORD[kernel_stack]

		push 		QWORD[ret_addr]
		ret


switch_k2u:
		pop 		QWORD[ret_addr] 			;Direccion de retorno

		mov 		QWORD[kernel_stack], rsp
		mov 		rsp, QWORD[task_stack]

		push 		QWORD[ret_addr]
		ret


reschedule:
		;Simulamos una interrpucion
		pop 		QWORD[ret_addr] 			;Direccion de retorno

		mov 		QWORD[ss_addr], 	ss 		;Stack Segment
		push 		QWORD[ss_addr]

		push  		rsp
		pushf 									;Se pushean los flags
		mov 		QWORD[cs_addr], 	cs 		;Code Segment
		push 		QWORD[cs_addr]
		push 		QWORD[ret_addr] 			;Direccion de retorno

		;En este momento el stack contiene:
		;
		; > red_addr
		;	cs
		;	rflags
		;	rsp
		; 	ss

		pusha

		mov 		rdi,	 rsp
		call 		scheduler_u2k
		mov 		rsp, 	rax

		call 		task_next

		call  		scheduler_k2u
		mov			rsp,	 rax

		popa
		iretq

align 16
keyboard:
		pusha
		cli

		xor			rax, 	rax

		in 			al, 	0x60				; Get the scancode from the keyboard

		mov 		rdi,	 rax
		call 		keyboard_irq_handler

		mov			al, 	0x20				; Acknowledge the IRQ
		out 		0x20, 	al

		sti
		popa
		iretq

kbd_run_handler:
		; Hacemos un backup de los parametros para cambiar de stack
		mov 		QWORD[kbdhandler], rdi
		mov 		QWORD[scode], 	rsi
		mov 		QWORD[cr3_reg], rdx

		; Cambiamos al stack del kernel
		call 		switch_u2k

		mov 		rdi, 	QWORD[cr3_reg]

		; Nos fijamos si el handler corresponde al kernel o a una tarea
		cmp 		rdi, 	0
		je 			run_handler

		; Corresponde a una tarea
		; Hacemos un backup del cr3 y traemos la tarea a memoria

		call 		readCR3
		mov 		QWORD[cr3_bak], rax

		mov 		rdi, 	QWORD[cr3_reg]
		call 		writeCR3

		; Ya esta la tarea mapeada en memoria, falta ejecutar el handler
run_handler:
		mov 		rax, 	QWORD[kbdhandler]
		mov 		rdi, 	QWORD[scode]

		call 		rax

		; Si tuvimos que reemplazar el cr3, lo volvemos al estado original
		mov 		rdi, 	QWORD[cr3_reg]

		; Nos fijamos si el handler corresponde al kernel o a una tarea
		cmp 		rdi, 	0
		je 			kbd_handler_ret

		mov 		rdi, 	QWORD[cr3_bak]
		call 		writeCR3


kbd_handler_ret:
		call  		switch_k2u
		ret

init_pic:										; Enable specific interrupts
		in 			al, 	0x21
		mov 		al, 	11111000b			; Enable Cascade, Keyboard
		out 		0x21,	 al

		ret

gpe_handler:
		cli
		pusha



		popa
		sti

align 16
INT0E: ; page fault handler
    pusha
    mov     eax, [rsp+136] ; error code
    mov     rdi, rax
    mov     rsi, cr2
    call    page_fault_handler
    popa
    add     esp, 8 ; error code
    iretq

writeCR0:
		mov cr0, rdi
		ret

readCR0:
		mov rax, cr0
		ret

writeCR2:
		mov cr2, rdi
		ret

readCR2:
		mov rax, cr2
		ret

writeCR3:
		mov cr3, rdi
		ret

readCR3:
		mov rax, cr3
		ret

writeCR4:
		mov cr4, rdi
		ret

readCR4:
		mov rax, cr4
		ret

intson:
		sti
		ret

intsoff:
		cli
		ret

get_flags:
		pushf
		pop 		rax
		ret

gdt_flush:
		lgdt [rdi]
		ret

section .bss

ret_addr:
		resq 1
cs_addr:
		resq 1
ss_addr:
		resq 1
task_stack:
		resq 1

kbdhandler:
		resq 1
scode:
		resq 1
cr3_reg:
	 	resq 1
cr3_bak:
		resq 1
