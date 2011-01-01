	.text
	.code16
	.global bootload_entry
bootload_entry:
	cli
	xor	%ax,%ax
	mov	%ax,%ss
	mov	%ax,%ds
	mov	%ax,%es
	mov	$0x6000,%sp
	mov	%sp,%bp
	mov	%sp,%di
	mov	$0x7C00,%si
	mov	$0x0200,%cx
	cld
	rep	movsb
	jmp	$0x0000,$start
IRQ0:
	push	%ax
	movb	$0x01,IRQ0.f
	mov	$0x20,%al
	out	%al,$0x20
	pop	%ax
	iret
IRQ0.f:	.byte	0x00

IRQ6:
	push	%ax
	movb	$0x01,IRQ6.f
	mov	$0x20,%al
	out	%al,$0x20
	pop	%ax
	iret
IRQ6.f:	.byte	0x00

timer:
	hlt
	cmpb	$0x01,IRQ0.f
	jne	timer
	movb	$0x00,IRQ0.f
	loop	timer
	ret

waitfloppy:
	hlt
	cmpb	$0x01,IRQ6.f
	jne	waitfloppy
	movb	$0x00,IRQ6.f
	ret

readFIFO:
	mov	$0x03F4,%dx
	in	%dx,%al
	and	$0x80,%al
	cmp	$0x80,%al
	jne	readFIFO
	inc	%dx
	in	%dx,%al
	ret

writeFIFO:
	mov	$0x03F4,%dx
	in	%dx,%al
	and	$0x80,%al
	cmp	$0x80,%al
	jne	writeFIFO
	inc	%dx
	mov	%ah,%al
	out	%al,%dx
	ret

start:
	movw	$IRQ0,0x0020
	movw	$0x00,0x0022
	movw	$IRQ6,0x0038
	movw	$0x00,0x003A

	# E
	mov	$0x0E45,%ax
	mov	$0x0007,%bx
	int	$0x10

	#PIC (master)
	mov	$0x11,%al
	out	%al,$0x20
	mov	$0x08,%al
	out	%al,$0x21
	mov	$0x04,%al
	out	%al,$0x21
	mov	$0x01,%al
	out	%al,$0x21
	mov	$0xBE,%al
	out	%al,$0x21

	#PIT
	mov	$0x36,%al
	out	%al,$0x43
	mov	$0x0B,%al
	out	%al,$0x40
	mov	$0xE9,%al
	out	%al,$0x40

	#DMA (master)
	mov	$0x00,%al
	out	%al,$0xC8
	mov	$0xFF,%al
	out	%al,$0xCD
	mov	$0x0E,%al
	out	%al,$0xCF

	#DMA (slave)
	mov	$0x00,%al
	out	%al,$0x08
	mov	$0xFF,%al
	out	%al,$0x0D
	mov	$0x0E,%al
	out	%al,$0x0F

	# X
	mov	$0x0E58,%ax
	mov	$0x0007,%bx
	int	$0x10

	# wait 3 secs
	sti
	mov	$0x003C,%cx
	call	timer

	#FDC init

	mov	$0x03F2,%dx
	mov	$0x00,%al
	out	%al,%dx
	nop
	nop
	nop

	mov	$0x0C,%al
	out	%al,%dx
	call	waitfloppy
	mov	$0x08,%ah
	call	writeFIFO
	call	readFIFO
	call	readFIFO

	mov	$0x03F7,%dx
	mov	$0x00,%al
	out	%al,%dx

	mov	$0x03,%ah
	call	writeFIFO
	mov	$0xDF,%ah
	call	writeFIFO
	mov	$0x02,%ah
	call	writeFIFO

	# floppy motor on

	mov	$0x03F2,%dx
	mov	$0x1C,%al
	out	%al,%dx
	mov	$0x000D,%cx
	call	timer

	# calibrate

cal:	mov	$0x07,%ah
	call	writeFIFO
	mov	$0x00,%ah
	call	writeFIFO
	call	waitfloppy

	mov	$0x08,%ah
	call	writeFIFO
	call	readFIFO
	call	readFIFO
	cmp	$0x00,%al
	jne	cal

	# set up DMA

	mov	$0xFF,%al
	out	%al,$0x0C
	mov	$0x00,%al
	out	%al,$0x04
	mov	$0x7C,%al
	out	%al,$0x04
	mov	$0x00,%al
	out	%al,$0x81

	mov	$0xFF,%al
	out	%al,$0x0C
	mov	$0xFF,%al
	out	%al,$0x05
	mov	$0x47,%al
	out	%al,$0x05

	mov	$0x46,%al
	out	%al,$0x0B
	mov	$0x0A,%al
	out	%al,$0x0F

	# O
	mov	$0x0E4F,%ax
	mov	$0x0007,%bx
	int	$0x10

	# floppy read

	mov	$0x0005,%cx
	call	timer

	mov	$0xC6,%ah
	call	writeFIFO
	mov	$0x00,%ah
	call	writeFIFO
	mov	$0x00,%ah
	call	writeFIFO
	mov	$0x00,%ah
	call	writeFIFO
	mov	$0x01,%ah
	call	writeFIFO
	mov	$0x02,%ah
	call	writeFIFO
	mov	$0x12,%ah
	call	writeFIFO
	mov	$0x1B,%ah
	call	writeFIFO
	mov	$0xFF,%ah
	call	writeFIFO

	call	waitfloppy

	call	readFIFO	#st0
	call	readFIFO	#st1
	call	readFIFO	#st2

	call	readFIFO	#rcy
	call	readFIFO	#rhe
	call	readFIFO	#rse

	call	readFIFO	#bps

	# floppy motor off
	mov	$0x03F2,%dx
	mov	$0x0C,%al
	out	%al,%dx
	mov	$0x000D,%cx
	call	timer

	# S
	mov	$0x0E53,%ax
	mov	$0x0007,%bx
	int	$0x10

	# get BIOS info (store at 0x6006, move later to 0x8006)

	mov	$0x0F,%ah
	int	$0x10
	cmp	$0x04,%al
	ja	error
	mov	%al,0x6006

	xor	%ax,%ax
	int	$0x12
	jc	error
	cmp	$0x0100,%ax
	jb	error
	mov	%ax,0x6008

	mov	$0x88,%ah
	int	$0x15
	mov	%ax,0x600E

	xor	%dx,%dx
	xor	%cx,%cx
	mov	$0xE801,%ax
	int	$0x15
	jcxz	useax
	mov	%cx,%ax
	mov	%dx,%bx
useax:
	mov	%ax,0x600A
	mov	%bx,0x600C

	# jump to second part of loader
	jmp	0x7E00
	
error:
	mov	$0x0E21,%ax
	mov	$0x0007,%bx
	int	$0x10
	cli
	hlt

	.word	0xAA55
