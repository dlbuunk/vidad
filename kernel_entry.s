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

	movl	%ebp,%esp
	popl	%ebp
	ret
