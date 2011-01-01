	.text
	.code16
	.global loader_entry
loader_entry:

	cli

	# exit FDC
	mov	$0x00,%al
	out	%al,%dx

	# exit DMA (slave)
	mov	$0x0F,%al
	out	%al,$0x0F
	mov	$0x04,%al
	out	%al,$0x08

	# exit DMA (master)
	mov	$0x0F,%al
	out	%al,$0xCF
	mov	$0x04,%al
	out	%al,$0xC8

	# exit PIC
	mov	$0xFF,%al
	out	%al,$0x21

	# CPUID

	pushf
	pop	%ax
	mov	%ax,%cx
	and	$0x0FFF,%ax
	push	%ax
	popf
	pushf
	pop	%ax
	and	$0xF000,%ax
	cmp	$0xF000,%ax
	je	error

	or	$0xF000,%cx
	push	%cx
	popf
	pushf
	pop	%ax
	and	$0xF000,%ax
	jz	error

	pushfl
	pop	%eax
	mov	%eax,%ecx
	xor	$0x00040000,%eax
	push	%eax
	popfl
	pushfl
	pop	%eax
	xor	%ecx,%eax
	movb	$0x03,0x6007
	jz	end_cpuid
	push	%ecx
	popfl

	movb	$0x04,0x6007
	mov	%ecx,%eax
	xor	$0x00200000,%eax
	push	%eax
	popfl
	pushfl
	pop	%eax
	xor	%ecx,%eax
	je	end_cpuid
	movb	$0x05,0x6007

end_cpuid:

	# NMI off
	in	$0x70,%al
	or	$0x80,%al
	out	%al,$0x70

	jmp	A20

waitA:
	in	$0x64,%al
	test	$0x02,%al
	jnz	waitA
	ret

waitB:
	in	$0x64,%al
	test	$0x01,%al
	jz	waitB
	ret

error:
	mov	$0x0E21,%ax
	mov	$0x0007,%bx
	int	$0x10
	cli
	hlt

A20:
	call	waitA
	mov	$0xAD,%al
	out	%al,$0x64

	call	waitA
	mov	$0xD0,%al
	out	%al,$0x64

	call	waitB
	in	$0x60,%al
	push	%ax

	call	waitA
	mov	$0xD1,%al
	out	%al,$0x64

	call	waitA
	pop	%ax
	or	$0x03,%al
	out	%al,$0x60

	# checksum

	mov	0x8014,%cx
	shr	$0x02,%cx
	xor	%ax,%ax
	xor	%dx,%dx
	mov	$0x8000,%si
sum:	add	(%si),%ax
	inc	%si
	inc	%si
	adc	(%si),%dx
	inc	%si
	inc	%si
	loop	sum
	cmp	%ax,%dx
	jne	error
	xor	%dx,%dx
	cmp	%ax,%dx
	jne	error

	# reloc BIOS info

	mov	$0x6006,%si
	mov	$0x8006,%di
	mov	$0x000A,%cx
	rep	movsb

	# init tables

	mov	$0x0800,%di
	mov	0x801C,%si
	add	$0x8000,%si
	mov	0x801E,%cx
	rep	movsb

	mov	$0x1000,%di
	mov	0x8018,%si
	add	$0x8000,%si
	mov	0x801A,%cx
	rep	movsb

	mov	0x801A,%ax
	dec	%ax
	mov	%ax,0x6000
	mov	$0x1000,%ax
	mov	%ax,0x6002
	xor	%ax,%ax
	mov	%ax,0x6004

	mov	0x801E,%ax
	dec	%ax
	mov	%ax,0x6008
	mov	$0x0800,%ax
	mov	%ax,0x600A
	xor	%ax,%ax
	mov	%ax,0x600C

	# init final goto

	mov	$end_jmp,%bx
	mov	0x8024,%ax
	mov	%ax,2(%bx)
	mov	0x8026,%ax
	mov	%ax,4(%bx)

	# enter pmode

	lidt	0x6008
	lgdt	0x6000

	smsw	%ax
	or	$0x01,%al
	lmsw	%ax

end_jmp:
	jmpl	$0x0008,$0x00000000
