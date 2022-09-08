	.file "crt0.s"
	.ignore	ld_st_style
	.text

SYS_exit = 1

	.global	_start
	.type	_start, #function
	.align	8
_start:
	{
		nop 4
		setwd wsz = 8, nfx = 1, dbl = 0
		setbn rbs = 4, rsz = 3, rcur = 0
		disp %ctpr1, main
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
