# Layout of boot block:
#
# The boot block is supposed to be loaded at phys 0x00001000.
# First 512 kb: boot sector.
# Then a table containg info about medium and bootloader:
# -------------------------------------------------------
# 0x200 | ascii | "VIOS"
# 0x204 | byte  | type, 0x41 for standard floppy
# 0x205 | byte  | number of heads
# 0x206 | byte  | number of tracks
# 0x207 | byte  | number of sectors
# 0x208 | qword | grand number of blocks
# 0x210 | qword | start of alloc table (block 1)
# 0x218 | qword | start of root dir (block 2)
# 0x220 | 23 dw | reserved
# 0x27C | word  | start of code (offset)
# 0x27E | word  | start of code (segment)
# 0x280 | 8 qw  | GDT
# 0x2C0 | 8 qw  | reserved
#
# Thus, 0.75 kb into the boot block, the loader code starts.

	.text
	# each function in this file should specify .code16 or .code32 !
	.global boot_block_entry

	# main table
	.ascii	"VIOS"
	.byte	0x41
	.byte	0x02
	.byte	0x50
	.byte	0x12
	.quad	0x0000000000000168
	.quad	0x0000000000000001
	.quad	0x0000000000000002
	# 0x20
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	# 0x40
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	# 0x60
	.quad	0
	.quad	0
	.quad	0
	.long	0
	# 0x7C
	.word	boot_block_entry
	.word	0x0000
	# 0x80

	# start of GDT
	# null entry
	.quad	0

	# system use LDT
	.quad	0

	# 16-bit loader code
	.word	0xFFFF	# limit	0:15
	.word	0x0000	# base 0:15
	.byte	0x00	# base 16:23
	.byte	0x99	# present, ring 0, non-system, executable,
			# readable, not-accessed
	.byte	0x00	# byte-granular, 16-bit, limit 16:19
	.byte	0x00	# base 24:31

	# 16-bit loader code
	.word	0xFFFF	# limit	0:15
	.word	0x0000	# base 0:15
	.byte	0x00	# base 16:23
	.byte	0x92	# present, ring 0, non-system, executable,
			# readable, not-accessed
	.byte	0x00	# byte-granular, 16-bit, limit 16:19
	.byte	0x00	# base 24:31

	# 32-bit kernel code
	.word	0xFFFF	# limit	0:15
	.word	0x0000	# base 0:15
	.byte	0x00	# base 16:23
	.byte	0x99	# present, ring 0, non-system, executable,
			# readable, not-accessed
	.byte	0xCF	# page-granular, 32-bit, limit 16:19
	.byte	0x00	# base 24:31

	# 32-bit kernel data
	.word	0xFFFF	# limit	0:15
	.word	0x0000	# base 0:15
	.byte	0x00	# base 16:23
	.byte	0x92	# present, ring 0, non-system, non-executable,
			# writeable, not-accessed
	.byte	0xCF	# page-granular, 32-bit, limit 16:19
	.byte	0x00	# base 24:31

	# 32-bit user code
	.word	0xFFFF	# limit	0:15
	.word	0x0000	# base 0:15
	.byte	0x00	# base 16:23
	.byte	0xF9	# present, ring 3, non-system, executable,
			# readable, not-accessed
	.byte	0xCF	# page-granular, 32-bit, limit 16:19
	.byte	0x00	# base 24:31

	# 32-bit user data
	.word	0xFFFF	# limit	0:15
	.word	0x0000	# base 0:15
	.byte	0x00	# base 16:23
	.byte	0xF2	# present, ring 3, non-system, non-executable,
			# writeable, not-accessed
	.byte	0xCF	# page-granular, 32-bit, limit 16:19
	.byte	0x00	# base 24:31

	# 8 reserved qwords
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0

	# first, the default EXCEP handler (point all IDT entries here)
	.code32
	cli
	movw	$0x0028,%ax
	movw	%ax,%es
	movl	$0x000B8000,%ebx
	movw	$exp_msg,%si
exp_l:	lodsb
	cmpb	$0x00,%al
	je	exp_e
	movb	%al,(%ebx)
	inc	%bx
	jmp	exp_l
exp_e:
	hlt
exp_msg:
	.asciz	"U n h a n d l e d   e x c e p t i o n . "

	# other INT-handlers and helper functions come here

	# real-mode BIOS-printing, %si is pointer to string to print
print_r:
	.code16
	pushw	%ax
	pushw	%bx
	movb	$0x0E,%ah
	movw	$0x0007,%bx
prr_s:	lodsb
	cmpb	$0x00,%al
	je	prr_e
	int	$0x10
	jmp	prr_s
prr_e:	popw	%bx
	popw	%ax
	ret

	# Main loader code starts here.
	# Tasks for the main loader:
	# => Check CPU type
	# => Obtain system data from BIOS, and store it.
	# => A20-gate.
	# => Setup IDT, and copy GDT.
	# => Prepare and swich to pmode.
	# => Re-init some hardware (PIC, PIT, DMAC, FDC)
boot_block_entry:
	.data
msg_cpu:
	.asciz	"\n\rViOS boot-block loaded.\n\rChecking CPU        "
msg_cpu_error:
	.asciz	"FAIL\n\rViOS cannot run on this CPU, aborting."
	.text
	.code16
	movw	$msg_cpu,%si
	call	print_r
	pushfw

	pushfw
	popw	%ax
	movw	%ax,%cx
	andw	$0x0FFF,%ax
	pushw	%ax
	popfw
	pushfw
	popw	%ax
	and	$0xF000,%ax
	cmp	$0xF000,%ax
	jne	check_286
	popfw
	movw	$msg_cpu_error,%si
	call	print_r
	cli
	hlt

check_286:
	orw	$0xF000,%cx
	pushw	%cx
	popfw
	pushfw
	popw	%ax
	and	$0xF000,%ax
	jnz	check_386
	popfw
	movw	$msg_cpu_error,%si
	call	print_r
	cli
	hlt

check_386:
	popfw
	movb	$0x03,0x2DFE
	# we have a 386, and are happy with it (for now)

	# check memory && other BIOS data
	.data
msg_memory:
	.asciz	"OK\r\nChecking memory     "
	.text
	.code16
	movw	$msg_memory,%si
	call	print_r

	# first try int 0x15, %eax = 0x0000E82
	movw	$0x2C00,%di
	movl	$0x0000E820,%eax
	movl	$0x00000000,%ebx
	movl	$0x00000020,%ecx
	movl	$0x534D4150,%edx
	int	$0x15
	jc	memm_err
	cmpl	$0x534D4150,%eax
	jne	memm_err
memm_loop:
	addw	$0x20,%di
	movl	$0x0000E820,%eax
	movl	$0x00000020,%ecx
	int	$0x15
	jc	memm_errc
	cmpl	$0x00000000,%ebx
	je	memm_end
	jmp	memm_loop
memm_errc:
	clc
	subw	$0x0020,%di
	jmp	memm_end
memm_err:
	clc
	movw	$0x2C00,%di
memm_end:
	movw	%di,%ax
	subw	$0x2C00,%ax
	movb	$0x20,%cl
	divb	%cl
	incb	%al
	movb	%al,0x2DFF

	# now try int 0x15, %ax = 0xE801
	xorw	%cx,%cx
	xorw	%dx,%dx
	movw	$0xE801,%ax
	int	$0x15
	jc	mem_err
	cmpb	$0x86,%ah
	je	mem_err
	cmpb	$0x80,%ah
	je	mem_err
	jcxz	mem_end
	movw	%cx,%ax
	movw	%dx,%bx
	jmp	mem_end
mem_err:
	clc
	movw	$0x0000,%ax
	movw	$0x0000,%bx
mem_end:
	movw	%bx,0x2DF4
	movw	%ax,0x2DF6

	# then try int 0x15, %ah = 0x88
	movb	$0x88,%ah
	int	$0x15
	jc	mema_err
	test	%ax,%ax
	je	mema_err
	cmp	$0x86,%ah
	je	mema_err
	cmp	$0x80,%ah
	je	mema_err
	jmp	mema_end
mema_err:
	clc
	movw	$0x0000,%ax
mema_end:
	movw	%ax,0x2DF8

	# finally, try int 0x12
	xorw	%ax,%ax
	int	$0x12
	jc	meml_err
	jmp	meml_end
meml_err:
	clc
	movw	$0x0000,%ax
meml_end:
	movw	%ax,0x2DFA

	# now, add the int 0x11 info
	xorw	%ax,%ax
	int	$0x11
	jc	hw_err
	jmp	hw_end
hw_err:
	clc
	movw	$0x0000,%ax
hw_end:
	movw	%ax,0x2DFC

	# tell everyone that it went OK, and go A20
	.data
msg_a20:
	.asciz	"OK\r\nEnabling A20        "
	.text
	.code16
	movw	msg_a20,%di
	call	print_r

	# clear the interrupts flag, it should already be down
	# but BIOS can do anything...
	cli
	jmp	a20

a20wait:
	inb	$0x64,%al
	test	$0x02,%al
	jnz	a20wait
	ret

a20wait2:
	inb	$0x64,%al
	test	$0x01,%al
	jz	a20wait2
	ret

a20:
	# disable keyboard
	call	a20wait
	movb	$0xAD,%al
	outb	%al,$0x64

	# read from input
	call	a20wait
	mov	$0xD0,%al
	outb	%al,$0x64
	call	a20wait2
	inb	$0x60,%al
	pushw	%ax

	# write to output
	call	a20wait
	movb	$0xD1,%al
	outb	%al,$0x64
	call	a20wait
	popw	%ax
	orb	$0x02,%al
	outb	%al,$0x60

	# wait for processing
	call	a20wait
	# and a20 should be enabled

	# preparing pmode
	.data
msg_pmode:
	.asciz	"OK\r\nSwitching to pmode  "
	.text
	.code16
	movw	$msg_pmode,%si
	call	print_r
	cli	# i know, i am paranoid

	# set up IDT
	movw	$0x0800,%di
idt_loop:
	movw	$0x1300,(%di)	# offset low
	incw	%di
	incw	%di
	movw	$0x0020,(%di)	# selector
	incw	%di
	incw	%di
	movw	$0x8E00,(%di)	# present, ring 0, 32-bit interrupt gate
	incw	%di
	incw	%di
	movw	$0x0000,(%di)	# offset high
	incw	%di
	incw	%di
	cmpw	$0x1000,%di
	jne	idt_loop

	# copy GDT
	movw	$0x1280,%si
	movw	$0x2000,%di
	movw	$0x0040,%cx	# 16 entries
	rep	movsw

	# setup GDTP && IDTP
	movw	$0x0040,0x2E02	# 16 GDT entries
	movw	$0x2000,0x2E04	# linear address of GDT low
	movw	$0x0000,0x2E06	# linear address of GDT high

	movw	$0x0800,0x2E0A	# 256 IDT entries
	movw	$0x0800,0x2E0C	# linear address of IDT low
	movw	$0x0000,0x2E0E	# linear address of IDT high

	# turn off NMI
	inb	$0x70,%al
	orb	$0x80,%al
	outb	%al,$0x70

	# switch to pmode
	smsw	%ax
	orb	$0x01,%al
	lgdt	0x2E02
	lmsw	%ax
	jmpw	$0x0010,$pmode16_entry

pmode16_entry:
	.code16
	# set up segment registers and stack
	movw	$0x0018,%ax
	movw	%ax,%ss
	movw	$0x3000,%sp
	movw	%sp,%bp
	movw	%ax,%ds
	# setup IVT
	lidt	0x2E0A
	# re-enable NMI
	inb	$0x70,%al
	andb	$0x7F,%al
	outb	%al,$0x70
	# and jump to the 32-bit segment
	jmpl	$0x0020,$pmode32_entry

pmode32_entry:
	.code32
	# setup segment registers and stack
	movw	$0x0028,%ax
	movw	%ax,%ss
	movl	$0x00003000,%esp
	movl	%esp,%ebp
	movw	%ax,%ds
	movw	%ax,%es
	movw	%ax,%fs
	movw	%ax,%gs

	cli
	hlt
