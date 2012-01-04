	.text
	.global	fs_entry
fs_entry:
	# alloc table
	.word	0xFFFC	# boot block
	.word	0xFFF8	# allocation table
	.word	0xFFF8	# root directory
	.word	0x0004	# kernel.bin
	.word	0x0005	# 4 blocks, 16 KiB
	.word	0x0006
	.word	0x0007
	.word	0xFFF8
	.fill	2040,2

	# root dir header
	.word	32
	.word	0
	.long	0
	.quad	0
	.quad	0
	.quad	0

	# root dir
	.quad	2
	.ascii	"."
	.fill	23

	.quad	3
	.ascii	"kernel.bin"
	.fill	14

	.fill	125<<5
