#       kheader.s
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

	# first the file header
	# number of bytes in file header
	# 0x00
	.word	0x0020
	.word	0
	.long	0
	.quad	0
	# 0x10
	.quad	0
	.quad	0

	# then the kernel header
	# 0x20
	.extern	page_init
	.long	page_init	# entry point
	.ascii	"ViOS"		# magic
	.byte	0x00		# major version
	.byte	0x01		# minor version
	.byte	0x00		# minor-minor version
	.byte	0x01		# number of blocks in page-init code
	.long	0x00000010	# number of blocks in kernel proper
	# STACK			# in memory, not on disk
	# 0x30
