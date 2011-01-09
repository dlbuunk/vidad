#  This file is part of Vidad.
#
#  Vidad is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  Vidad is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with Vidad.  If not, see <http://www.gnu.org/licenses/>.

	# kernel header
	# located at 0x8000
	.ascii	"VIDADB"	# magic
	.byte	0x00		# video mode
	.byte	0x00		# cpu type
	.word	0x0000		# low mem
	.word	0x0000		# mid mem
	.word	0x0000		# high mem
	.word	0x0000		# alt mem
	.byte	0x00		# kernel version
	.byte	0x01		# kernel subversion
	.word	0x0000		# reserved
	.long	0x00000000	# kernel size in bytes
	.word	0x0080		# GDT start
	.word	0x0080		# GDT length
	.word	0x0100		# IDT start
	.word	0x0200		# IDT length
	.long	0x00008000	# image load address
	.long	0x00008500	# run start address
	.long	0x00000000	# 32-bit modular sum
	.long	0x00000000	# 32-bit checksum of image
	.quad	0,0
	.quad	0,0,0,0,0,0,0,0
	# total of 0x80 bytes
