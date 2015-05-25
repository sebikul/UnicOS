section .text
GLOBAL syscall


syscall:
	push 	rbp
	mov		rbp, 	rsp

	push	rbx
	push	rsp
	push	rbp
	push	r12
	push	r13
	push 	r14
	push 	r15

	int 	0x80

	pop 	r15
	pop 	r14
	pop		r13
	pop		r12
	pop		rbp
	pop		rsp
	pop 	rbx

	mov		rsp,	rbp
	pop		rbp
	ret