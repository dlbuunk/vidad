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
