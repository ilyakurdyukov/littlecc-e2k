! -*- tab-width: 8 -*-

	.file "syscall.s"
	.ignore	ld_st_style
	.text
	.global	syscall
	.type	syscall, #function
	.align	8
syscall:
	{
		setwd wsz = 8, nfx = 1, dbl = 0
		setbn rbs = 4, rsz = 3, rcur = 0
		sdisp %ctpr1, 3
		shld 1, 5, %r2
	}
	{
		nop 1
		getsp,0 0, %r1
	}
	{
		addd,sm 0, %r0, %b[0]
		ldd,sm 8, %r1, %b[1]
		ldd,sm 16, %r1, %b[2]
		ldd,sm 24, %r1, %b[3]
		addd %r1, %r2, %r2
	}
	{
		ldd,sm 0, %r2, %b[4]
		ldd,sm 8, %r2, %b[5]
		ldd,sm 16, %r2, %b[6]
		ldd,sm 24, %r2, %b[7]
	}
	{
		call %ctpr1, wbs = 4; ipd 3
	}
	{
		nop 5
		return %ctpr3; ipd 2
		addd,sm %b[0], 0, %r0
	}
	{
		ct %ctpr3; ipd 3
	}
	.size	syscall, .- syscall
