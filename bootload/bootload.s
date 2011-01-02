	.text
	.code16
	.global bootload_entry
	. = 0x6000;
bootload_entry:
	cli
	xor	%ax,%ax
	mov	%ax,%ss
	mov	%ax,%ds
	mov	%ax,%es
	mov	$0x6000,%sp
	mov	%sp,%bp
	mov	%sp,%di
	mov	$0x7C00,%si
	mov	$0x0200,%cx
	cld
	rep	movsb
	jmp	$0x0000,$start
IRQ0:
	push	%ax
	movb	$0x01,IRQ0.f
	mov	$0x20,%al
	out	%al,$0x20
	pop	%ax
	iret
IRQ0.f:	.byte	0x00

IRQ6:
	push	%ax
	movb	$0x01,IRQ6.f
	mov	$0x20,%al
	out	%al,$0x20
	pop	%ax
	iret
IRQ6.f:	.byte	0x00

timer:
	hlt
	cmpb	$0x01,IRQ0.f
	jne	timer
	movb	$0x00,IRQ0.f
	loop	timer
	ret

waitfloppy:
	hlt
	cmpb	$0x01,IRQ6.f
	jne	waitfloppy
	movb	$0x00,IRQ6.f
	ret

readFIFO:
	mov	$0x03F4,%dx
	in	%dx,%al
	and	$0x80,%al
	cmp	$0x80,%al
	jne	readFIFO
	inc	%dx
	in	%dx,%al
	ret

writeFIFO:
	mov	$0x03F4,%dx
	in	%dx,%al
	and	$0x80,%al
	cmp	$0x80,%al
	jne	writeFIFO
	inc	%dx
	mov	%ah,%al
	out	%al,%dx
	ret

start:
	movw	$IRQ0,0x0020
	movw	$0x00,0x0022
	movw	$IRQ6,0x0038
	movw	$0x00,0x003A

	# Vidad
	mov	$0x0E56,%ax
	mov	$0x0007,%bx
	int	$0x10

	#PIC (master)
	mov	$0x11,%al
	out	%al,$0x20
	mov	$0x08,%al
	out	%al,$0x21
	mov	$0x04,%al
	out	%al,$0x21
	mov	$0x01,%al
	out	%al,$0x21
	mov	$0xBE,%al
	out	%al,$0x21

	#PIT
	mov	$0x36,%al
	out	%al,$0x43
	mov	$0x0B,%al
	out	%al,$0x40
	mov	$0xE9,%al
	out	%al,$0x40

	#DMA
	mov	$0x00,%al
	out	%al,$0xC8
	out	%al,$0x08
	mov	$0xFF,%al
	out	%al,$0xCD
	out	%al,$0x0D
	mov	$0x0E,%al
	out	%al,$0xCF
	out	%al,$0x0F

	# wait 3 secs
	sti
	mov	$0x003C,%cx
	call	timer

	# vIdad
	mov	$0x0E49,%ax
	mov	$0x0007,%bx
	int	$0x10

	#FDC init

	mov	$0x03F2,%dx
	mov	$0x00,%al
	out	%al,%dx

	mov	$0x0C,%al
	out	%al,%dx
	call	waitfloppy
	mov	$0x08,%ah
	call	writeFIFO
	call	readFIFO
	call	readFIFO

	mov	$0x03F7,%dx
	mov	$0x00,%al
	out	%al,%dx

	mov	$0x03,%ah
	call	writeFIFO
	mov	$0xDF,%ah
	call	writeFIFO
	mov	$0x02,%ah
	call	writeFIFO
	
	# floppy motor on

	mov	$0x03F2,%dx
	mov	$0x1C,%al
	out	%al,%dx
	mov	$0x000D,%cx
	call	timer

	# viDad
	mov	$0x0E44,%ax
	mov	$0x0007,%bx
	int	$0x10

	# calibrate

cal:	mov	$0x07,%ah
	call	writeFIFO
	mov	$0x00,%ah
	call	writeFIFO
	call	waitfloppy

	mov	$0x08,%ah
	call	writeFIFO
	call	readFIFO
	call	readFIFO
	cmp	$0x00,%al
	jne	cal

	# set up DMA (track 0)

	mov	$0xFF,%al
	out	%al,$0x0C
	mov	$0x00,%al
	out	%al,$0x04
	mov	$0x7C,%al
	out	%al,$0x04
	mov	$0x00,%al
	out	%al,$0x81

	mov	$0xFF,%al
	out	%al,$0x0C
#	mov	$0xFF,%al
	out	%al,$0x05
	mov	$0x47,%al
	out	%al,$0x05

	mov	$0x46,%al
	out	%al,$0x0B
	mov	$0x0A,%al
	out	%al,$0x0F

	# floppy read (track 0)

	mov	$0x0005,%cx
	call	timer

	mov	$0xC6,%ah
	call	writeFIFO
	mov	$0x00,%ah
	call	writeFIFO
#	mov	$0x00,%ah
	call	writeFIFO
#	mov	$0x00,%ah
	call	writeFIFO
	mov	$0x01,%ah
	call	writeFIFO
	mov	$0x02,%ah
	call	writeFIFO
	mov	$0x12,%ah
	call	writeFIFO
	mov	$0x1B,%ah
	call	writeFIFO
	mov	$0xFF,%ah
	call	writeFIFO

	call	waitfloppy

	mov	$0x0007,%cx
rf0:	call	readFIFO
	loop	rf0

	# vidAd
	mov	$0x0E41,%ax
	mov	$0x0007,%bx
	int	$0x10

	# seek track 1
	
seek1:	mov	$0x0F,%ah
	call	writeFIFO
	mov	$0x00,%ah
	call	writeFIFO
	mov	$0x01,%ah
	call	writeFIFO
	call	waitfloppy

	mov	$0x08,%ah
	call	writeFIFO
	call	readFIFO
	call	readFIFO
	cmp	$0x01,%al
	jne	seek1
	
	# set up DMA (track 1)

	mov	$0x06,%al
	out	%al,$0x0A
	mov	$0xFF,%al
	out	%al,$0x0C
	mov	$0x00,%al
	out	%al,$0x04
	mov	$0x00,%al
	out	%al,$0x04
	mov	$0x01,%al
	out	%al,$0x81

	mov	$0xFF,%al
	out	%al,$0x0C
#	mov	$0xFF,%al
	out	%al,$0x05
	mov	$0x47,%al
	out	%al,$0x05

	mov	$0x46,%al
	out	%al,$0x0B
	mov	$0x0A,%al
	out	%al,$0x0F

	# floppy read (track 1)

	mov	$0x0005,%cx
	call	timer

	mov	$0xC6,%ah
	call	writeFIFO
	mov	$0x00,%ah
	call	writeFIFO
	mov	$0x01,%ah
	call	writeFIFO
	mov	$0x00,%ah
	call	writeFIFO
	mov	$0x01,%ah
	call	writeFIFO
	mov	$0x02,%ah

	# space is up, jump over bootsector signature

	jmp	0x7E00
	.word	0xAA55

	. = 0x7E00;
	call	writeFIFO
	mov	$0x12,%ah
	call	writeFIFO
	mov	$0x1B,%ah
	call	writeFIFO
	mov	$0xFF,%ah
	call	writeFIFO

	call	waitfloppy

	mov	$0x0007,%cx
rf1:	call	readFIFO
	loop	rf1

	# relocate the loaded part of the kernel from 0x10000 to 0x0C400
	push	%es
	push	%ds
	mov	$0x1000,%ax
	mov	%ax,%ds
	mov	$0x0c40,%ax
	mov	%ax,%es
	xor	%si,%si
	xor	%di,%di
	mov	$0x4800,%cx
	rep	movsb
	pop	%ds
	pop	%es

	# vidaD
	mov	$0x0E44,%ax
	mov	$0x0007,%bx
	int	$0x10

	# floppy motor off
	mov	$0x03F2,%dx
	mov	$0x0C,%al
	out	%al,%dx
	mov	$0x000D,%cx
	call	timer

	# get BIOS info (store at 0x6006, move later to 0x8006)

	mov	$0x0F,%ah
	int	$0x10
	cmp	$0x04,%al
	ja	error
	mov	%al,0x6006

	xor	%ax,%ax
	int	$0x12
	jc	error
	cmp	$0x0100,%ax
	jb	error
	mov	%ax,0x6008

	mov	$0x88,%ah
	int	$0x15
	mov	%ax,0x600E

	xor	%dx,%dx
	xor	%cx,%cx
	mov	$0xE801,%ax
	int	$0x15
	jcxz	useax
	mov	%cx,%ax
	mov	%dx,%bx
useax:
	mov	%ax,0x600A
	mov	%bx,0x600C

	# exit FDC
	mov	$0x00,%al
	out	%al,%dx

	# exit DMA
	mov	$0x0F,%al
	out	%al,$0x0F
	out	%al,$0xCF
	mov	$0x04,%al
	out	%al,$0x08
	out	%al,$0xC8

	# exit PIC
	mov	$0xFF,%al
	out	%al,$0x21

	# CPUID

	pushf
	pop	%ax
	mov	%ax,%cx
	and	$0x0FFF,%ax
	push	%ax
	popf
	pushf
	pop	%ax
	and	$0xF000,%ax
	cmp	$0xF000,%ax
	je	error

	or	$0xF000,%cx
	push	%cx
	popf
	pushf
	pop	%ax
	and	$0xF000,%ax
	jz	error

	pushfl
	pop	%eax
	mov	%eax,%ecx
	xor	$0x00040000,%eax
	push	%eax
	popfl
	pushfl
	pop	%eax
	xor	%ecx,%eax
	movb	$0x03,0x6007
	jz	end_cpuid
	push	%ecx
	popfl

	movb	$0x04,0x6007
	mov	%ecx,%eax
	xor	$0x00200000,%eax
	push	%eax
	popfl
	pushfl
	pop	%eax
	xor	%ecx,%eax
	je	end_cpuid
	movb	$0x05,0x6007

end_cpuid:

	# all interrupts off (including NMI)
	in	$0x70,%al
	or	$0x80,%al
	out	%al,$0x70
	cli

	jmp	A20

waitA:
	in	$0x64,%al
	test	$0x02,%al
	jnz	waitA
	ret

waitB:
	in	$0x64,%al
	test	$0x01,%al
	jz	waitB
	ret

error:
	mov	$0x0E21,%ax
	mov	$0x0007,%bx
	int	$0x10
	cli
	hlt

A20:	# turn A20 line on to get access to more than 1 MiB

	call	waitA
	mov	$0xAD,%al
	out	%al,$0x64

	call	waitA
	mov	$0xD0,%al
	out	%al,$0x64

	call	waitB
	in	$0x60,%al
	push	%ax

	call	waitA
	mov	$0xD1,%al
	out	%al,$0x64

	call	waitA
	pop	%ax
	or	$0x03,%al
	out	%al,$0x60

	# checksum

	mov	0x8014,%cx
	shr	$0x02,%cx
	xor	%ax,%ax
	xor	%dx,%dx
	mov	$0x8000,%si
sum:	add	(%si),%ax
	inc	%si
	inc	%si
	adc	(%si),%dx
	inc	%si
	inc	%si
	loop	sum
	cmp	%ax,%dx
	jne	error
	xor	%dx,%dx
	cmp	%ax,%dx
	jne	error

	# reloc BIOS info

	mov	$0x6006,%si
	mov	$0x8006,%di
	mov	$0x000A,%cx
	rep	movsb

	# init tables

	mov	$0x0800,%di
	mov	0x801C,%si
	add	$0x8000,%si
	mov	0x801E,%cx
	rep	movsb

	mov	$0x1000,%di
	mov	0x8018,%si
	add	$0x8000,%si
	mov	0x801A,%cx
	rep	movsb

	mov	0x801A,%ax
	dec	%ax
	mov	%ax,0x6000
	mov	$0x1000,%ax
	mov	%ax,0x6002
	xor	%ax,%ax
	mov	%ax,0x6004

	mov	0x801E,%ax
	dec	%ax
	mov	%ax,0x6008
	mov	$0x0800,%ax
	mov	%ax,0x600A
	xor	%ax,%ax
	mov	%ax,0x600C

	# init final goto

	mov	$end_jmp,%bx
	mov	0x8024,%ax
	mov	%ax,2(%bx)
	mov	0x8026,%ax
	mov	%ax,4(%bx)

	# enter pmode

	lidt	0x6008
	lgdt	0x6000

	smsw	%ax
	or	$0x01,%al
	lmsw	%ax

end_jmp:
	jmpl	$0x0008,$0x00000000
