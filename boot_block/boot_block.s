# Layout of boot block:
#
# The boot block is supposed to be loaded at phys 0x00001000.
# First 512 kb: boot sector.
# Then a table containg info about medium and bootloader:
# -------------------------------------------------------
# 0x200 | ascii | "VIOS"
# 0x204 | byte  | type, 0x41 for standard floppy
# 0x205 | byte  | number of heads
# 0x206 | byte  | number of tracks
# 0x207 | byte  | number of sectors
# 0x208 | qword | grand number of blocks
# 0x210 | qword | start of alloc table (block 1)
# 0x218 | qword | start of root dir (block 2)
# 0x220 | 23 dw | reserved
# 0x27C | word  | start of code (offset)
# 0x27E | word  | start of code (segment)
# 0x280 | 8 qw  | GDT
# 0x2C0 | 8 qw  | reserved
#
# Thus, 0.75 kb into the boot block, the loader code starts.

	.text
	.code16
	.global boot_block_entry

	# main table
	.ascii	"VIOS"
	.byte	0x41
	.byte	0x02
	.byte	0x50
	.byte	0x12
	.quad	0x0000000000000168
	.quad	0x0000000000000001
	.quad	0x0000000000000002
	# 0x20
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	# 0x40
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	# 0x60
	.quad	0
	.quad	0
	.quad	0
	.long	0
	# 0x7C
	.word	boot_block_entry
	.word	0x0000
	# 0x80

	# start of GDT
	# null entry
	.quad	0

	# system use LDT
	.quad	0

	# 16-bit loader code
	.word	0x0000	# base 0:15
	.word	0xFFFF	# limit	0:15
	.byte	0x00	# base 16:23
	.byte	0x99	# present, ring 0, non-system, executable,
			# readable, not-accessed
	.byte	0x00	# byte-granular, 16-bit, limit 16:19
	.byte	0x00	# base 24:31

	# 16-bit loader code
	.word	0x0000	# base 0:15
	.word	0xFFFF	# limit	0:15
	.byte	0x00	# base 16:23
	.byte	0x92	# present, ring 0, non-system, executable,
			# readable, not-accessed
	.byte	0x00	# byte-granular, 16-bit, limit 16:19
	.byte	0x00	# base 24:31

	# 32-bit kernel code
	.word	0x0000	# base 0:15
	.word	0xFFFF	# limit	0:15
	.byte	0x00	# base 16:23
	.byte	0x99	# present, ring 0, non-system, executable,
			# readable, not-accessed
	.byte	0xCF	# page-granular, 32-bit, limit 16:19
	.byte	0x00	# base 24:31

	# 32-bit kernel data
	.word	0x0000	# base 0:15
	.word	0xFFFF	# limit	0:15
	.byte	0x00	# base 16:23
	.byte	0x92	# present, ring 0, non-system, non-executable,
			# writeable, not-accessed
	.byte	0xCF	# page-granular, 32-bit, limit 16:19
	.byte	0x00	# base 24:31

	# 32-bit user code
	.word	0x0000	# base 0:15
	.word	0xFFFF	# limit	0:15
	.byte	0x00	# base 16:23
	.byte	0xF9	# present, ring 3, non-system, executable,
			# readable, not-accessed
	.byte	0xCF	# page-granular, 32-bit, limit 16:19
	.byte	0x00	# base 24:31

	# 32-bit user data
	.word	0x0000	# base 0:15
	.word	0xFFFF	# limit	0:15
	.byte	0x00	# base 16:23
	.byte	0xF2	# present, ring 3, non-system, non-executable,
			# writeable, not-accessed
	.byte	0xCF	# page-granular, 32-bit, limit 16:19
	.byte	0x00	# base 24:31

	# first, the default EXCEP handler
	cli
	movw	$0x0028,%ax
	movw	%ax,%es
	movl	$0x000B8000,%ebx
	movw	$excep_msg,%si
exp_l:	lodsb
	cmpb	$0x00,%al
	je	exp_e
	movb	%al,(%ebx)
	inc	%bx
	jmp	exp_l
exp_e:
	hlt
excep_msg:
	.asciz	"U n h a n d l e d   e x c e p t i o n . "

	# other INT-handlers and helper functions come here

boot_block_entry:
	hlt
