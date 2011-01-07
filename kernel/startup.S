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

	# continue bootloader process by doing checksums and relocating BIOS/CPU info
	# this assumes that the first 5 records are read OK (in-bootloader assumed only the first 3)
	mov	0x00008014,%ecx
	shr	$0x02,%ecx
	xor	%eax,%eax
	mov	$0x00008000,%esi
sum:	add	(%esi),%eax
	add	$0x04,%esi
	loop	sum
	cmp	$0x00000000,%eax
	jne	checksum_error

	mov	0x00008014,%ecx
	sub	$0x00000080,%ecx
	shr	$0x02,%ecx
	xor	%eax,%eax
	mov	$0x00008080,%esi
csum:	ror	%eax
	add	(%esi),%eax
	add	$0x04,%esi
	loop	csum
	cmp	0x0000802C,%eax
	jne	checksum_error

	mov	$0x00006006,%esi
	mov	$0x00008006,%edi
	mov	$0x0000000A,%cx
	rep	movsb

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

checksum_error:
	mov	$0x4F,%ah
	mov	$0x0000000E,%ecx
	mov	$error_msg,%esi
	mov	$0x000B8000,%edi
print:	lodsb
	stosw
	loop	print
	mov	$0x20,%al
	mov	$0x000007C2,%ecx
	rep	stosw
	hlt

error_msg:
	.ascii	"CHECKSUM ERROR"

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
