	.text
	.code16
	.global _bootsect_entry
_bootsect_entry:
	cli
	cld
	jmpw	$0x0000,$0x7C10
	.ascii	"VIDAD"
	.long	0x00000000

	# main execution starts here
	# set the stack and segement registers
	xorw	%ax,%ax
	movw	%ax,%ss
	movw	%ax,%sp
	movw	%ax,%bp
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
IRQ0.f:	.byte	0x00

IRQ6:
	pushw	%ax
	movb	$0x01,IRQ6.f
	movb	$0x20,%al
	outb	%al,$0x20
	popw	%ax
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
	movw	$0x03F4,%dx
	inb	%dx,%al
	andb	$0x80,%al
	cmpb	$0x80,%al
	jne	readFIFO
	incw	%dx
	inb	%dx,%al
	ret

writeFIFO:
	movw	$0x03F,%dx
	inb	%dx,%al
	andb	$0x80,%al
	cmpb	$0x80,%al
	jne	writeFIFO
	movb	%ah,%al
	incw	%dx
	outb	%al,%dx
	ret

start:
	# init the PIC
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

	# init the PIT
	movb	$0x36,%al
	outb	%al,$0x43
	movb	$0x0B,%al
	outb	%al,$0x40
	movb	$0xE9,%al
	outb	%al,$0x40

	# set the interrupt vectors
	movw	$IRQ0,0x0020
	movw	$0x00,0x0022
	movw	$IRQ6,0x0038
	movw	$0x00,0x003A

	# interrupts on
	sti

	# init FDC
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

	# set CCR
	movw	$0x03F7,%dx
	movb	$0x00,%al
	outb	%al,%dx

	# SPECIFY
	

	# print a '?'
	movw	$0x0E3F,%ax
	movw	$0x0007,%bx
	int	$0x10

	# halt the CPU
	cli
	hlt
