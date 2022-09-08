! -*- tab-width: 8 -*-

	.file "crt0.s"
	.ignore	ld_st_style
	.text

SYS_exit = 1

	.global	_start
	.type	_start, #function
	.align	8
_start:
	{
		setwd wsz = 8, nfx = 1, dbl = 0
		setbn rbs = 4, rsz = 3, rcur = 0
		disp %ctpr1, main; ipd 2
	}
	{
		nop 1
		getsp,0 0, %r0
	}
	{
		nop 3
		disp %ctpr2, 2f; ipd 2
		ldd %r0, 0, %b[0]  ! argc
		ldd %r0, 8, %r3  ! unparsed argv
	}
	{
		addd %b[0], 0, %r1
		addd %b[0], 9, %r2
	}
	{
		shld %r2, 3, %r2
	}
	{
		subd 0, %r2, %r2
	}
	{
		andnd %r2, 15, %r2
	}
	{
		nop 1
		! alloca(argc * 8 + 8)
		getsp,0 %r2, %r2
	}
	{
		nop 2
		addd %r2, 64, %b[1]  ! argv
		addd %r2, 64, %r2
		ldb,sm 0, %r3, %r5
		cmpedb %r1, 0, %pred1
		ibranch	1f; ipd 3
	}
2:	{
		nop 2
		cmpesb %r5, 0, %pred0
		ldb,sm %r3, 1, %r5
		addd %r3, 1, %r3
	}
	{
		ct %ctpr2 ? ~%pred0; ipd 3
	}
1:	{
		cmpedb %r1, 1, %pred1
		subd %r1, 1, %r1
		std %r2, 0, %r3 ? ~%pred1
		std %r2, 0, %r1 ? %pred1  ! write NULL
		addd %r2, 8, %r2
		ct %ctpr2 ? ~%pred1; ipd 3
	}
	{
		call %ctpr1, wbs = 4; ipd 3
	}
	{
		nop 4
		sdisp %ctpr1, 3; ipd 2
		addd 0, SYS_exit, %b[0]
		addd 0, %b[0], %b[1]
	}
	{
		call %ctpr1, wbs = 4; ipd 3
	}
	.size	_start, .- _start
