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
