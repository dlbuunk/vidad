	.text
	.global	fs_entry
fs_entry:
	# alloc table
	.word	0xFFFC
	.word	0xFFFC
	.word	0xFFF8
	.word	0xFFF8
	.fill	2044,2

	# root dir
	.quad	2
	.ascii	"."
	.fill	23

	.quad	3
	.ascii	"kernel.bin"
	.fill	14

	.fill	126<<5
