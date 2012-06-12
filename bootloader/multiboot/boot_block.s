#	boot_block.s
#
#	Copyright 2012 D.L.Buunk <dlbuunk@gmail.com>
#
#	This file is part of ViOS.
#
#	ViOS is free software; you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation; either version 2 of the License, or
#	(at your option) any later version.
#
#	ViOS is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with ViOS; if not, write to the Free Software
#	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#	MA 02110-1301, USA.

	.text
	# each function in this file should specify .code16 or .code32 !
	.global boot_block_entry

	# multiboot header
	.long	0x1BADB002	# magic
	.long	0x00010001	# flags
	# align modules, no mem detection needed, no video mode, use aout kludge
	.long	0xE4514FFD	# checksum
	.long	0x00100000	# header load address
	.long	0x00100000	# code load adress (same)
	.long	0x00000000	# load end adress (filesize)
	.long	0x00107000	# end of BSS
	.long	0x00101000	# entry adress
	.long	0		# reserved
	.long	0
	.long	0
	# end of multiboot header

	# pointer to the entry point @ 0x0000102C
	.long	boot_block_entry
	# endp

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

	# first, the default EXCEP handler (point all IDT entries here)
	.code32
	cli
	movw	$0x0028,%ax
	movw	%ax,%ds
	movl	$0x000B8000,%ebx
	movl	$exp_msg,%esi
	movb	$0x20,%ah
exp_l:	lodsb
	cmpb	$0x00,%al
	je	exp_e
	movw	%ax,(%ebx)
	incl	%ebx
	incl	%ebx
	jmp	exp_l
exp_e:	hlt
exp_msg:
	.asciz	"Unhandled exception."

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
	# => Drop to realmode
	# => Obtain system data from BIOS, and store it.
	# => Setup IDT, and copy GDT.
	# => Prepare and swich to pmode.
	.data
msg_load:
	.asciz	"\n\rViOS boot-block loaded.\n\rMemory  "

	.text
boot_block_entry:
	.code16
	# This is 16-bit pmode, mind you!
	# All segment registers are invalid and we don't have a stack.
	movw	$0x0010,%ax
	movw	%ax,%ss
	movw	$0x3000,%sp
	movw	%sp,%bp
	movw	%ax,%ds
	movw	%ax,%es
	movw	%ax,%fs
	movw	%ax,%gs
	# Reset ALL segments to prevent triple-fault later on...
	# Now, drop to realmode:
	movl	%cr0,%eax
	andb	$0xFE,%al
	movl	%eax,%cr0
	# And do the required far jump:
	jmpw	$0x0000,$realmode_entry
realmode_entry:
	.code16

	# Now, we'll have to give BIOS the correct values for the cursor
	# position, first obtain the data from the VGA-registers.
	movw	$0x03D4,%dx
	movb	$0x0E,%al
	outb	%al,%dx
	inc	%dx
	inb	%dx,%al
	movb	%al,%ah
	dec	%dx
	movb	$0x0F,%al
	outb	%al,%dx
	inc	%dx
	inb	%dx,%al

	# calculate row/collumn and do the interrupt
	movb	$0x50,%cl
	divb	%cl
	rorw	$8,%ax
	movw	%ax,%dx
	movb	$0x00,%bh
	movb	$0x02,%ah
	int	$0x10

	# print "loaded" string
	movw	$msg_load,%si
	call	print_r

	# detect memory
	# first try int 0x15, %eax = 0x0000E82
	movw	$0x2C00,%di
	movl	$0x0000E820,%eax
	movl	$0x00000000,%ebx
	movl	$0x00000014,%ecx
	movl	$0x534D4150,%edx
	int	$0x15
	jc	memm_err
	cmpl	$0x534D4150,%eax
	jne	memm_err
memm_loop:
	addw	$0x14,%di
	movl	$0x0000E820,%eax
	movl	$0x00000014,%ecx
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

	# preparing pmode
	.data
msg_pmode:
	.asciz	"OK\r\nPmode   "
	.text
	.code16
	movw	$msg_pmode,%si
	call	print_r
	cli	# i know, i am paranoid

	# set up IDT
	movw	$0x0800,%di
idt_loop:
	movw	$0x1060,(%di)	# offset low
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
	movw	$0x1030,%si
	movw	$0x2000,%di
	movw	$0x0018,%cx	# 6 entries
	rep	movsw

	# setup GDTP && IDTP
	movw	$0x0030,0x2E02	# 6 GDT entries
	movw	$0x2000,0x2E04	# linear address of GDT low
	movw	$0x0000,0x2E06	# linear address of GDT high

	movw	$0x0800,0x2E0A	# 256 IDT entries
	movw	$0x0800,0x2E0C	# linear address of IDT low
	movw	$0x0000,0x2E0E	# linear address of IDT high

	# mask the PIC and turn off NMI
	movb	$0xFF,%al
	outb	%al,$0x21
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

	# now we can call the loader, first do the hardware part
#	.extern	loader_hw
#	call	loader_hw

	cli
	hlt
