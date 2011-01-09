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

	# global descriptor table
	# 16 entries
	# located at 0x8080
	# moved to 0x1000

	# null entry
	.quad	0

	# code segment
	.word	0x00FF		# limit 15:0, 1 megabyte
	.word	0x0000		# base 15:0
	.byte	0x00		# base 23:16
	.byte	0x9A		# access: present, ring 0, executable,
				# non-conforming, readable
	.byte	0xC0		# flags: page-granular, USE32
				# limit 19:16
	.byte	0x00		# base 31:24

	# data segment
	.word	0x00FF		# limit 15:0, 1 megabyte
	.word	0x0000		# base 15:0
	.byte	0x00		# base 23:16
	.byte	0x92		# access: present, ring 0, not-executable,
				# expand-up, writeable
	.byte	0xC0		# flags: page-granular, USE32
				# limit 19:16
	.byte	0x00		# base 31:24

	# 0x0D more null entries, reserved
	.quad	0
	.quad	0,0,0,0
	.quad	0,0,0,0,0,0,0,0

	# total of 0x80 bytes
