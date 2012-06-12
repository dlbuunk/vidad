#	multiboot.s
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

#	Why loading according to the multiboot spec is a mess (for ViOS)
#
#	Grub can only load stuff above the 1MB mark, and places data,
#	usefull data (!) , in lower memory, at random places in lower memory :-(
#	So to do anything usefull, we'll have to move the grub-data to a _known_
#	and _reserved_ location in high memory (we'll use the bss for that).
#	After that we move the kernel "module" to lower mem (than free for use),
#	move the grub-data to lower mem, drop down to realmode,
#	and run the usual startup sequence.
#	This boot_block will have it's routines for reading data from disk
#	modified so that they just read data from the buffer.

	.text
	.code32
	.global multiboot_entry

	# multiboot header
	.long	0x1BADB002	# magic
	.long	0x00010001	# flags
	# align modules, no mem detection needed, no video mode, use aout kludge
	.long	0xE4514FFD	# checksum
	.long	0x00100000	# header load address
	.long	0x00100000	# code load adress (same)
	.long	0x00000000	# load end adress (filesize)
	.long	0x00110000	# end of BSS
	.long	multiboot_entry	# entry adress
	# end of header

multiboot_entry:
	# Okay, we are loaded multiboot, what to do next:
	# (This is going to be a _heck_ of a lot of copying stuff around...)
	# 1 => Copy all multiboot data to the 0x00101000-0x001FFFFF range.
	# 2 => Copy the gathered multiboot data to lower memory.
	# 3 => Copy the kernel and boot_block modules to lower memory.
	# 4 => Load a new, temporary, special GDT.
	# 5 => Switch to 16-bit pmode, and jump to boot block.

	# set up stack and clear some flags
	movl	$0x00110000,%esp
	movl	%esp,%ebp
	cli
	cld

	# push grub-given values, and call mb_data()
	pushl	%ebx
	pushl	%eax

	.extern	mb_data
	call	mb_data

	popl	%ebx
	popl	%ebx
	pushl	%eax

	# copy mb-data to low mem
	movl	%eax,%esi
	movl	$0x00008000,%edi
	movl	$0x00006000,%ecx
	rep	movsb

	# copy kernel
	popl	%eax
	movl	24(%eax),%ebx
	movl	(%ebx),%esi
	movl	4(%ebx),%ecx
	subl	%esi,%ecx
	shrl	$2,%ecx
	incl	%ecx
	movl	$0x00020000,%edi
	rep	movsl

	# copy self


	cli
	hlt
