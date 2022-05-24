%{
/*
// e2k/linux lburg spec.
// Written by Ilya Kurdyukov <jpegqs@gmail.com>
*/
#include "c.h"
#define NODEPTR_TYPE Node
#define OP_LABEL(p) ((p)->op)
#define LEFT_CHILD(p) ((p)->kids[0])
#define RIGHT_CHILD(p) ((p)->kids[1])
#define STATE_LABEL(p) ((p)->x.state)
static void address(Symbol, Symbol, long);
static void blkfetch(int, int, int, int);
static void blkloop(int, int, int, int, int, int[]);
static void blkstore(int, int, int, int);
static void defaddress(Symbol);
static void defconst(int, int, Value);
static void defstring(int, char *);
static void defsymbol(Symbol);
static void doarg(Node);
static void emit2(Node);
static void export(Symbol);
static void clobber(Node);
static void function(Symbol, Symbol[], Symbol[], int);
static void global(Symbol);
static void import(Symbol);
static void local(Symbol);
static void progbeg(int, char**);
static void progend(void);
static void segment(int);
static void space(int);
static void target(Node);
static Symbol ireg[32], iregw, *breg = ireg + 24;

static int retstruct;
static int cseg;

#define cvtsize(size, cost) (a->syms[0]->u.c.v.i == size ? cost : LBURG_MAX)
%}
%start stmt
%term CNSTF4=4113
%term CNSTF8=8209
%term CNSTF16=16401
%term CNSTI1=1045
%term CNSTI2=2069
%term CNSTI4=4117
%term CNSTI8=8213
%term CNSTP4=4119
%term CNSTP8=8215
%term CNSTU1=1046
%term CNSTU2=2070
%term CNSTU4=4118
%term CNSTU8=8214
 
%term ARGB=41
%term ARGF4=4129
%term ARGF8=8225
%term ARGF16=16417
%term ARGI4=4133
%term ARGI8=8229
%term ARGP4=4135
%term ARGP8=8231
%term ARGU4=4134
%term ARGU8=8230

%term ASGNB=57
%term ASGNF4=4145
%term ASGNF8=8241
%term ASGNF16=16433
%term ASGNI1=1077
%term ASGNI2=2101
%term ASGNI4=4149
%term ASGNI8=8245
%term ASGNP4=4151
%term ASGNP8=8247
%term ASGNU1=1078
%term ASGNU2=2102
%term ASGNU4=4150
%term ASGNU8=8246

%term INDIRB=73
%term INDIRF4=4161
%term INDIRF8=8257
%term INDIRF16=16449
%term INDIRI1=1093
%term INDIRI2=2117
%term INDIRI4=4165
%term INDIRI8=8261
%term INDIRP4=4167
%term INDIRP8=8263
%term INDIRU1=1094
%term INDIRU2=2118
%term INDIRU4=4166
%term INDIRU8=8262

%term CVFF4=4209
%term CVFF8=8305
%term CVFF16=16497
%term CVFI4=4213
%term CVFI8=8309

%term CVIF4=4225
%term CVIF8=8321
%term CVIF16=16513
%term CVII1=1157
%term CVII2=2181
%term CVII4=4229
%term CVII8=8325
%term CVIU1=1158
%term CVIU2=2182
%term CVIU4=4230
%term CVIU8=8326

%term CVPP4=4247
%term CVPP8=8343
%term CVPP16=16535
%term CVPU4=4246
%term CVPU8=8342

%term CVUI1=1205
%term CVUI2=2229
%term CVUI4=4277
%term CVUI8=8373
%term CVUP4=4279
%term CVUP8=8375
%term CVUP16=16567
%term CVUU1=1206
%term CVUU2=2230
%term CVUU4=4278
%term CVUU8=8374

%term NEGF4=4289
%term NEGF8=8385
%term NEGF16=16577
%term NEGI4=4293
%term NEGI8=8389

%term CALLB=217
%term CALLF4=4305
%term CALLF8=8401
%term CALLF16=16593
%term CALLI4=4309
%term CALLI8=8405
%term CALLP4=4311
%term CALLP8=8407
%term CALLU4=4310
%term CALLU8=8406
%term CALLV=216

%term RETF4=4337
%term RETF8=8433
%term RETF16=16625
%term RETI4=4341
%term RETI8=8437
%term RETP4=4343
%term RETP8=8439
%term RETU4=4342
%term RETU8=8438
%term RETV=248

%term ADDRGP4=4359
%term ADDRGP8=8455

%term ADDRFP4=4375
%term ADDRFP8=8471

%term ADDRLP4=4391
%term ADDRLP8=8487

%term ADDF4=4401
%term ADDF8=8497
%term ADDF16=16689
%term ADDI4=4405
%term ADDI8=8501
%term ADDP4=4407
%term ADDP8=8503
%term ADDU4=4406
%term ADDU8=8502

%term SUBF4=4417
%term SUBF8=8513
%term SUBF16=16705
%term SUBI4=4421
%term SUBI8=8517
%term SUBP4=4423
%term SUBP8=8519
%term SUBU4=4422
%term SUBU8=8518

%term LSHI4=4437
%term LSHI8=8533
%term LSHU4=4438
%term LSHU8=8534

%term MODI4=4453
%term MODI8=8549
%term MODU4=4454
%term MODU8=8550

%term RSHI4=4469
%term RSHI8=8565
%term RSHU4=4470
%term RSHU8=8566

%term BANDI4=4485
%term BANDI8=8581
%term BANDU4=4486
%term BANDU8=8582

%term BCOMI4=4501
%term BCOMI8=8597
%term BCOMU4=4502
%term BCOMU8=8598

%term BORI4=4517
%term BORI8=8613
%term BORU4=4518
%term BORU8=8614

%term BXORI4=4533
%term BXORI8=8629
%term BXORU4=4534
%term BXORU8=8630

%term DIVF4=4545
%term DIVF8=8641
%term DIVF16=16833
%term DIVI4=4549
%term DIVI8=8645
%term DIVU4=4550
%term DIVU8=8646

%term MULF4=4561
%term MULF8=8657
%term MULF16=16849
%term MULI4=4565
%term MULI8=8661
%term MULU4=4566
%term MULU8=8662

%term EQF4=4577
%term EQF8=8673
%term EQF16=16865
%term EQI4=4581
%term EQI8=8677
%term EQU4=4582
%term EQU8=8678

%term GEF4=4593
%term GEF8=8689
%term GEI4=4597
%term GEI8=8693
%term GEI16=16885
%term GEU4=4598
%term GEU8=8694

%term GTF4=4609
%term GTF8=8705
%term GTF16=16897
%term GTI4=4613
%term GTI8=8709
%term GTU4=4614
%term GTU8=8710

%term LEF4=4625
%term LEF8=8721
%term LEF16=16913
%term LEI4=4629
%term LEI8=8725
%term LEU4=4630
%term LEU8=8726

%term LTF4=4641
%term LTF8=8737
%term LTF16=16929
%term LTI4=4645
%term LTI8=8741
%term LTU4=4646
%term LTU8=8742

%term NEF4=4657
%term NEF8=8753
%term NEF16=16945
%term NEI4=4661
%term NEI8=8757
%term NEU4=4662
%term NEU8=8758

%term JUMPV=584

%term LABELV=600

%term LOADB=233
%term LOADF4=4321
%term LOADF8=8417
%term LOADF16=16609
%term LOADI1=1253
%term LOADI2=2277
%term LOADI4=4325
%term LOADI8=8421
%term LOADP4=4327
%term LOADP8=8423
%term LOADU1=1254
%term LOADU2=2278
%term LOADU4=4326
%term LOADU8=8422

%term VREGP=711
%%
reg:  INDIRI1(VREGP)     "# read register\n"
reg:  INDIRU1(VREGP)     "# read register\n"
reg:  INDIRI2(VREGP)     "# read register\n"
reg:  INDIRU2(VREGP)     "# read register\n"
reg:  INDIRI4(VREGP)     "# read register\n"
reg:  INDIRU4(VREGP)     "# read register\n"
reg:  INDIRP4(VREGP)     "# read register\n"
reg:  INDIRF4(VREGP)     "# read register\n"
reg:  INDIRI8(VREGP)     "# read register\n"
reg:  INDIRU8(VREGP)     "# read register\n"
reg:  INDIRP8(VREGP)     "# read register\n"
reg:  INDIRF8(VREGP)     "# read register\n"

stmt: ASGNI1(VREGP,reg)  "# write register\n"
stmt: ASGNU1(VREGP,reg)  "# write register\n"
stmt: ASGNI2(VREGP,reg)  "# write register\n"
stmt: ASGNU2(VREGP,reg)  "# write register\n"
stmt: ASGNI4(VREGP,reg)  "# write register\n"
stmt: ASGNU4(VREGP,reg)  "# write register\n"
stmt: ASGNP4(VREGP,reg)  "# write register\n"
stmt: ASGNF4(VREGP,reg)  "# write register\n"
stmt: ASGNI8(VREGP,reg)  "# write register\n"
stmt: ASGNU8(VREGP,reg)  "# write register\n"
stmt: ASGNP8(VREGP,reg)  "# write register\n"
stmt: ASGNF8(VREGP,reg)  "# write register\n"

con: CNSTI1  "%a"
con: CNSTU1  "%a"
con: CNSTI2  "%a"
con: CNSTU2  "%a"
con: CNSTI4  "%a"
con: CNSTU4  "%a"
con: CNSTI8  "%a"
con: CNSTU8  "%a"
con: CNSTP8  "%a"

stmt: reg  ""
rc: con  "%0"
rc: reg  "%%%0"
rc5: CNSTI4  "%a"  range(a, 0, 31)
rc5: CNSTI8  "%a"  range(a, 0, 31)
rc5: reg  "%%%0"
addr: rc  "0, %0"
addr: ADDRGP8  "0, %a"
stk: ADDRFP8  "%a+%F"
stk: ADDRLP8  "%a+%F"
addr: stk  "%%r23, %0"
addr: ADDI8(reg,rc)  "%%%0, %1"
addr: ADDP8(reg,rc)  "%%%0, %1"
addr: ADDU8(reg,rc)  "%%%0, %1"

reg: addr "\taddd\t%0, %%%c\n"  1

reg: INDIRI1(addr)  "\tldb\t%0, %%%c\n"  1
reg: INDIRI2(addr)  "\tldh\t%0, %%%c\n"  1
reg: INDIRI4(addr)  "\tldw\t%0, %%%c\n"  1
reg: INDIRI8(addr)  "\tldd\t%0, %%%c\n"  1
reg: INDIRU1(addr)  "\tldb\t%0, %%%c\n"  1
reg: INDIRU2(addr)  "\tldh\t%0, %%%c\n"  1
reg: INDIRU4(addr)  "\tldw\t%0, %%%c\n"  1
reg: INDIRU8(addr)  "\tldw\t%0, %%%c\n"  1
reg: INDIRP8(addr)  "\tldd\t%0, %%%c\n"  1
reg: INDIRF4(addr)  "\tldw\t%0, %%%c\n"  1
reg: INDIRF8(addr)  "\tldd\t%0, %%%c\n"  1

stmt: ASGNI1(addr,reg)  "\tstb\t%0, %%%1\n"  1
stmt: ASGNI2(addr,reg)  "\tsth\t%0, %%%1\n"  1
stmt: ASGNI4(addr,reg)  "\tstw\t%0, %%%1\n"  1
stmt: ASGNI8(addr,reg)  "\tstd\t%0, %%%1\n"  1
stmt: ASGNU1(addr,reg)  "\tstb\t%0, %%%1\n"  1
stmt: ASGNU2(addr,reg)  "\tsth\t%0, %%%1\n"  1
stmt: ASGNU4(addr,reg)  "\tstw\t%0, %%%1\n"  1
stmt: ASGNU8(addr,reg)  "\tstd\t%0, %%%1\n"  1
stmt: ASGNP8(addr,reg)  "\tstd\t%0, %%%1\n"  1
stmt: ASGNF4(addr,reg)  "\tstw\t%0, %%%1\n"  1
stmt: ASGNF8(addr,reg)  "\tstd\t%0, %%%1\n"  1

reg: CVUU4(INDIRU1(addr))  "\tldb\t%0, %%%c\n"  1
reg: CVUU8(INDIRU1(addr))  "\tldb\t%0, %%%c\n"  1
reg: CVUU4(INDIRU2(addr))  "\tldh\t%0, %%%c\n"  1
reg: CVUU8(INDIRU2(addr))  "\tldh\t%0, %%%c\n"  1
reg: CVUU8(INDIRU4(addr))  "\tldw\t%0, %%%c\n"  1
reg: CVUI4(INDIRU1(addr))  "\tldb\t%0, %%%c\n"  1
reg: CVUI8(INDIRU1(addr))  "\tldb\t%0, %%%c\n"  1
reg: CVUI4(INDIRU2(addr))  "\tldh\t%0, %%%c\n"  1
reg: CVUI8(INDIRU2(addr))  "\tldh\t%0, %%%c\n"  1
reg: CVUI8(INDIRU4(addr))  "\tldw\t%0, %%%c\n"  1

reg: LOADI1(reg)  "\tadds\t0, %%%0, %%%c\n"  move(a)
reg: LOADI2(reg)  "\tadds\t0, %%%0, %%%c\n"  move(a)
reg: LOADI4(reg)  "\tadds\t0, %%%0, %%%c\n"  move(a)
reg: LOADI8(reg)  "\tadds\t0, %%%0, %%%c\n"  move(a)
reg: LOADP8(reg)  "\taddd\t0, %%%0, %%%c\n"  move(a)
reg: LOADU1(reg)  "\tadds\t0, %%%0, %%%c\n"  move(a)
reg: LOADU2(reg)  "\tadds\t0, %%%0, %%%c\n"  move(a)
reg: LOADU4(reg)  "\tadds\t0, %%%0, %%%c\n"  move(a)
reg: LOADU8(reg)  "\taddd\t0, %%%0, %%%c\n"  move(a)
reg: LOADF4(reg)  "\tadds\t0, %%%0, %%%c\n"  move(a)
reg: LOADF8(reg)  "\taddd\t0, %%%0, %%%c\n"  move(a)

reg: ADDI4(rc5,rc)   "\tadds\t%0, %1, %%%c\n"  1
reg: ADDU4(rc5,rc)   "\tadds\t%0, %1, %%%c\n"  1
reg: ADDI8(rc5,rc)   "\taddd\t%0, %1, %%%c\n"  1
reg: ADDU8(rc5,rc)   "\taddd\t%0, %1, %%%c\n"  1
reg: ADDP8(rc5,rc)   "\taddd\t%0, %1, %%%c\n"  1
reg: BANDI4(rc5,rc)  "\tands\t%0, %1, %%%c\n"  1
reg: BANDU4(rc5,rc)  "\tands\t%0, %1, %%%c\n"  1
reg: BANDI8(rc5,rc)  "\tandd\t%0, %1, %%%c\n"  1
reg: BANDU8(rc5,rc)  "\tandd\t%0, %1, %%%c\n"  1
reg: BORI4(rc5,rc)   "\tors\t%0, %1, %%%c\n"  1
reg: BORU4(rc5,rc)   "\tors\t%0, %1, %%%c\n"  1
reg: BORI8(rc5,rc)   "\tord\t%0, %1, %%%c\n"  1
reg: BORU8(rc5,rc)   "\tord\t%0, %1, %%%c\n"  1
reg: BXORI4(rc5,rc)  "\txors\t%0, %1, %%%c\n"  1
reg: BXORU4(rc5,rc)  "\txors\t%0, %1, %%%c\n"  1
reg: BXORI8(rc5,rc)  "\txord\t%0, %1, %%%c\n"  1
reg: BXORU8(rc5,rc)  "\txord\t%0, %1, %%%c\n"  1
reg: SUBI4(rc5,rc)   "\tsubs\t%0, %1, %%%c\n"  1
reg: SUBU4(rc5,rc)   "\tsubs\t%0, %1, %%%c\n"  1
reg: SUBI8(rc5,rc)   "\tsubd\t%0, %1, %%%c\n"  1
reg: SUBU8(rc5,rc)   "\tsubd\t%0, %1, %%%c\n"  1
reg: SUBP8(rc5,rc)   "\tsubd\t%0, %1, %%%c\n"  1

reg: LSHI4(rc5,rc)  "\tshls\t%0, %1, %%%c\n"  1
reg: LSHU4(rc5,rc)  "\tshls\t%0, %1, %%%c\n"  1
reg: LSHI8(rc5,rc)  "\tshld\t%0, %1, %%%c\n"  1
reg: LSHU8(rc5,rc)  "\tshld\t%0, %1, %%%c\n"  1
reg: RSHI4(rc5,rc)  "\tsars\t%0, %1, %%%c\n"  1
reg: RSHU4(rc5,rc)  "\tshrs\t%0, %1, %%%c\n"  1
reg: RSHI8(rc5,rc)  "\tsard\t%0, %1, %%%c\n"  1
reg: RSHU8(rc5,rc)  "\tshrd\t%0, %1, %%%c\n"  1

reg: BANDI4(BCOMI4(rc5),rc)  "\tandns\t%0, %1, %%%c\n"  1
reg: BANDU4(BCOMU4(rc5),rc)  "\tandns\t%0, %1, %%%c\n"  1
reg: BORI4(BCOMI4(rc5),rc)   "\torns\t%0, %1, %%%c\n"  1
reg: BORU4(BCOMU4(rc5),rc)   "\torns\t%0, %1, %%%c\n"  1
reg: BXORI4(BCOMI4(rc5),rc)  "\txorns\t%0, %1, %%%c\n"  1
reg: BXORU4(BCOMU4(rc5),rc)  "\txorns\t%0, %1, %%%c\n"  1

reg: NEGI4(reg)   "\tsubs\t0, %%%0, %%%c\n"  1
reg: NEGI8(reg)   "\tsubd\t0, %%%0, %%%c\n"  1
reg: BCOMI4(reg)  "\txorns\t0, %%%0, %%%c\n"  1
reg: BCOMU4(reg)  "\txorns\t0, %%%0, %%%c\n"  1
reg: BCOMI8(reg)  "\txornd\t0, %%%0, %%%c\n"  1
reg: BCOMU8(reg)  "\txornd\t0, %%%0, %%%c\n"  1

reg: CVII4(reg)  "\tsxt\t0, %%%0, %%%c\n"  cvtsize(1, 1)
reg: CVIU4(reg)  "\tsxt\t0, %%%0, %%%c\n"  cvtsize(1, 1)
reg: CVII4(reg)  "\tsxt\t1, %%%0, %%%c\n"  cvtsize(2, 1)
reg: CVIU4(reg)  "\tsxt\t1, %%%0, %%%c\n"  cvtsize(2, 1)
reg: CVUI4(reg)  "\tsxt\t4, %%%0, %%%c\n"  cvtsize(1, 1)
reg: CVUU4(reg)  "\tsxt\t4, %%%0, %%%c\n"  cvtsize(1, 1)
reg: CVUI4(reg)  "\tsxt\t5, %%%0, %%%c\n"  cvtsize(2, 1)
reg: CVUU4(reg)  "\tsxt\t5, %%%0, %%%c\n"  cvtsize(2, 1)

reg1i: CVII4(reg)  "%%%0"  cvtsize(1, 0)
reg2i: CVII4(reg)  "%%%0"  cvtsize(2, 0)
reg1u: CVUI4(reg)  "%%%0"  cvtsize(1, 0)
reg2u: CVUI4(reg)  "%%%0"  cvtsize(2, 0)
reg: CVII8(reg1i)  "\tsxt\t0, %0, %%%c\n"  1
reg: CVII8(reg2i)  "\tsxt\t1, %0, %%%c\n"  1
reg: CVIU8(reg1i)  "\tsxt\t0, %0, %%%c\n"  1
reg: CVIU8(reg2i)  "\tsxt\t1, %0, %%%c\n"  1
reg: CVII8(reg1u)  "\tsxt\t4, %0, %%%c\n"  1
reg: CVII8(reg2u)  "\tsxt\t5, %0, %%%c\n"  1
reg: CVIU8(reg1u)  "\tsxt\t4, %0, %%%c\n"  1
reg: CVIU8(reg2u)  "\tsxt\t5, %0, %%%c\n"  1

reg: CVII8(reg)  "\tsxt\t2, %%%0, %%%c\n"  cvtsize(4, 1)
reg: CVIU8(reg)  "\tsxt\t2, %%%0, %%%c\n"  cvtsize(4, 1)
reg: CVUI8(reg)  "\tsxt\t6, %%%0, %%%c\n"  cvtsize(4, 1)
reg: CVUU8(reg)  "\tsxt\t6, %%%0, %%%c\n"  cvtsize(4, 1)

reg: MULI4(rc5,rc)  "\tmuls\t%0, %1, %%%c\n"  6
reg: MULU4(rc5,rc)  "\tmuls\t%0, %1, %%%c\n"  6
reg: MULI8(rc5,rc)  "\tmuld\t%0, %1, %%%c\n"  6
reg: MULU8(rc5,rc)  "\tmuld\t%0, %1, %%%c\n"  6
reg: DIVI4(rc5,rc)  "\tsdivs\t%0, %1, %%%c\n"  10
reg: DIVU4(rc5,rc)  "\tudivs\t%0, %1, %%%c\n"  10
reg: DIVI8(rc5,rc)  "\tsdivd\t%0, %1, %%%c\n"  10
reg: DIVU8(rc5,rc)  "\tudivd\t%0, %1, %%%c\n"  10
reg: MODI4(reg,rc)  "\tsxt\t2, %%%0, %%r8\n\tsmodx\t%%r8, %1, %%%c\n"  11
reg: MODU4(reg,rc)  "\tsxt\t6, %%%0, %%r8\n\tumodx\t%%r8, %1, %%%c\n"  11
reg: MODI8(rc5,rc)  "\tsdivd\t%0, %1, %%r8\n\tmuld\t%%r8, %1, %%r8\n\tsubd\t%0, %%r8, %%%c\n"  17
reg: MODU8(rc5,rc)  "\tudivd\t%0, %1, %%r8\n\tmuld\t%%r8, %1, %%r8\n\tsubd\t%0, %%r8, %%%c\n"  17

pred0: EQI4(rc5,rc)  "cmpesb\t%0, %1"
pred0: EQU4(rc5,rc)  "cmpesb\t%0, %1"
pred0: LEI4(rc5,rc)  "cmplesb\t%0, %1"
pred0: LEU4(rc5,rc)  "cmpbesb\t%0, %1"
pred0: LTI4(rc5,rc)  "cmplsb\t%0, %1"
pred0: LTU4(rc5,rc)  "cmpbsb\t%0, %1"
npred0: NEI4(rc5,rc)  "cmpesb\t%0, %1"
npred0: NEU4(rc5,rc)  "cmpesb\t%0, %1"
npred0: GEI4(rc5,rc)  "cmplsb\t%0, %1"
npred0: GEU4(rc5,rc)  "cmpbsb\t%0, %1"
npred0: GTI4(rc5,rc)  "cmplesb\t%0, %1"
npred0: GTU4(rc5,rc)  "cmpbesb\t%0, %1"

pred0: EQI8(rc5,rc)  "cmpedb\t%0, %1"
pred0: EQU8(rc5,rc)  "cmpedb\t%0, %1"
pred0: LEI8(rc5,rc)  "cmpledb\t%0, %1"
pred0: LEU8(rc5,rc)  "cmpbedb\t%0, %1"
pred0: LTI8(rc5,rc)  "cmpldb\t%0, %1"
pred0: LTU8(rc5,rc)  "cmpbdb\t%0, %1"
npred0: NEI8(rc5,rc)  "cmpedb\t%0, %1"
npred0: NEU8(rc5,rc)  "cmpedb\t%0, %1"
npred0: GEI8(rc5,rc)  "cmpldb\t%0, %1"
npred0: GEU8(rc5,rc)  "cmpbdb\t%0, %1"
npred0: GTI8(rc5,rc)  "cmpledb\t%0, %1"
npred0: GTU8(rc5,rc)  "cmpbedb\t%0, %1"

pred0: EQF4(rc5,rc)  "fcmpeqsb\t%0, %1"
pred0: EQF8(rc5,rc)  "fcmpeqdb\t%0, %1"
pred0: LEF4(rc5,rc)  "fcmplesb\t%0, %1"
pred0: LEF8(rc5,rc)  "fcmpledb\t%0, %1"
pred0: LTF4(rc5,rc)  "fcmpltsb\t%0, %1"
pred0: LTF8(rc5,rc)  "fcmpltdb\t%0, %1"
npred0: NEF4(rc5,rc)  "fcmpeqsb\t%0, %1"
npred0: NEF8(rc5,rc)  "fcmpeqsb\t%0, %1"
npred0: GEF4(rc5,rc)  "fcmpltsb\t%0, %1"
npred0: GEF8(rc5,rc)  "fcmpltdb\t%0, %1"
npred0: GTF4(rc5,rc)  "fcmplesb\t%0, %1"
npred0: GTF8(rc5,rc)  "fcmpledb\t%0, %1"

stmt: pred0  "\t%0, %%pred0\n\tibranch\t%a ? %%pred0\n"  4
stmt: npred0  "\t%0, %%pred0\n\tibranch\t%a ? ~%%pred0\n"  4

reg: ADDF4(reg,reg)  "\tfadds\t%%%0, %%%1, %%%c\n"  1
reg: ADDF8(reg,reg)  "\tfaddd\t%%%0, %%%1, %%%c\n"  1
reg: SUBF4(reg,reg)  "\tfsubs\t%%%0, %%%1, %%%c\n"  1
reg: SUBF8(reg,reg)  "\tfsubd\t%%%0, %%%1, %%%c\n"  1
reg: MULF4(reg,reg)  "\tfmuls\t%%%0, %%%1, %%%c\n"  5
reg: MULF8(reg,reg)  "\tfmuld\t%%%0, %%%1, %%%c\n"  5
reg: DIVF4(reg,reg)  "\tfdivs\t%%%0, %%%1, %%%c\n"  10
reg: DIVF8(reg,reg)  "\tfdivd\t%%%0, %%%1, %%%c\n"  10
reg: NEGF4(reg)   "\txors\t%%%0, 0x80000000, %%%c\n"  1
reg: NEGF8(reg)   "\txord\t%%%0, 0x8000000000000000, %%%c\n"  1

reg: CVFF4(reg)  "\tfdtofs\t%%%0, %%%c\n"  1
reg: CVFF8(reg)  "\tfstofd\t%%%0, %%%c\n"  1
reg: CVFI4(reg)  "\tfstoistr\t%%%0, %%%c\n"  cvtsize(4, 3)
reg: CVFI4(reg)  "\tfdtoistr\t%%%0, %%%c\n"  cvtsize(8, 3)
reg: CVFI8(reg)  "\tfstoidtr\t%%%0, %%%c\n"  cvtsize(4, 3)
reg: CVFI8(reg)  "\tfdtoidtr\t%%%0, %%%c\n"  cvtsize(8, 3)
reg: CVIF4(reg)  "\tistofs\t%%%0, %%%c\n"  cvtsize(4, 3)
reg: CVIF4(reg)  "\tidtofs\t%%%0, %%%c\n"  cvtsize(8, 3)
reg: CVIF8(reg)  "\tistofd\t%%%0, %%%c\n"  cvtsize(4, 3)
reg: CVIF8(reg)  "\tidtofd\t%%%0, %%%c\n"  cvtsize(8, 3)

label: ADDRGP8  "%a"
stmt: JUMPV(label)  "\tibranch\t%0\n"   2
stmt: LABELV  "%a:\n"

ctpr1: reg  "\tmovtd\t%%%0, %%ctpr1" 5
ctpr1: ADDRGP8  "\tdisp\t%%ctpr1, %a" 5
stmt: JUMPV(ctpr1)  "%0\n\tct\t%%ctpr1\n" 5
reg: CALLI4(ctpr1)  "%0\n\tcall\t%%ctpr1, wbs = 12\n"  10
reg: CALLI8(ctpr1)  "%0\n\tcall\t%%ctpr1, wbs = 12\n"  10
reg: CALLU4(ctpr1)  "%0\n\tcall\t%%ctpr1, wbs = 12\n"  10
reg: CALLU8(ctpr1)  "%0\n\tcall\t%%ctpr1, wbs = 12\n"  10
reg: CALLP8(ctpr1)  "%0\n\tcall\t%%ctpr1, wbs = 12\n"  10
reg: CALLF4(ctpr1)  "%0\n\tcall\t%%ctpr1, wbs = 12\n"  10
reg: CALLF8(ctpr1)  "%0\n\tcall\t%%ctpr1, wbs = 12\n"  10
stmt: CALLV(ctpr1)  "%0\n\tcall\t%%ctpr1, wbs = 12\n"  10
stmt: CALLB(ctpr1,reg)  "# CALLB\n"  10

stmt: RETI4(reg)  "# ret\n"  1
stmt: RETU4(reg)  "# ret\n"  1
stmt: RETI8(reg)  "# ret\n"  1
stmt: RETU8(reg)  "# ret\n"  1
stmt: RETP8(reg)  "# ret\n"  1
stmt: RETF4(reg)  "# ret\n"  1
stmt: RETF8(reg)  "# ret\n"  1
stmt: RETV(reg)   "# ret\n"  1

stmt: ARGI4(reg)  "\tstw\t%%r23, %c*8, %%%0\n"  1
stmt: ARGU4(reg)  "\tstw\t%%r23, %c*8, %%%0\n"  1
stmt: ARGI8(reg)  "\tstd\t%%r23, %c*8, %%%0\n"  1
stmt: ARGU8(reg)  "\tstd\t%%r23, %c*8, %%%0\n"  1
stmt: ARGP8(reg)  "\tstd\t%%r23, %c*8, %%%0\n"  1
stmt: ARGF4(reg)  "\tstw\t%%r23, %c*8, %%%0\n"  1
stmt: ARGF8(reg)  "\tstd\t%%r23, %c*8, %%%0\n"  1

stmt: ARGB(INDIRB(reg))  "# ARGB\n"
stmt: ASGNB(reg,INDIRB(reg))  "# ASGNB\n"

%%
static void progbeg(int argc, char *argv[]) {
	int i;

	{
		union { char c; int i; } u;
		u.i = 1; 
		swap = u.c != IR->little_endian;
	}

	parseflags(argc, argv);

	for (i = 0; i < 24; i++)
		ireg[i] = mkreg("r%d", i, 1, IREG);
	for (i = 0; i < 8; i++)
		ireg[i + 24] = mkreg(stringf("b[%d]", i), i + 24, 1, IREG);

	iregw = mkwildcard(ireg);
	tmask[IREG] = 0x007fffff;
	vmask[IREG] = 0;
	tmask[FREG] = 0;
	vmask[FREG] = 0;

	print("\t.file\t\"%s\"\n", firstfile);
	print("\t.ignore\tld_st_style\n");
}

static void progend(void) {
	const char *name = "little_c_compiler_v4.2";
	print("\t.weak\t%s\n\t%s = 0\n", name, name);
}

static Symbol rmap(int opk) {
	switch (optype(opk)) {
	case I: case U: case P: case B: case F:
		return iregw;
	default:
		return 0;
	}
}

/* variadic arguments (including preceding argument)
   must be both in registers and on the stack */
static int variadic_lim(Node p, int n) {
	Type ty, *proto; int i;

	/* ugly hack to find function argument list */
	for (; p; p = p->link) {
		int t = generic(p->op);
		if (t == CALL) break;
		if (t == ASGN && generic(p->kids[1]->op) == CALL) {
			p = p->kids[1];
			break;
		}
	}
	assert(p && p->syms[0]);
	ty = p->syms[0]->type;
	assert(ty && isfunc(ty));
	proto = ty->u.f.proto;

	/* without a prototype it's impossible 
	   to know whether it is variadic or not */
	if (!proto) n = 0;
	else {
		for (i = 0; proto[i]; i++);
		if (i > 1 && proto[i - 1] == voidtype)
			for (n = 0; i > 2; i--)
				n += (proto[i - 3]->size + 7) >> 3;
	}
	return n;
}

static void target(Node p) {
	assert(p);
	switch (specific(p->op)) {
	case CALL+B: {
		int size;
		assert(p->syms[1] && p->syms[1]->type && isfunc(p->syms[1]->type));
		size = freturn(p->syms[1]->type)->size;
		if (maxargoffset < size) maxargoffset = size;
		p->syms[1] = intconst(size);
		break;
	}
	case CALL+I: case CALL+P: case CALL+U: case CALL+F: case CALL+V:
		setreg(p, breg[0]);
		break;
	case RET+I: case RET+P: case RET+U: case RET+F:
		rtarget(p, 0, ireg[0]);
		p->kids[0]->x.registered = 1;
		break;
	case ARG+I: case ARG+P: case ARG+U: case ARG+F: {
		int i = p->syms[RX]->u.c.v.i, n = 8;
		if (i < n) {
			rtarget(p, 0, breg[i]);
			if (i < variadic_lim(p, 8)) {
				p->op = LOAD+opkind(p->op);
				setreg(p, breg[i]);
			}
		}
		break;
	}
	case ARG+B:
		/* save instead of align */
		p->syms[1]->u.c.v.i = variadic_lim(p, 8);
		break;
	}
}

static void clobber(Node p) {
	assert(p);
	switch (specific(p->op)) {
	case ARG+B: {
		int size = p->syms[0]->u.c.v.i;
		int pos = p->syms[RX]->u.c.v.i;
		if (pos * 8 + size > 64)
			spill(7 << 8, IREG, p);
		break;
	}
	case MOD+I: case MOD+U:
		spill(1 << 8, IREG, p);
		break;
	case ASGN+B: case CALL+B:
		spill(7 << 8, IREG, p);
		break;
	}
}

static void doarg(Node p) {
	assert(p && p->syms[0]);
	p->syms[RX] = intconst(mkactual(8, p->syms[0]->u.c.v.i) / 8);
}

/* TODO: better implementation */
static void blkcopy_e2k(const char *dreg, int doff, const char *sreg, int soff, int size) {
	int i, tmp[3] = { 8, 9, 10 };
	if (dreg[0] == '!') {
		dreg++; doff += framesize;
		if (size <= 64) {
			int c2 = 0, rn = atoi(dreg + 1);
			for (i = 0; size > 0; size -= 8, i += 8) {
				char c = size > 4 ? 'd' : "0bhww"[size];
				if (i >> 3 == rn) c2 = c;
				else print("\tld%c\t%%%s, %d, %%r%d\n", c, sreg, soff + i, i >> 3);
			}
			if (c2)
				print("\tld%c\t%%%s, %d, %%r%d\n", c2, sreg, soff + rn * 8, rn);
			return;
		}
	}
	if (sreg[0] == '!') {
		sreg++;
		if (size <= 64) {
			for (i = 0; size > 0; size -= 8, i += 8) {
				char c = size > 4 ? 'd' : "0bhww"[size];
				print("\tst%c\t%%%s, %d, %%b[%d]\n", c, dreg, doff + i, i >> 3);
			}
			return;
		}
	}
	print("\t! memcpy(%s+%d, %s+%d, %d)\n", dreg, doff, sreg, soff, size);
	for (i = 0; size > 0; size -= 8, i += 8) {
		char c = size > 4 ? 'd' : "0bhww"[size];
		print("\tld%c\t%%%s, %d, %%r%d\n", c, sreg, soff + i, tmp[0]);
		print("\tst%c\t%%%s, %d, %%r%d\n", c, dreg, doff + i, tmp[0]);
	}
	print("\t! memcpy end\n");
}

static void emit2(Node p) {
	switch (p->op) {
	case ARG+B: {
		int size = p->syms[0]->u.c.v.i;
		int pos = p->syms[RX]->u.c.v.i;
		const char *src = p->x.kids[0]->syms[RX]->x.name;
		if (pos * 8 + size > 64) {
			blkcopy_e2k("r23", pos, src, 0, size);
		} else {
			int i, spill = p->syms[1]->u.c.v.i;
			for (i = 0; size > 0; size -= 8, i += 8, pos++) {
				char c = size > 4 ? 'd' : "0bhww"[size];
				print("\tld%c\t%%%s, %d, %%b[%d]\n", c, src, i, pos);
				if (pos >= spill)
					print("\tstd\t%%r23, %d, %%b[%d]\n", pos * 8, pos);
			}
		}
		break;
	}
	case ASGN+B: {
		const char *dst = p->x.kids[0]->syms[RX]->x.name;
		const char *src = p->x.kids[1]->syms[RX]->x.name;
		int size = p->syms[0]->u.c.v.i;
		// int align = p->syms[1]->u.c.v.i;
		blkcopy_e2k(dst, 0, src, 0, size);
		break;
	}
	case CALL+B: {
		const char *dst = p->kids[1]->syms[RX]->x.name;
		int size = p->syms[1]->u.c.v.i;
		emitasm(p->kids[0], _ctpr1_NT);
		print("\n\tcall\t%%ctpr1, wbs = 12\n");
		blkcopy_e2k(dst, 0, "!r23", 0, size);
		break;
	}
	}
}

static void local(Symbol p) {
	if (retstruct == 1) {
		assert(p == retv);
		p->x.name = "!r23";
		p->sclass = REGISTER;
		retstruct = 2;
		return;
	}
	if (askregvar(p, rmap(ttob(p->type))) == 0)
		mkauto(p);
}

static void function(Symbol f, Symbol caller[], Symbol callee[], int ncalls) {
	int i, autoargs = 0, spillregs = 0;

	usedmask[IREG] = usedmask[FREG] = 0;
	freemask[IREG] = freemask[FREG] = ~0u;
	offset = 0;
	for (i = 0; callee[i]; i++) {
		Symbol p = callee[i], q = caller[i];
		int size = roundup(q->type->size, 8);
		assert(q);
		if (offset + size > 64) {
			p->x.offset = q->x.offset = offset;
			p->x.name = q->x.name = stringd(offset);
			p->sclass = q->sclass = AUTO;
			autoargs = 1;
		} else if (isstruct(q->type)) {
			int j;
			p->x.offset = q->x.offset = offset;
			p->x.name = q->x.name = stringd(offset);
			p->sclass = q->sclass = AUTO;
			autoargs = 1;
			for (j = 0; j < size; j += 8)
				spillregs |= 1 << ((offset + j) >> 3);
		} else if (p->addressed) {
			p->x.offset = offset;
			p->x.name = stringd(offset);
			p->sclass = AUTO;
			q->sclass = REGISTER;
			askregvar(q, ireg[offset >> 3]);
			assert(q->x.regnode);
			q->type = p->type;
			autoargs = 1;
		} else {
			p->sclass = q->sclass = REGISTER;
			askregvar(p, ireg[offset >> 3]);
			assert(p->x.regnode);
			q->x.name = p->x.name;
			q->type = p->type;
		}
		offset += size;
	}
	assert(!caller[i]);
	maxargoffset = offset = maxoffset = 0;
	retstruct = isstruct(freturn(f->type));
	gencode(caller, callee);
	maxargoffset = roundup(maxargoffset, 8);
	framesize = roundup(maxoffset + maxargoffset, 16);
	print(f->sclass != STATIC ?
			"\t.global\t%s\n" : "\t.local\t%s\n", f->x.name);
	print("\t.type\t%s, #function\n", f->x.name);
	print("\t.align\t8\n%s:\n", f->x.name);
	print("\t{\n");
	if (ncalls) print(
			"\t  setwd\twsz = 16, nfx = 1, dbl = 0\n"
			"\t  setbn\trbs = 12, rsz = 3, rcur = 0\n");
	else print(
			"\t  setwd\twsz = 12, nfx = 1, dbl = 0\n");
	if (framesize || autoargs || retstruct)
		print("\t  getsp\t%d, %%r23\n", -framesize);
	print("\t}\n");
	for (i = 0; spillregs; spillregs >>= 1, i++)
		if (spillregs & 1)
			print("\tstd\t%%r23, %d+%d, %%r%d\n", i * 8, framesize, i);
	emitcode();
	print("\treturn\t%%ctpr3\n");
	print("\tct\t%%ctpr3\n");
	print("\t.size\t%s, .- %s\n", f->x.name, f->x.name);
}

static void defconst(int suffix, int size, Value v) {
	if (suffix == F && size == 4) {
		float f = v.d;
		print("\t.uaword\t0x%x\n", *(unsigned*)&f);
	} else if (suffix == F && size == 8) {
		double d = v.d;
		print("\t.uadword\t0x%X\n", *(unsigned long*)&d);
	} else if (suffix == P)
		print("\t.uadword\t0x%x\n", v.p);
	else if (size == 1)
		print("\t.byte\t0x%x\n", (unsigned char)(suffix == I ? v.i : v.u));
	else if (size == 2)
		print("\t.uahalf\t0x%x\n", (unsigned short)(suffix == I ? v.i : v.u));
	else if (size == 4)
		print("\t.uaword\t0x%x\n", (unsigned)(suffix == I ? v.i : v.u));
	else if (size == 8)
		print("\t.uadword\t0x%X\n", suffix == I ? v.i : v.u);
	else assert(0);
}

static void defaddress(Symbol p) {
	print("\t.uadword\t%s\n", p->x.name);
}

static void defstring(int n, char *str) {
	int i, a;
	for (i = 0; i < n; i++) {
		if (!(i & 31))
			print("\"\n\t.ascii\t\"" + !i * 2);
		a = str[i];
		switch (a) {
		case '"': case '\\': print("\\%c", a); break;
		case '\b': print("\\b"); break;
		case '\t': print("\\t"); break;
		case '\n': print("\\n"); break;
		case '\f': print("\\f"); break;
		case '\r': print("\\r"); break;
		default:
			if (a >= 32 && a < 127) print("%c", a);
			else print("\\%d%d%d", a >> 6 & 3, a >> 3 & 7, a & 7);
		}
	}
	if (i) print("\"\n");
}

static void address(Symbol q, Symbol p, long n) {
	if (p->scope == GLOBAL || p->sclass == STATIC || p->sclass == EXTERN)
		q->x.name = stringf("%s%s%D", p->x.name, n >= 0 ? "+" : "", n);
	else {
		assert(n <= INT_MAX && n >= INT_MIN);
		q->x.offset = p->x.offset + n;
		q->x.name = stringd(q->x.offset);
	}
}

static void export(Symbol p) { }
static void import(Symbol p) { }

static void defsymbol(Symbol p) {
	if (p->scope >= LOCAL && p->sclass == STATIC)
		p->x.name = stringf("%d", genlabel(1));
	else
		assert(p->scope != CONSTANTS || isint(p->type) || isptr(p->type)),

	p->x.name = p->name;
	if (p->scope >= LABELS)
		p->x.name = stringf(p->generated ? ".L%s" : "%s", p->x.name);
}

static void segment(int n) {
	if (n == cseg) return;
	cseg = n;
	if (cseg == CODE) print("\t.text\n");
	else if (cseg == BSS) print("\t.section .bss\n");
	else if (cseg == DATA) print("\t.data\n");
	else if (cseg == LIT) print("\t.section .rodata\n");
}

static void space(int n) {
	if (cseg != BSS) print("\t.skip\t0x%x\n", n);
}

static void global(Symbol p) {
	static const char tab[8] = { 1, 2, 4, 4, 8, 8, 8, 8 };
	unsigned n = p->type->size, align;
	assert(p->u.seg);
	align = n ? n < 9 ? tab[n - 1] : 16 : p->type->size;

	if (p->u.seg == BSS && p->sclass != STATIC) {
		print("\t.comm\t%s, 0x%x, %d\n", p->x.name, n, align);
	} else {
		print(p->sclass != STATIC ?
				"\t.global\t%s\n" : "\t.local\t%s\n", p->x.name);
		print("\t.type\t%s, #object\n", p->x.name);
		print("\t.size\t%s, 0x%x\n", p->x.name, n);
		print("\t.align\t%d\n%s:\n", align, p->x.name);
		if (p->u.seg == BSS)
			print("\t.skip\t0x%x\n", p->type->size);
	}
}

Interface e2kIR = {
	1, 1, 0,  /* char */
	2, 2, 0,  /* short */
	4, 4, 0,  /* int */
	8, 8, 0,  /* long */
	8, 8, 0,  /* long long */
	4, 4, 1,  /* float */
	8, 8, 1,  /* double */
	8, 8, 1,  /* long double */
	8, 8, 0,  /* T * */
	0, 1, 0,  /* struct */
	1,  /* little_endian */
	0,  /* mulops_calls */
	1,  /* wants_callb */
	1,  /* wants_argb */
	1,  /* left_to_right */
	0,  /* wants_dag */
	0,  /* unsigned_char */
	address,
	blockbeg,
	blockend,
	defaddress,
	defconst,
	defstring,
	defsymbol,
	emit,
	export,
	function,
	gen,
	global,
	import,
	local,
	progbeg,
	progend,
	segment,
	space,
	0, 0, 0, 0, 0, 0, 0,
	{
		1,  /* max_unaligned_load */
		rmap,
		0, 0, 0,    /* blkfetch, blkstore, blkloop */
		_label,
		_rule,
		_nts,
		_kids,
		_string,
		_templates,
		_isinstruction,
		_ntname,
		emit2,
		doarg,
		target,
		clobber,
	}
};
static char rcsid[] = "$Id$";
