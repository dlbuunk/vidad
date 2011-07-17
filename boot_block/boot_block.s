	.text
	.global _boot_block_init

	# general info
	.word	_boot_block_init	# void (*init)(void)
	.word	0x0014			# lenght of header
	.word	0x0000			# checksum of header (defunct)
	.word	0x0001			# type (standard floppy)
	# type-specific info
	.word	0x0050			# number of cylinders (80)
	.byte	0x02			# number of heads (2)
	.byte	0x12			# number of sectors (18)
	.word	0x0214			# start of alloc-table in boot-block
	.word	0xFFFF			# first block of index-blocks
	.word	0xFFFF			# first block of root-dir
	.word	0x0001			# first block of kernel (-loader)

	# allocation table
	.word	0xFFFC
	.word	0xFFF8

	.rept	360 - 2
	.word	0
	.endr

	# here come the IDT/GDT data
	.align	8,0
IDT:
	# one single entry (repeated)
	.word	_unhandled_int	# offset 15:0
	.word	0x0010		# selector
	.byte	0x00		# zero
	.byte	0x8E		# present, ring 0, gate, 32-bit interrupt
	.word	0x0000		# offset 31:16
GDT:
	# null entries (2, to create alignment)
	.quad	0
	.quad	0

	# 32-bit code (10)
	.word	0x01FF		# limit 15:0, 2 MiB
	.word	0x0000		# base 15:0
	.byte	0x00		# base 23:16
	.byte	0x9A		# access: present, ring 0, executable,
				# non-conforming, readable
	.byte	0xC0		# flags: page-granular, USE32
				# limit 19:16
	.byte	0x00		# base 31:24

	# 32-bit data (18)
	.word	0x01FF		# limit 15:0, 2 MiB
	.word	0x0000		# base 15:0
	.byte	0x00		# base 23:16
	.byte	0x92		# access: present, ring 0, non-executable,
				# expand-up, writeable
	.byte	0xC0		# flags: page-granular, USE32
				# limit 19:16
	.byte	0x00		# base 31:24

	# 16-bit code (20)
	.word	0xFFFF		# limit 15:0, 64 KiB
	.word	0x0000		# base 15:0
	.byte	0x00		# base 23:16
	.byte	0x9A		# access: present, ring 0, executable,
				# non-conforming, readable
	.byte	0x00		# flags: byte-granular, USE16
				# limit 19:16
	.byte	0x00		# base 31:24

	# 16-bit data (28)
	.word	0xFFFF		# limit 15:0, 64 KiB
	.word	0x0000		# base 15:0
	.byte	0x00		# base 23:16
	.byte	0x92		# access: present, ring 0, non-executable,
				# expand up, writeable
	.byte	0x00		# flags: byte-granular, USE16
				# limit 19:16
	.byte	0x00		# base 31:24

	# unhandled interrupt handler
	.code32
_unhandled_int:
	cli
	hlt

	# 16-bit code (realmode)
	# first the helper functions
	.code16

cpu_error:
	movw	$cpu_error_msg,%si
	movw	$0x0007,%bx
	movb	$0x0E,%ah
celoop:	movb	(%si),%al
	cmpb	$0x00,%al
	je	error_end
	int	$0x10
	incw	%si
	jmp	celoop
cpu_error_msg:
	.asciz	"Unsupported CPU"

error_end:
	cli
	hlt

	# main execution starts here
_boot_block_init:
	# at this point the system is "frozen":
	# all IRQ's are disabled (including NMI) so we won't be disturbed
	# now, we detect if we have a 386 or better
	# then we get memory size and such
	# finally, we run into pmode

	# viOs
	movw	$0x0E4F,%ax
	movw	$0x0007,%bx
	int	$0x10

	# CPU detection
	pushfw
	popw	%ax
	movw	%ax,%cx
	andw	$0x0FFF,%ax
	pushw	%ax
	popfw
	pushfw
	popw	%ax
	andw	$0xF000,%ax
	cmpw	$0xF000,%ax
	je	cpu_error
	orw	$0xF000,%cx
	pushw	%cx
	popfw
	pushfw
	popw	%ax
	andw	$0xF000,%ax
	jz	cpu_error
	# Okay, we have a 386 or better
	movb	$0x03,0x07FE

	# get memory size
	# BIOS INT 0x15 EAX=0x0000E820
	movw	$0x0600,%di
	xorl	%ebx,%ebx
	movl	$0x534D4150,%edx
	movl	$0x0000E820,%eax
	movl	$0x00000014,%ecx
	int	$0x15
	jc	msend
	cmpl	$0x534D4150,%eax
	jne	msend
	movb	$0x00,0x07FF
msloop:	cmpl	$0x00000000,%ebx
	je	msend
	incb	0x07FF
	addw	$0x0014,%di
	movl	$0x0000E820,%eax
	movl	$0x00000014,%ecx
	int	$0x15
	jc	msend
	jmp	msloop
msend:

	# BIOS INT 0x15 AX=0xE801
	xorw	%cx,%cx
	xorw	%dx,%dx
	movw	%cx,0x07F4
	movw	%cx,0x07F6
	movw	$0xE801,%ax
	int	$0x15
	jc	m2err
	cmpb	$0x86,%ah
	je	m2err
	cmpb	$0x80,%ah
	je	m2err
	jcxz	m2end
	movw	%cx,%ax
	movw	%dx,%bx
m2end:	movw	%ax,0x07F4
	movw	%bx,0x07F6
m2err:

	# BIOS INT 0x15 AH=0x88
	movw	$0x0000,0x07F8
	movb	$0x88,%ah
	int	$0x15
	jc	m3end
	testw	%ax,%ax
	je	m3end
	cmpb	$0x86,%ah
	je	m3end
	cmpb	$0x80,%ah
	je	m3end
	movw	%ax,0x07F8
m3end:

	# BIOS INT 0x12
	xorw	%ax,%ax
	movw	%ax,0x07FA
	int	$0x12
	jc	m4end
	movw	%ax,0x07FA
m4end:

	# BIOS INT 0x11
	xorw	%ax,%ax
	movw	%ax,0x07FC
	int	$0x11
	jc	hdend
	movw	%ax,0x07FC
hdend:

	# vioS
	movw	$0x0E53,%ax
	movw	$0x0007,%bx
	int	$0x10

	# all detection DONE, now prepare pmode
	# first, prepare IDT
	movw	$0x0100,%cx
	movw	$0x0800,%di
setup_idt:
	movw	$IDT,%si
	movsw
	movsw
	movsw
	movsw
	loop	setup_idt

	# then, prepare GDT
	movw	$0x0018,%cx
	movw	$0x2000,%di
	movw	$GDT,%si
	rep	movsw

	# prepare GDTP and IDTP
	movw	$0x0030,0xFF02
	movw	$0x2000,0xFF04
	movw	$0x0000,0xFF06

	movw	$0x0800,0xFF0A
	movw	$0x0800,0xFF0C
	movw	$0x0000,0xFF0E

	# start pmode
	smsw	%ax
	orb	$0x01,%al
	lgdt	0xFF02
	lmsw	%ax
	jmpw	$0x0020,$start_p16

	# this is 16-bit pmode
start_p16:
	movw	$0x0028,%ax
	movw	%ax,%ss
	movw	$0x0000,%sp
	movw	%sp,%bp
	movw	%ax,%ds
	movw	%ax,%es
	lidt	0xFF0A
	jmpl	$0x0010,$start_p32

	# NOW, we are in 32-bit pmode
	.code32
start_p32:
	movw	$0x0018,%ax
	movw	%ax,%ss
	movl	$0x00010000,%esp
	movl	%esp,%ebp
	movw	%ax,%ds
	movw	%ax,%es
	movw	%ax,%fs
	movw	%ax,%gs

	# Now, 32-bit pmode is fully set up.
	# print a char
	movw	$0x1F21,0x000B8000
	cli
	hlt

