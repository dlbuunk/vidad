	# this file contains memory and port functions
	# all functions can be called by c/c++
	.text
	.code32

	.global memstob
	# void memstob(byte *addr, byte val, size_t count);
memstob:
	push	%ebp
	mov	%esp,%ebp
	mov	8(%ebp),%edi
	mov	16(%ebp),%ecx
	mov	12(%ebp),%al
	jcxz	msb
	rep	stosb
msb:	pop	%ebp
	ret

	.global memstow
	# void memstow(word *addr, word val, size_t count);
memstow:
	push	%ebp
	mov	%esp,%ebp
	mov	8(%ebp),%edi
	mov	16(%ebp),%ecx
	mov	12(%ebp),%ax
	jcxz	msw
	rep	stosw
msw:	pop	%ebp
	ret

	.global memcpyb
	# void memcpyb(byte *to, byte *from, size_t count);
memcpyb:
	push	%ebp
	mov	%esp,%ebp
	mov	8(%ebp),%edi
	mov	16(%ebp),%ecx
	mov	12(%ebp),%esi
	jcxz	mcb
	rep	movsb
mcb:	pop	%ebp
	ret

	.global memcpyw
	# void memcpyw(word *to, word *from, size_t count);
memcpyw:
	push	%ebp
	mov	%esp,%ebp
	mov	8(%ebp),%edi
	mov	16(%ebp),%ecx
	mov	12(%ebp),%esi
	jcxz	mcw
	rep	movsw
mcw:	pop	%ebp
	ret

	.global inportb
	# byte inportb(word port);
inportb:
	push	%ebp
	mov	%esp,%ebp
	mov	8(%ebp),%dx
	in	%dx,%al
	pop	%ebp
	ret

	.global outportb
	# void outportb(word port, byte val);
outportb:
	push	%ebp
	mov	%esp,%ebp
	mov	8(%ebp),%dx
	mov	12(%ebp),%al
	out	%al,%dx
	pop	%ebp
	ret

