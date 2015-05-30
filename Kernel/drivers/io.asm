global outb
global inb

outb:
		mov 		rax,	rsi
		mov 		rdx,	rdi

		;int 01h

		out 		dx,		al
		ret

inb:
		mov 		rdx, 	rdi
		xor 		rax,	rax
		;int 01h

		in  		al, 	dx
		ret