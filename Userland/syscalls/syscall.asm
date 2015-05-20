
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

	int 	80h

	push 	r15
	push 	r14
	push	r13
	push	r12
	push	rbp
	pop		rsp
	pop 	rbx

	mov		rsp,	rbp
	pop		rbp
	ret

	