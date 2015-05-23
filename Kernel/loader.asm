global loader
extern main
extern initializeKernelBinary

extern 		main
extern 		initializeKernelBinary

extern 		sys_write 
extern 		sys_read
extern 		exit

loader:
		call 	set_interrupt_handlers
		call 		initializeKernelBinary		; Set up the kernel binary, and get thet stack address
		mov		rsp, rax			; Set up the stack with the returned address
		push 		rax
		call		 main
hang:
		hlt						; halt machine should kernel return
		jmp 		hang


IDTR64:								; Interrupt Descriptor Table Register
		dw 		256*16-1			; limit of IDT (size minus one) (4096 bytes - 1)
		dq 		0x0000000000000000		; linear address of IDT


; create_gate
; rax = address of handler
; rdi = gate # to configure
create_gate:
		push 		rdi
		push 		rax

		shl 		rdi,	4			; quickly multiply rdi by 16
		stosw						; store the low word (15..0)
		shr 		rax, 	16
		add 		rdi,	4			; skip the gate marker
		stosw						; store the high word (31..16)
		shr 		rax,	16
		stosd						; store the high dword (63..32)

		pop 		rax
		pop 		rdi
		ret

set_interrupt_handlers:
		mov 		rdi, 	0x80			; Set up Software Interrups handler
		mov 		rax, 	soft_interrupt
		call 		create_gate

		lidt 		[IDTR64]			; load IDT register

align 16
soft_interrupt:
		push 		rdi
		push 		rax

		cmp		rdi, 4
		jz		int_sys_write

		cmp		rdi, 3
		jz 		int_sys_read

		jp 		soft_interrupt_done 		; La syscall no existe

int_sys_write:
		call 		sys_write_handler
		jp 		soft_interrupt_done

int_sys_read:
		call 		sys_read_handler
		jp 		soft_interrupt_done


soft_interrupt_done:
		mov 		al, 	0x20			; Acknowledge the IRQ
		out 		0x20, 	al

		pop 		rax
		pop 		rdi
		iretq

prepare_params:
		mov 		rdi,	rsi
		mov 		rsi,	rdx
		mov 		rdx,	rcx
		mov		rcx,	r8

		ret


sys_write_handler:
		call 		prepare_params
		call 		sys_write
		ret

sys_read_handler:
		call 		prepare_params

		call 		sys_read

		ret
