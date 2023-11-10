#include "kettlingur.h"

#define fs ((opcode._u32[0] >> 11) & 0x1f)
#define ft ((opcode._u32[0] >> 16) & 0x1f)
#define fd ((opcode._u32[0] >> 6) & 0x1f)
#define dest ((opcode._u32[0] >> 21) & 0xf)


#define vtx ps2.vu0.vf[((opcode._u32[0] >> 16) & 0x1f)].x
#define vty ps2.vu0.vf[((opcode._u32[0] >> 16) & 0x1f)].y
#define vtz ps2.vu0.vf[((opcode._u32[0] >> 16) & 0x1f)].z
#define vtw ps2.vu0.vf[((opcode._u32[0] >> 16) & 0x1f)].w

#define vsx ps2.vu0.vf[((opcode._u32[0] >> 11) & 0x1f)].x
#define vsy ps2.vu0.vf[((opcode._u32[0] >> 11) & 0x1f)].y
#define vsz ps2.vu0.vf[((opcode._u32[0] >> 11) & 0x1f)].z
#define vsw ps2.vu0.vf[((opcode._u32[0] >> 11) & 0x1f)].w

#define vdx ps2.vu0.vf[((opcode._u32[0] >> 6) & 0x1f)].x
#define vdy ps2.vu0.vf[((opcode._u32[0] >> 6) & 0x1f)].y
#define vdz ps2.vu0.vf[((opcode._u32[0] >> 6) & 0x1f)].z
#define vdw ps2.vu0.vf[((opcode._u32[0] >> 6) & 0x1f)].w


#define accx ps2.vu0.acc.x
#define accy ps2.vu0.acc.y
#define accz ps2.vu0.acc.z
#define accw ps2.vu0.acc.w

#define vdy ps2.vu0.vf[((opcode._u32[0] >> 6) & 0x1f)].y
#define vdz ps2.vu0.vf[((opcode._u32[0] >> 6) & 0x1f)].z
#define vdw ps2.vu0.vf[((opcode._u32[0] >> 6) & 0x1f)].w

#define vtbc ps2.vu0.vf[((opcode._u32[0] >> 16) & 0x1f)]._f32[opcode._u32[0] & 0x3]

#define xen() dest & 1
#define yen() dest & 2
#define zen() dest & 4
#define wen() dest & 8

#define i ps2.vu0.i
#define q ps2.vu0.q
#define r ps2.vu0.r
#define p ps2.vu0.p

//upper micro
static void ABS(reg32 opcode){
	if(xen()) vtx = fabsf(vsx);
	if(yen()) vty = fabsf(vsy);
	if(zen()) vtz = fabsf(vsz);
	if(wen()) vtw = fabsf(vsw);
}
static void ADD(reg32 opcode){
	if(xen()) vdx = vsx + vtx;
	if(yen()) vdy = vsy + vty;
	if(zen()) vdz = vsz + vtz;
	if(wen()) vdw = vsw + vtw;
}
static void ADDi(reg32 opcode){
	if(xen()) vdx = vsx + i;
	if(yen()) vdy = vsy + i;
	if(zen()) vdz = vsz + i;
	if(wen()) vdw = vsw + i;
}
static void ADDq(reg32 opcode){
	if(xen()) vdx = vsx + q;
	if(yen()) vdy = vsy + q;
	if(zen()) vdz = vsz + q;
	if(wen()) vdw = vsw + q;
}
static void ADDbc(reg32 opcode){
	if(xen()) vdx = vsx + vtbc;
	if(yen()) vdy = vsy + vtbc;
	if(zen()) vdz = vsz + vtbc;
	if(wen()) vdw = vsw + vtbc;
}
static void ADDA(reg32 opcode){
		if(xen()) accx = vsx + vtx;
		if(yen()) accy = vsy + vty;
		if(zen()) accz = vsz + vtz;
		if(wen()) accw = vsw + vtw;
	}
static void ADDAi(reg32 opcode){
	if(xen()) accx = vsx + i;
	if(yen()) accy = vsy + i;
	if(zen()) accz = vsz + i;
	if(wen()) accw = vsw + i;}
static void ADDAq(reg32 opcode){
	if(xen()) accx = vsx + q;
	if(yen()) accy = vsy + q;
	if(zen()) accz = vsz + q;
	if(wen()) accw = vsw + q;}
static void ADDAbc(reg32 opcode){
	if(xen()) accx = vsx + vtbc;
	if(yen()) accy = vsy + vtbc;
	if(zen()) accz = vsz + vtbc;
	if(wen()) accw = vsw + vtbc;}
static void CLIP(reg32 opcode){}
static void FTOI0(reg32 opcode){}
static void FTOI4(reg32 opcode){}
static void FTOI12(reg32 opcode){}
static void FTOI15(reg32 opcode){}
static void ITOF0(reg32 opcode){}
static void ITOF4(reg32 opcode){}
static void ITOF12(reg32 opcode){}
static void ITOF15(reg32 opcode){}
static void MADD(reg32 opcode){}
static void MADDi(reg32 opcode){}
static void MADDq(reg32 opcode){}
static void MADDbc(reg32 opcode){}
static void MADDA(reg32 opcode){}
static void MADDAi(reg32 opcode){}
static void MADDAq(reg32 opcode){}
static void MADDAbc(reg32 opcode){}
static void MAX(reg32 opcode){}
static void MAXi(reg32 opcode){}
static void MAXbc(reg32 opcode){}
static void MIN(reg32 opcode){}
static void MINi(reg32 opcode){}
static void MINbc(reg32 opcode){}
static void MSUB(reg32 opcode){}
static void MSUBi(reg32 opcode){}
static void MSUBq(reg32 opcode){}
static void MSUBbc(reg32 opcode){}
static void MSUBA(reg32 opcode){}
static void MSUBAi(reg32 opcode){}
static void MSUBAq(reg32 opcode){}
static void MSUBAbc(reg32 opcode){}
static void MUL(reg32 opcode){}
static void MULi(reg32 opcode){}
static void MULq(reg32 opcode){}
static void MULbc(reg32 opcode){}
static void MULA(reg32 opcode){}
static void MULAi(reg32 opcode){}
static void MULAq(reg32 opcode){}
static void MULAbc(reg32 opcode){}
static void NOP(reg32 opcode){}
static void OPMULA(reg32 opcode){}
static void OPMSUB(reg32 opcode){}
static void SUB(reg32 opcode){}
static void SUBi(reg32 opcode){}
static void SUBq(reg32 opcode){}
static void SUBbc(reg32 opcode){}
static void SUBA(reg32 opcode){}
static void SUBAi(reg32 opcode){}
static void SUBAq(reg32 opcode){}
static void SUBAbc(reg32 opcode){}
//lower micro
static void B(reg32 opcode){}
static void BAL(reg32 opcode){}
static void DIV(reg32 opcode){}
static void EATAN(reg32 opcode){}
static void EATANxy(reg32 opcode){}
static void EATANxz(reg32 opcode){}
static void EEXP(reg32 opcode){}
static void ELENG(reg32 opcode){}
static void ERCPR(reg32 opcode){}
static void ERLENG(reg32 opcode){}
static void ERSADD(reg32 opcode){}
static void ESIN(reg32 opcode){}
static void ESQRT(reg32 opcode){}
static void ESUM(reg32 opcode){}
static void FCAND(reg32 opcode){}
static void FCEQ(reg32 opcode){}
static void FCGET(reg32 opcode){}
static void FCOR(reg32 opcode){}
static void FCSET(reg32 opcode){}
static void FMAND(reg32 opcode){}
static void FMEQ(reg32 opcode){}
static void FMOR(reg32 opcode){}
static void FSAND(reg32 opcode){}
static void FSEQ(reg32 opcode){}
static void FSOR(reg32 opcode){}
static void FSSET(reg32 opcode){}
static void IADD(reg32 opcode){}
static void IADDI(reg32 opcode){}
static void IADDIU(reg32 opcode){}

static void IAND(reg32 opcode){}
static void IBEQ(reg32 opcode){}
static void IBGEZ(reg32 opcode){}
static void IBGTZ(reg32 opcode){}
static void IBLEZ(reg32 opcode){}
static void IBLTZ(reg32 opcode){}
static void IBNE(reg32 opcode){}
static void ILW(reg32 opcode){}
static void ILWR(reg32 opcode){}
static void IOR(reg32 opcode){}
static void ISUB(reg32 opcode){}
static void ISUBIU(reg32 opcode){}
static void ISW(reg32 opcode){}
static void ISWR(reg32 opcode){}
static void JR(reg32 opcode){}
static void LQ(reg32 opcode){}
static void LQD(reg32 opcode){}
static void LDI(reg32 opcode){}
static void MFIR(reg32 opcode){}
static void MFP(reg32 opcode){}
static void MOVE(reg32 opcode){}
static void MR32(reg32 opcode){}
static void MTIR(reg32 opcode){}
static void RGET(reg32 opcode){}
static void RINIT(reg32 opcode){}
static void RNEXT(reg32 opcode){}
static void RSQRT(reg32 opcode){}
static void RXOR(reg32 opcode){}
static void SQ(reg32 opcode){}
static void SQD(reg32 opcode){}
static void SQI(reg32 opcode){}
static void SQRT(reg32 opcode){}

static void WAITP(reg32 opcode){}
static void WAITQ(reg32 opcode){}
static void XGKICK(reg32 opcode){}
static void XITOP(reg32 opcode){}
static void XTOP(reg32 opcode){}