#       boot_block.s
#
#       Copyright 2011,2012 D.L.Buunk <dlbuunk@gmail.com>
#
#       This file is part of ViOS.
#
#       ViOS is free software; you can redistribute it and/or modify
#       it under the terms of the GNU General Public License as published by
#       the Free Software Foundation; either version 2 of the License, or
#       (at your option) any later version.
#
#       ViOS is distributed in the hope that it will be useful,
#       but WITHOUT ANY WARRANTY; without even the implied warranty of
#       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#       GNU General Public License for more details.
#
#       You should have received a copy of the GNU General Public License
#       along with ViOS; if not, write to the Free Software
#       Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#       MA 02110-1301, USA.

# outline of this file:
# first thre



	.text
	# each function in this file should specify .code16 or .code32 !
	.global boot_block_entry

	# segment:offset pointer to start of code + padding
	.word	boot_block_entry
	.word	0x0000
	.long	0
	.quad	0

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

	# other INT-handlers and helper functions come here

	# irq 0 handler
	.global	irq0
irq0:
	.code32
	pushl	%eax
	movb	$0x01,irq0f
	movb	$0x20,%al
	outb	%al,$0x20
	popl	%eax
	iretl
irq0f:	.byte	0x00

	.global	wait_irq0
wait_irq0:
	hlt
	cmpb	$0x01,irq0f
	jne	wait_irq0
	movb	$0x00,irq0f
	ret

	# irq 6 handler
	.global	irq6
irq6:
	.code32
	pushl	%eax
	movb	$0x01,irq6f
	movb	$0x20,%al
	outb	%al,$0x20
	popl	%eax
	iretl
irq6f:	.byte	0x00

	.global	wait_irq6
wait_irq6:
	hlt
	cmpb	$0x01,irq6f
	jne	wait_irq6
	movb	$0x00,irq6f
	ret

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

	# real-mode irq-handling and such
IRQ0:
	.code16
	pushw	%ax
	movb	$0x01,IRQ0.f
	movb	$0x20,%al
	outb	%al,$0x20
	popw	%ax
	iretw
IRQ0.f:
	.byte	0x00
timer:
	.code16
	hlt
	cmpb	$0x01,IRQ0.f
	jne	timer
	movb	$0x00,IRQ0.f
	loop	timer
	ret

readFIFO:
	.code16
	movw	$0x03F4,%dx
	inb	%dx,%al
	andb	$0x80,%al
	cmpb	$0x80,%al
	jne	readFIFO
	incw	%dx
	inb	%dx,%al
	ret

	# Main loader code starts here.
	# Tasks for the main loader:
	# => Shut down hw
	# => Check CPU type
	# => Obtain system data from BIOS, and store it.
	# => A20-gate.
	# => Setup IDT, and copy GDT.
	# => Prepare and swich to pmode.
boot_block_entry:
	.data
msg_cpu:
	.asciz	"\n\rViOS boot-block loaded.\n\rCPU     "
msg_cpu_error:
	.asciz	"FAIL"
	.text
	.code16

	call	readFIFO
	call	readFIFO
	call	readFIFO
	call	readFIFO
	call	readFIFO

	# print A
	movw	$0x0E41,%ax
	int	$0x10

	# shut off the floppy motor
	movw	$0x03F2,%dx
	movb	$0x0C,%al
	outb	%al,%dx
	movw	$0x0005,%cx
	call	timer
	cli

	# and shut off the entire FDC
	movb	$0x00,%al
	outb	%al,%dx

	# why not kill the DMAC while we're at it?
	movb	$0x0F,%al	# mask all channels
	outb	%al,$0x0F
	outb	%al,$0xDE
	movb	$0x04,%al	# kill both DMACs
	outb	%al,$0x08
	outb	%al,$0xD0

	# and mask the PIC, for good measure
	movb	$0xFF,%al
	outb	%al,$0x21

	# print D
	movw	$0x0E44,%ax
	int	$0x10

	# check CPU type
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
	.asciz	"OK\r\nMemory  "
	.text
	.code16
	movw	$msg_memory,%si
	call	print_r

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

	# tell everyone that it went OK, and go A20
	.data
msg_a20:
	.asciz	"OK\r\nA20     "
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
	.asciz	"OK\r\nPmode   "
	.text
	.code16
	movw	$msg_pmode,%si
	call	print_r
	cli	# i know, i am paranoid

	# set up IDT
	movw	$0x0800,%di
idt_loop:
	movw	$0x1270,(%di)	# offset low
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
	movw	$0x1240,%si
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


	# now we can call the loader, first do the hardware part
	.extern	loader_hw
	call	loader_hw

	cli
	hlt
