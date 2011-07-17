	.text
	.code16
	.global _bootsect_entry
_bootsect_entry:
	cli
	cld
	jmpw	$0x0000,$0x7C10
	.ascii	"VIDAD"
	.long	0x00000000

	# execution starts here
	xorw	%ax,%ax
	movw	%ax,%ss
	movw	%ax,%sp
	movw	%sp,%bp
	movw	%ax,%ds
	movw	%ax,%es
	jmp	start

IRQ0:
	pushw	%ax
	movb	$0x01,IRQ0.f
	movb	$0x20,%al
	outb	%al,$0x20
	popw	%ax
	iret
IRQ0.f: .byte 0x00

IRQ6:
	pushw	%ax
	movb	$0x01,IRQ6.f
	movb	$0x20,%al
	outb	%al,$0x20
	popw	%ax
	iret
IRQ6.f: .byte 0x00

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
	movw	$0x03F4,%dx
	inb	%dx,%al
	andb	$0x80,%al
	cmpb	$0x80,%al
	jne	readFIFO
	incw	%dx
	inb	%dx,%al
	ret

writeFIFO:
	movw	$0x03F4,%dx
	inb	%dx,%al
	andb	$0x80,%al
	cmpb	$0x80,%al
	jne	writeFIFO
	incw	%dx
	movb	%ah,%al
	outb	%al,%dx
	ret

start:
	# setup interrupt vectors
	movw	$IRQ0,0x0020
	movw	$0x00,0x0022
	movw	$IRQ6,0x0038
	movw	$0x00,0x003A

	#PIC (master)
	movb	$0x11,%al
	outb	%al,$0x20
	movb	$0x08,%al
	outb	%al,$0x21
	movb	$0x04,%al
	outb	%al,$0x21
	movb	$0x01,%al
	outb	%al,$0x21
	movb	$0xBE,%al
	outb	%al,$0x21

	#PIT
	movb	$0x36,%al
	outb	%al,$0x43
	movb	$0x0B,%al
	outb	%al,$0x40
	movb	$0xE9,%al
	outb	%al,$0x40

	#DMA
	movb	$0x00,%al
	outb	%al,$0xC8
	outb	%al,$0x08
	movb	$0xFF,%al
	outb	%al,$0xCD
	outb	%al,$0x0D
	movb	$0x0E,%al
	outb	%al,$0xCF
	outb	%al,$0x0F

	sti

	#FDC init

	movw	$0x03F2,%dx
	movb	$0x00,%al
	outb	%al,%dx

	movw	$0x0005,%cx
	call	timer
	movb	$0x00,IRQ6.f

	movb	$0x0C,%al
	outb	%al,%dx
	call	waitfloppy
	movb	$0x08,%ah
	call	writeFIFO
	call	readFIFO
	call	readFIFO

	movw	$0x03F7,%dx
	movb	$0x00,%al
	outb	%al,%dx

	movb	$0x03,%ah
	call	writeFIFO
	movb	$0xDF,%ah
	call	writeFIFO
	movb	$0x02,%ah
	call	writeFIFO

	# floppy motor on

	movw	$0x03F2,%dx
	movb	$0x1C,%al
	outb	%al,%dx
	movw	$0x000D,%cx
	call	timer

cal:	# calibrate

	movb	$0x07,%ah
	call	writeFIFO
	movb	$0x00,%ah
	call	writeFIFO
	call	waitfloppy

	movb	$0x08,%ah
	call	writeFIFO
	call	readFIFO
	call	readFIFO
	cmpb	$0x00,%al
	jne	cal

	# setup DMA for transfer

	movb	$0x06,%al
	outb	%al,$0x0A

	movb	$0xFF,%al
	outb	%al,$0x0C
	movb	$0x00,%al
	outb	%al,$0x04
	movb	$0x10,%al
	outb	%al,$0x04
	movb	$0x00,%al
	outb	%al,$0x81

	movb	$0xFF,%al
	outb	%al,$0x0C
	movb	$0xFF,%al
	outb	%al,$0x05
	movb	$0x0F,%al
	outb	%al,$0x05

	movb	$0x46,%al
	outb	%al,$0x0B
	movb	$0x0A,%al
	outb	%al,$0x0F

	# read the floppy

	movw	$0x0005,%cx
	call	timer

	movb	$0x46,%ah
	call	writeFIFO
	movb	$0x00,%ah
	call	writeFIFO
	movb	$0x00,%ah
	call	writeFIFO
	movb	$0x00,%ah
	call	writeFIFO
	movb	$0x01,%ah
	call	writeFIFO
	movb	$0x02,%ah
	call	writeFIFO
	movb	$0x12,%ah
	call	writeFIFO
	movb	$0x1B,%ah
	call	writeFIFO
	movb	$0xFF,%ah
	call	writeFIFO

	call	waitfloppy

	call	readFIFO
	call	readFIFO
	call	readFIFO
	call	readFIFO
	call	readFIFO
	call	readFIFO
	call	readFIFO

	# print '?' and HALT
	movw	$0x0E3F,%ax
	movw	$0x0007,%bx
	int	$0x10
	cli
	hlt
