	.text
	.code32

#include "header.inc"
#include "gdt.inc"
#include "idt.inc"
#include "handle_int.inc"

	.global exec_start
	.global handle_int

exec_start:
	# PMode execution starts HERE

	# setting ALL segment registers to appropriate values
	mov	$0x0010,%ax
	mov	%ax,%ss
	mov	%ax,%ds
	mov	%ax,%es
	mov	%ax,%fs
	mov	%ax,%gs

	# setting up stack
	mov	$0x00020000,%esp
	mov	%esp,%ebp

	# init FPU
	finit
	mov	$0x0000037F,%eax
	push	%eax
	fldcw	(%esp)
	pop	%eax

	# re-enable interrupts, PIC still masked out.
	in	$0x70,%al
	and	$0x7F,%al
	out	%al,$0x70
	sti

	# push stack base
	push	%ebp

	# pushing pointer to header
	mov	$0x00008000,%eax
	push	%eax

	call	kmain

	pop	%eax
	pop	%eax

	cli
	hlt

handle_int:
	# interrupts are handled here

	pusha
	and	$0x000000FF,%eax
	push	%eax

	call	inter_main

	pop	%eax
	popa
	pop	%eax
	iret
