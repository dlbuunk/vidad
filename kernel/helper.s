	# this file contains memory and port functions
	# all functions can be called by c/c++
	.text
	.code32

	.global memstob
	# void memstob(dword addr, dword count, byte val);
memstob:
	push	%ebp
	mov	%esp,%ebp
	mov	8(%ebp),%edi
	mov	12(%ebp),%ecx
	mov	16(%ebp),%al
	rep	stosb
	pop	%ebp
	ret

	.global memstow
	# void memstow(dword addr, dword count, word val);
memstow:
	push	%ebp
	mov	%esp,%ebp
	mov	8(%ebp),%edi
	mov	12(%ebp),%ecx
	mov	16(%ebp),%ax
	rep	stosw
	pop	%ebp
	ret

	.global memcpyb
	# void memcpyb(dword to, dword count, dword from);
memcpyb:
	push	%ebp
	mov	%esp,%ebp
	mov	8(%ebp),%edi
	mov	12(%ebp),%ecx
	mov	16(%ebp),%esi
	rep	movsb
	pop	%ebp
	ret

	.global memcpyw
	# void memcpyw(dword to, dword count, dword from);
memcpyw:
	push	%ebp
	mov	%esp,%ebp
	mov	8(%ebp),%edi
	mov	12(%ebp),%ecx
	mov	16(%ebp),%esi
	rep	movsw
	pop	%ebp
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

