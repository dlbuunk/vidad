	.text
	.code16
	.global boot_sect_entry
boot_sect_entry:

	# do a far jump to make sure cs:ip are where we expect them to be
	# nop, cli, cld is mostly for padding
	nop
	cli
	cld
	jmpw	$0x0000,$0x7C08	### MIND THE JMPF

	# set segment registers and stack
	xorw	%ax,%ax
	movw	%ax,%ss
	movw	%ax,%sp
	movw	%sp,%bp
	movw	%ax,%ds
	movw	%ax,%es

	# relocate the bootsector to 0x0000:0x0600
	movw	$0x7C00,%si
	movw	$0x0600,%di
	movw	$0x0100,%cx
	rep	movsw
	jmpw	$0x0000,$0x0640	### MIND THE JMPF

IRQ0:
	pushw	%ax
	movb	$0x01,IRQ0.f
	movb	$0x20,%al
	outb	%al,$0x20
	popw	%ax
	iretw
IRQ0.f:
	.word	0x00

IRQ6:
	pushw	%ax
	movb	$0x01,IRQ6.f
	movb	$0x20,%al
	outb	%al,$0x20
	popw	%ax
	iretw
IRQ6.f:
	.word	0x00


	# the actual start of the bootloader
	# print 'V'
	movw	$0x0E56,%ax
	movw	$0x0007,%bx
	int	$0x10

	# init the PIC
	movb	$0x11,%al	# init
	outb	%al,$0x20
	movb	$0x08,%al	# vector offset
	outb	%al,$0x21
	movb	$0x02,%al	# single
	outb	%al,$0x21
	movb	$0x01,%al	# 8086
	outb	%al,$0x21
	movb	$0xBE,%al	# mask
	outb	%al,$0x21

	# init the PIT
	movb	$0x34,%al	# chan 0, lo/hi, rate gen, binary
	outb	%al,$0x43
	movb	$0x0B,%al	# 20 Hz
	outb	%al,$0x40
	movb	$0xE9,%al
	outb	%al,$0x40

	# set the interrupt vectors
	movw	$IRQ0,0x0020
	movw	$0x00,0x0022
	movw	$IRQ6,0x0038
	movw	$0x00,0x003A
	sti

	# print I
	movw	$0x0E49,%ax
	int	$0x10

	# reset FDC
	movw	$0x03F2,%dx
	movb	$0x00,%al
	outb	%al,%dx

	movb	$0x0002,%cx
	call	timer
	movb	$0x00,IRQ6.f

	#init FDC
	movb	$0x0C,%al
	outb	%al,%dx
	call	waitfloppy

	# print D
	movw	$0x0E44,%ax
	int	$0x10

	#halt
	cli
	hlt

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
