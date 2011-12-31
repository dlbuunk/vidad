#       kernel_entry.s
#       
#       Copyright 2011 D.L.Buunk <dlbuunk@gmail.com>
#       
#       This program is free software; you can redistribute it and/or modify
#       it under the terms of the GNU General Public License as published by
#       the Free Software Foundation; either version 2 of the License, or
#       (at your option) any later version.
#
#       This program is distributed in the hope that it will be useful,
#       but WITHOUT ANY WARRANTY; without even the implied warranty of
#       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#       GNU General Public License for more details.
#
#       You should have received a copy of the GNU General Public License
#       along with this program; if not, write to the Free Software
#       Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#       MA 02110-1301, USA.

	.text
	.code32
	.global __kernel_entry
	# This must be the first instruction of the kernel proper,
	# page_init calls right into it.
	# The goal of this code is to init the int-handlers,
	# and then to call the next part of the startup code.
__kernel_entry:
	pushl	%ebp
	movl	%esp,%ebp
	movl	8(%ebp),%eax
	# The main stack resides at 0x0010F000.
	# At 0x0011000 there is a TSS-stack for interrupts.
	movl	$0x10F000,%esp	# STACK
	pushl	%ebp
	movl	%esp,%ebp
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	pushl	%eax

	# setup the TSS
	movw	$0x0067,0x10040
	movl	$__tss_0,%eax
	movw	%ax,0x10042
	shrl	$16,%eax
	movb	%al,0x10044
	movb	$0x89,0x10045
	movb	$0x40,0x10046
	movb	%ah,0x10047
	movw	$0x0040,%ax
	ltr	%ax

	# setup the int's
	movl	$0x0800,%edi

	movl	$__int_0,%ebx
	call	__install_int
	movl	$__int_1,%ebx
	call	__install_int
	movl	$__int_2,%ebx
	call	__install_int
	movl	$__int_3,%ebx
	call	__install_int
	movl	$__int_4,%ebx
	call	__install_int
	movl	$__int_5,%ebx
	call	__install_int
	movl	$__int_6,%ebx
	call	__install_int
	movl	$__int_7,%ebx
	call	__install_int
	movl	$__int_8,%ebx
	call	__install_int
	movl	$__int_9,%ebx
	call	__install_int
	movl	$__int_A,%ebx
	call	__install_int
	movl	$__int_B,%ebx
	call	__install_int
	movl	$__int_C,%ebx
	call	__install_int
	movl	$__int_D,%ebx
	call	__install_int
	movl	$__int_E,%ebx
	call	__install_int
	movl	$__int_F,%ebx
	call	__install_int
	movl	$__int_10,%ebx
	call	__install_int
	movl	$__int_11,%ebx
	call	__install_int
	movl	$__int_12,%ebx
	call	__install_int
	movl	$__int_13,%ebx
	call	__install_int
	movl	$__int_14,%ebx
	call	__install_int
	movl	$__int_15,%ebx
	call	__install_int
	movl	$__int_16,%ebx
	call	__install_int
	movl	$__int_17,%ebx
	call	__install_int
	movl	$__int_18,%ebx
	call	__install_int
	movl	$__int_19,%ebx
	call	__install_int
	movl	$__int_1A,%ebx
	call	__install_int
	movl	$__int_1B,%ebx
	call	__install_int
	movl	$__int_1C,%ebx
	call	__install_int
	movl	$__int_1D,%ebx
	call	__install_int
	movl	$__int_1E,%ebx
	call	__install_int
	movl	$__int_1F,%ebx
	call	__install_int
	movl	$__int_20,%ebx
	call	__install_int
	movl	$__int_21,%ebx
	call	__install_int
	movl	$__int_22,%ebx
	call	__install_int
	movl	$__int_23,%ebx
	call	__install_int
	movl	$__int_24,%ebx
	call	__install_int
	movl	$__int_25,%ebx
	call	__install_int
	movl	$__int_26,%ebx
	call	__install_int
	movl	$__int_27,%ebx
	call	__install_int
	movl	$__int_28,%ebx
	call	__install_int
	movl	$__int_29,%ebx
	call	__install_int
	movl	$__int_2A,%ebx
	call	__install_int
	movl	$__int_2B,%ebx
	call	__install_int
	movl	$__int_2C,%ebx
	call	__install_int
	movl	$__int_2D,%ebx
	call	__install_int
	movl	$__int_2E,%ebx
	call	__install_int
	movl	$__int_2F,%ebx
	call	__install_int
	movl	$__int_30,%ebx
	call	__install_int
	movl	$__int_31,%ebx
	call	__install_int
	movl	$__int_32,%ebx
	call	__install_int
	movl	$__int_33,%ebx
	call	__install_int
	movl	$__int_34,%ebx
	call	__install_int
	movl	$__int_35,%ebx
	call	__install_int
	movl	$__int_36,%ebx
	call	__install_int
	movl	$__int_37,%ebx
	call	__install_int
	movl	$__int_38,%ebx
	call	__install_int
	movl	$__int_39,%ebx
	call	__install_int
	movl	$__int_3A,%ebx
	call	__install_int
	movl	$__int_3B,%ebx
	call	__install_int
	movl	$__int_3C,%ebx
	call	__install_int
	movl	$__int_3D,%ebx
	call	__install_int
	movl	$__int_3E,%ebx
	call	__install_int
	movl	$__int_3F,%ebx
	call	__install_int

	# limit the IDT to 0x40 entries
	movw	$0x0200,0x2E0A
	movl	$0x0800,0x2E0C
	lidt	0x2E0A

	# call the C entry point
	.extern	__c_entry
	call	__c_entry
	popl	%eax

	popl	%ebx
	popl	%esi
	popl	%edi
	movl	%ebp,%esp
	popl	%ebp
	movl	%ebp,%esp
	popl	%ebp
	ret

__install_int:
	movl	$0x00200000,%eax
	movw	%bx,%ax
	movw	$0x8E00,%bx
	stosl
	movl	%ebx,%eax
	stosl
	ret

__int_handler:
	pushl	%ds
	pushl	%es
	pushl	%ebp
	pushl	%edi
	pushl	%esi
	pushl	%edx
	pushl	%ecx
	pushl	%ebx
	pushl	%eax
	pushl	%esp
	.extern	__int_entry
	call	__int_entry
	popl	%esp
	popl	%eax
	popl	%ebx
	popl	%ecx
	popl	%edx
	popl	%esi
	popl	%edi
	popl	%ebp
	popl	%es
	popl	%ds
	ret

__tss_0:
	.long	0
	.long	0x00110000	# STACK
	.word	0
	.word	0x0028
	.quad	0	# ring 1
	.quad	0	# ring 2
	.long	0	# cr3
	.quad	0	# ip/flags
	.quad	0	# ax/cx
	.quad	0	# dx/bx
	.quad	0	# sp/bp
	.quad	0	# si/di
	.quad	0	# es/cs
	.quad	0	# ss/ds
	.quad	0	# fs/gs
	.long	0	# ldtr
	.word	0x0068	# sizeof(TSS)
	.word	0	# reserved

__int_0:
	pushl	$0
	call	__int_handler
	addl	$4,%esp
	iretl

__int_1:
	pushl	$1
	call	__int_handler
	addl	$4,%esp
	iretl

__int_2:
	pushl	$2
	call	__int_handler
	addl	$4,%esp
	iretl

__int_3:
	pushl	$3
	call	__int_handler
	addl	$4,%esp
	iretl

__int_4:
	pushl	$4
	call	__int_handler
	addl	$4,%esp
	iretl

__int_5:
	pushl	$5
	call	__int_handler
	addl	$4,%esp
	iretl

__int_6:
	pushl	$6
	call	__int_handler
	addl	$4,%esp
	iretl

__int_7:
	pushl	$7
	call	__int_handler
	addl	$4,%esp
	iretl

__int_8:
	pushl	$8
	call	__int_handler
	addl	$8,%esp
	iretl

__int_9:
	pushl	$9
	call	__int_handler
	addl	$4,%esp
	iretl

__int_A:
	pushl	$0xA
	call	__int_handler
	addl	$8,%esp
	iretl

__int_B:
	pushl	$0xB
	call	__int_handler
	addl	$8,%esp
	iretl

__int_C:
	pushl	$0xC
	call	__int_handler
	addl	$8,%esp
	iretl

__int_D:
	pushl	$0xD
	call	__int_handler
	addl	$8,%esp
	iretl

__int_E:
	pushl	$0xE
	call	__int_handler
	addl	$8,%esp
	iretl

__int_F:
	pushl	$0xF
	call	__int_handler
	addl	$4,%esp
	iretl

__int_10:
	pushl	$0x10
	call	__int_handler
	addl	$4,%esp
	iretl

__int_11:
	pushl	$0x11
	call	__int_handler
	addl	$8,%esp
	iretl

__int_12:
	pushl	$0x12
	call	__int_handler
	addl	$4,%esp
	iretl

__int_13:
	pushl	$0x13
	call	__int_handler
	addl	$4,%esp
	iretl

__int_14:
	pushl	$0x14
	call	__int_handler
	addl	$4,%esp
	iretl

__int_15:
	pushl	$0x15
	call	__int_handler
	addl	$4,%esp
	iretl

__int_16:
	pushl	$0x16
	call	__int_handler
	addl	$4,%esp
	iretl

__int_17:
	pushl	$0x17
	call	__int_handler
	addl	$4,%esp
	iretl

__int_18:
	pushl	$0x18
	call	__int_handler
	addl	$4,%esp
	iretl

__int_19:
	pushl	$0x19
	call	__int_handler
	addl	$4,%esp
	iretl

__int_1A:
	pushl	$0x1A
	call	__int_handler
	addl	$4,%esp
	iretl

__int_1B:
	pushl	$0x1B
	call	__int_handler
	addl	$4,%esp
	iretl

__int_1C:
	pushl	$0x1C
	call	__int_handler
	addl	$4,%esp
	iretl

__int_1D:
	pushl	$0x1D
	call	__int_handler
	addl	$4,%esp
	iretl

__int_1E:
	pushl	$0x1E
	call	__int_handler
	addl	$4,%esp
	iretl

__int_1F:
	pushl	$0x1F
	call	__int_handler
	addl	$4,%esp
	iretl

__int_20:
	pushl	$0x20
	call	__int_handler
	addl	$4,%esp
	iretl

__int_21:
	pushl	$0x21
	call	__int_handler
	addl	$4,%esp
	iretl

__int_22:
	pushl	$0x22
	call	__int_handler
	addl	$4,%esp
	iretl

__int_23:
	pushl	$0x23
	call	__int_handler
	addl	$4,%esp
	iretl

__int_24:
	pushl	$0x24
	call	__int_handler
	addl	$4,%esp
	iretl

__int_25:
	pushl	$0x25
	call	__int_handler
	addl	$4,%esp
	iretl

__int_26:
	pushl	$0x26
	call	__int_handler
	addl	$4,%esp
	iretl

__int_27:
	pushl	$0x27
	call	__int_handler
	addl	$4,%esp
	iretl

__int_28:
	pushl	$0x28
	call	__int_handler
	addl	$4,%esp
	iretl

__int_29:
	pushl	$0x29
	call	__int_handler
	addl	$4,%esp
	iretl

__int_2A:
	pushl	$0x2A
	call	__int_handler
	addl	$4,%esp
	iretl

__int_2B:
	pushl	$0x2B
	call	__int_handler
	addl	$4,%esp
	iretl

__int_2C:
	pushl	$0x2C
	call	__int_handler
	addl	$4,%esp
	iretl

__int_2D:
	pushl	$0x2D
	call	__int_handler
	addl	$4,%esp
	iretl

__int_2E:
	pushl	$0x2E
	call	__int_handler
	addl	$4,%esp
	iretl

__int_2F:
	pushl	$0x2F
	call	__int_handler
	addl	$4,%esp
	iretl

__int_30:
	pushl	$0x30
	call	__int_handler
	addl	$4,%esp
	iretl

__int_31:
	pushl	$0x31
	call	__int_handler
	addl	$4,%esp
	iretl

__int_32:
	pushl	$0x32
	call	__int_handler
	addl	$4,%esp
	iretl

__int_33:
	pushl	$0x33
	call	__int_handler
	addl	$4,%esp
	iretl

__int_34:
	pushl	$0x34
	call	__int_handler
	addl	$4,%esp
	iretl

__int_35:
	pushl	$0x35
	call	__int_handler
	addl	$4,%esp
	iretl

__int_36:
	pushl	$0x36
	call	__int_handler
	addl	$4,%esp
	iretl

__int_37:
	pushl	$0x37
	call	__int_handler
	addl	$4,%esp
	iretl

__int_38:
	pushl	$0x38
	call	__int_handler
	addl	$4,%esp
	iretl

__int_39:
	pushl	$0x39
	call	__int_handler
	addl	$4,%esp
	iretl

__int_3A:
	pushl	$0x3A
	call	__int_handler
	addl	$4,%esp
	iretl

__int_3B:
	pushl	$0x3B
	call	__int_handler
	addl	$4,%esp
	iretl

__int_3C:
	pushl	$0x3C
	call	__int_handler
	addl	$4,%esp
	iretl

__int_3D:
	pushl	$0x3D
	call	__int_handler
	addl	$4,%esp
	iretl

__int_3E:
	pushl	$0x3E
	call	__int_handler
	addl	$4,%esp
	iretl

__int_3F:
	pushl	$0x3F
	call	__int_handler
	addl	$4,%esp
	iretl
