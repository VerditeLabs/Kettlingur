#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;
typedef uint64_t u64;
typedef int64_t s64;
typedef __uint128_t u128;
typedef __int128_t s128;
typedef float f32;


#define EE_RAM_START 0x0
#define EE_RAM_SIZE (1024 * 1024 * 32)
#define EE_RAM_END (EE_RAM_START + EE_RAM_SIZE)
#define EE_RAM_MASK (EE_RAM_END - 1)

#define EE_RAM_UNCACHED_START 0x20000000
#define EE_RAM_UNCACHED_SIZE EE_RAM_SIZE
#define EE_RAM_UNCACHED_END (EE_RAM_UNCACHED_START + EE_RAM_UNCACHED_SIZE)

#define EE_RAM_ACCELERATED_START 0x30000000
#define EE_RAM_ACCELERATED_SIZE EE_RAM_SIZE
#define EE_RAM_ACCELERATED_END (EE_RAM_ACCELERATED_START + EE_RAM_ACCELERATED_SIZE)

#define EE_REGS_START 0x10000000
#define EE_REGS_SIZE 0x10000
#define EE_REGS_END (EE_REGS_START + EE_REGS_SIZE)

#define VU_REGS_START 0x11000000
#define VU_REGS_SIZE 0x10000
#define VU_REGS_END (VU_REGS_START + VU_REGS_SIZE)

#define GS_REGS_START 0x12000000
#define GS_REGS_SIZE 0x10000
#define GS_REGS_END (GS_REGS_START + GS_REGS_SIZE)

#define BIOS_START 0x1fc00000

#ifdef PSX_EMULATOR
#define BIOS_SIZE (1024 * 512)
#else
#define BIOS_SIZE (1024 * 1024 * 4)
#endif
#define BIOS_END (BIOS_START + BIOS_SIZE)

#define EE_IOP_RAM_START 0x1c000000
#define EE_IOP_RAM_SIZE (1024 * 1024 * 2)
#define EE_IOP_RAM_END (EE_IOP_RAM_START + EE_IOP_RAM_SIZE)

#define EE_SCRATCHPAD_START 0x70000000
#define EE_SCRATCHPAD_SIZE (1024 * 16)
#define EE_SCRATCHPAD_END (EE_SCRATCHPAD_START + EE_SCRATCHPAD_SIZE)

#define IOP_IOP_RAM_START 0x00000000
#define IOP_IOP_RAM_SIZE (1024 * 1024 * 2)
#define IOP_IOP_RAM_END (IOP_IOP_RAM_START + IOP_IOP_RAM_SIZE)
#define IOP_IOP_RAM_MASK (IOP_IOP_RAM_END - 1)

#define IOP_SCRATCHPAD_START 0x1f800000
#define IOP_SCRATCHPAD_SIZE (1024)
#define IOP_SCRATCHPAD_END (IOP_SCRATCHPAD_START + IOP_SCRATCHPAD_SIZE)

#define IOP_REGS_START (0x1f801000)
#define IOP_REGS_END (0x1f810000)
#define IOP_REGS_SIZE (IOP_REGS_END - IOP_REGS_START)

#define IOP_IO_START (0xfffe0000)
#define IOP_IO_SIZE (1024 * 4)
#define IOP_IO_END (IOP_IO_START + IOP_IO_SIZE)

#define IOP_EXPANSION_ONE_START (0x1f000000)
#define IOP_EXPANSION_ONE_SIZE (1024 * 1024 * 8)
#define IOP_EXPANSION_ONE_END (IOP_EXPANSION_ONE_START + IOP_EXPANSION_ONE_SIZE)

#define PSX_JOY_START 0x1f801040
#define PSX_JOY_END 0x1f801050

#define SBUS_START 0x1000F200
#define SBUS_END 0x1000F300

#define SBUS_PS1_START 0x1000F300
#define SBUS_PS1_END 0x1000F400

#define PSX_CDROM_START 0x1f801800
#define PSX_CDROM_END 0x1f801810

#define DMAC_START 0x10008000
#define DMAC_END 0x1000f000

#define DMAC_ADDTL_START 0x1000f500
#define DMAC_ADDTL_END 0x1000f600

#define FIFO_START 0x10004000
#define FIFO_END 0x10008000

#define GIF_START 0x10003000
#define GIF_END 0x10003800

#define INTC_START 0x1000f000
#define INTC_END 0x1000f100

#define SIF_START 0x1000f200
#define SIF_END 0x1000f300

#define SIO_START 0x1000f100
#define SIO_END 0x1000f200

#define TIMER_START 0x10000000
#define TIMER_END 0x10002000

#define VIF_START 0x10003800
#define VIF_END 0x10004000

#define MCH_START 0x1000F400
#define MCH_END 0x1000F500

#define IPU_START 0x10002000
#define IPU_END 0x10003000

#define PSX_DMAC_START 0x1f801080
#define PSX_DMAC_END 0x1f801100

#define PSX_GPU_START 0x1f801810
#define PSX_GPU_END 0x1f801820

#define PSX_INTERRUPTS_START 0x1f801070
#define PSX_INTERRUPTS_END 0x1f801080

#define PSX_MDEC_START 0x1f801820
#define PSX_MDEC_END 0x1f801830

#define PSX_SIO_START 0x1f801050
#define PSX_SIO_END 0x1f801060

#define PSX_SPU_START 0x1f801c00
#define PSX_SPU_END 0x1f802000

#define PSX_TIMER_START 0x1f801100
#define PSX_TIMER_END 0x1f801130


typedef union {
	u8 _u8[16];
	s8 _s8[16];
	u16 _u16[8];
	s16 _s16[8];
	u32 _u32[4];
	s32 _s32[4];
	u64 _u64[2];
	s64 _s64[2];
	u128 _u128[1];
	s128 _s128[1];
	f32 _f32[4];
} reg;


typedef union {
	u8 _u8[4];
	s8 _s8[4];
	u16 _u16[2];
	s16 _s16[2];
	u32 _u32[1];
	s32 _s32[1];
	f32 _f32[1];
} reg32;


typedef union {
	u8 _u8[2];
	s8 _s8[2];
	u16 _u16[1];
	s16 _s16[1];
} reg16;

union gpr{
	reg raw[32];
	struct {
		reg zero, at, v0, v1, a0,a1,a2,a3,t0,t1,t2,t3,t4,t5,t6,t7,s0,s1,s2,s3,s4,s5,s6,s7,t8,t9,k0,k1,gp,sp,fp,ra;
	};
};
union cop0 {
	reg raw[32];
	struct {
		reg zero, at, v0, v1, a0,a1,a2,a3,t0,t1,t2,t3,t4,t5,t6,t7,s0,s1,s2,s3,s4,s5,s6,s7,t8,t9,k0,k1,gp,sp,fp,ra;
	};
};
struct cop1 {
	reg32 fpr[32];
	reg32 fcr[32];
};

struct {
	struct {
		reg32 pc;
		union gpr gpr;
		reg lo, hi, barrelshift;
		u8* ram, *regs, *scratch, *bios, *gsregs;
		union cop0 cop0;
		struct cop1 cop1;
	} ee;

	struct {
		reg32 pc;
		union gpr gpr;
		reg32 lo, hi;
		union cop0 cop0;
		u8  *ram, *scratch, *bios, *regs;
	}iop;

	struct{
		reg vf[32];
		reg acc;
		f32 q,p;
		u32 mac;
		u32 clip;
		u32 status;
		u16 vi[16];
		u8 *code, *data;
	}vu0, vu1;


	struct {

	}gs;

} ps2;

#define rd() ((opcode._u32[0] >> 11) & 0x1f)
#define rs() ((opcode._u32[0] >> 21) & 0x1f)
#define rt() ((opcode._u32[0] >> 16) & 0x1f)
#define sa() ((opcode._u32[0] >> 6) & 0x1f)
#define base() rs()

#define gpr(dt, which, i) ps2.ee.gpr.raw[which()]._##dt[i]

#define rs32s gpr(s32, rs, 0)
#define rt32s gpr(s32, rt, 0)
#define rd32s gpr(s32, rd, 0)
#define rs32u gpr(s32, rs, 0)
#define rt32u gpr(s32, rt, 0)
#define rd32u gpr(s32, rd, 0)

#define rs64s gpr(s64, rs, 0)
#define rt64s gpr(s64, rt, 0)
#define rd64s gpr(s64, rd, 0)
#define rs64u gpr(s64, rs, 0)
#define rt64u gpr(s64, rt, 0)
#define rd64u gpr(s64, rd, 0)

#define rsi(dt, i) ps2.ee.gpr.raw[rs()]._##dt[i]
#define rti(dt, i) ps2.ee.gpr.raw[rt()]._##dt[i]
#define rdi(dt, i) ps2.ee.gpr.raw[rd()]._##dt[i]

#define base32s gpr(s32, base, 0)
#define base32u gpr(u32, base, 0)
#define base64s gpr(s64, base, 0)
#define base64u gpr(u64, base, 0)


#define loi(dt, i) ps2.ee.lo._##dt[i]
#define hii(dt, i) ps2.ee.hi._##dt[i]

#define lo32s loi(s32, 0)
#define hi32s hii(s32, 0)
#define lo32u loi(u32, 0)
#define hi32u hii(u32, 0)
#define lo64s loi(s64, 0)
#define hi64s hii(s64, 0)
#define lo64u loi(u64, 0)
#define hi64u hii(u64, 0)


#define pc32s ps2.ee.pc._s32[0]
#define pc32u ps2.ee.pc._u32[0]


#define ra32s ps2.ee.gpr.ra._s32[0]
#define ra32u ps2.ee.gpr.ra._u32[0]
#define ra64s ps2.ee.gpr.ra._s64[0]
#define ra64u ps2.ee.gpr.ra._u64[0]

#define simm16 (opcode._s16[0])
#define uimm16 (opcode._u16[0])
#define offset (((s32)(opcode._s16[0])) << 2)

#define ex(func) case (__COUNTER__-baseval): func(opcode); break;
#define null() case (__COUNTER__-baseval): invalid(); break;

void init(){
	ps2.ee.ram = aligned_alloc(4096, EE_RAM_SIZE);
	ps2.ee.scratch = aligned_alloc(4096, EE_SCRATCHPAD_SIZE);
	ps2.ee.bios = aligned_alloc(4096, BIOS_SIZE);
	ps2.ee.gsregs = aligned_alloc(4096, GS_REGS_SIZE);

	ps2.vu0.code = NULL;
	ps2.vu0.data = NULL;
	ps2.vu1.code = NULL;
	ps2.vu1.data = NULL;

	ps2.iop.ram = aligned_alloc(4096, IOP_IOP_RAM_SIZE);
	ps2.iop.scratch = aligned_alloc(4096, 4096); //TODO: how big is this?
	ps2.iop.bios = ps2.ee.bios;
	ps2.iop.regs = aligned_alloc(4096, IOP_REGS_SIZE);

	//values from pcsx2
	pc32u = 0xbfc00000;
	ps2.ee.cop0.config._u32[0] = 0x440;
	ps2.ee.cop0.status._u32[0] = 0x70400004; //0x10900000 <-- wrong; // COP0 enabled | BEV = 1 | TS = 1
	ps2.ee.cop0.prid._u32[0] = 0x00002e20; // PRevID = Revision ID, same as R5900
	ps2.ee.cop1.fcr[0]._u32[0] = 0x00002e00; // fpu Revision..
	ps2.ee.cop1.fcr[31]._u32[0] = 0x01000001; // fpu Status/Control
}

void fini(){
	free(ps2.iop.regs);
	//no free for ps2.iop.bios because it is also ps2.ee.bios
	free(ps2.iop.scratch);
	free(ps2.iop.ram);
	free(ps2.ee.gsregs);
	free(ps2.ee.bios);
	free(ps2.ee.scratch);
	free(ps2.ee.ram);
}

void* readwrite(u32 addr) {
	if(addr >= 0x80000000 && addr < 0xa0000000){
		addr -= 0x80000000;
	} else if(addr >= 0xa0000000 && addr < 0xc0000000){
		addr -= 0xa0000000;
	} else if(addr >= 0xc0000000 && addr < 0xe0000000) {
		addr -= 0xc0000000;
	} else {
		assert(false);
	}
	void *host = NULL;
	if (addr >= EE_RAM_START && addr < EE_RAM_END) {
		host = &ps2.ee.ram[addr - EE_RAM_START];
	} else if (addr >= EE_RAM_UNCACHED_START && addr < EE_RAM_UNCACHED_END) {
		host = &ps2.ee.ram[addr - EE_RAM_UNCACHED_START];
	} else if (addr >= EE_RAM_ACCELERATED_START && addr < EE_RAM_ACCELERATED_END) {
		host = &ps2.ee.ram[addr - EE_RAM_ACCELERATED_START];
	} else if (addr >= EE_REGS_START && addr < EE_REGS_END) {
		host = &ps2.ee.regs[addr - EE_REGS_START];
	} else if (addr >= VU_REGS_START && addr < VU_REGS_END) {
		return 0;
	} else if (addr >= GS_REGS_START && addr < GS_REGS_END) {
		return 0;
	} else if (addr >= BIOS_START && addr < BIOS_END) {
		host = &ps2.ee.bios[addr - EE_REGS_START];
	} else if (addr >= EE_SCRATCHPAD_START && addr < EE_SCRATCHPAD_END) {
		host = &ps2.ee.scratch[addr - EE_REGS_START];
	} else if (addr >= EE_IOP_RAM_START && addr < EE_IOP_RAM_END) {
		host = &ps2.iop.ram[addr - EE_IOP_RAM_START];
	}
	return host;
}

u8 memread8(u32 addr){
	return *(u8*)readwrite(addr);
}
u16 memread16(u32 addr){
	return *(u16*)readwrite(addr);
}
u32 memread32(u32 addr){
	return *(u32*)readwrite(addr);
}
u64 memread64(u32 addr){
	return *(u64*)readwrite(addr);
}
u128 memread128(u32 addr){
	return *(u128*)readwrite(addr);
}

void memwrite8(u32 addr, u8 data){
	 *(u8*)readwrite(addr) = data;
}
void memwrite16(u32 addr, u16 data){
	*(u16*)readwrite(addr) = data;
}
void memwrite32(u32 addr, u32 data){
	*(u32*)readwrite(addr) = data;
}
void memwrite64(u32 addr, u64 data){
	*(u64*)readwrite(addr) = data;
}
void memwrite128(u32 addr, u64 data){
	*(u128*)readwrite(addr) = data;
}

void exception();

void OPCODE(reg32 opcode);
void SPECIAL(reg32 opcode);
void REGIMM(reg32 opcode);
void COP0(reg32 opcode);
void COP1(reg32 opcode);
void COP2(reg32 opcode);
void MMI(reg32 opcode);
void MMI0(reg32 opcode);
void MMI1(reg32 opcode);
void MMI2(reg32 opcode);
void MMI3(reg32 opcode);

void SLL(reg32 opcode){
	rd64s = rt32s << sa();
}
void SRL(reg32 opcode){
	rd64s = (s32)(rt32u >> sa());
}
void SRA(reg32 opcode){
	rd64s = rt32s >> sa();
}
void SLLV(reg32 opcode){
	rd64s = rt32s << (rt32s & 0x1f);
}
void JR(reg32 opcode){
	pc32s = rs32s;
}
void JALR(reg32 opcode){
	rd64s = pc32s + 8;
	pc32s = rs32s;
}
void MOVZ(reg32 opcode){
	if(rt64s == 0){
		rd64s = rs64s;
	}
}
void MOVN(reg32 opcode) {
	if (rt64s != 0) {
		rd64s = rs64s;
	}
}
void SYSCALL(reg32 opcode);
void BREAK(reg32 opcode);
void SYNC(reg32 opcode);
void MFHI(reg32 opcode){
	rd64s = hi64s;
}
void MTHI(reg32 opcode){
	hi64s = rs64s;
}
void MFLO(reg32 opcode){
	rd64s = lo64s;
}
void MTLO(reg32 opcode){
	lo64s = rs64s;
}
void DSLLV(reg32 opcode){
	rd64u = rt64u << (rs64s & 0x3f);
}
void DSRLV(reg32 opcode) {
	rd64u = rt64u >> (rs64s & 0x3f);
}
void DSRAV(reg32 opcode){
	rd64s = rt64s >> (rs64s & 0x3f);
}
void MULT(reg32 opcode){
	reg ret;
	ret._s64[0] = (s64)rs32s * (s64)rt32s;
	lo64s = ret._s32[0];
	hi64s = ret._s32[1];
}
void MULTU(reg32 opcode){
	reg ret;
	ret._u64[0] = (u64)rs32u * (u64)rt32u;
	lo64s = ret._s32[0];
	hi64s = ret._s32[1];
}
void DIV(reg32 opcode){
	lo64s = (s64)rs32s / (s64)rt32s;
	hi64s = (s64)rs32s % (s64)rt32s;
}
void DIVU(reg32 opcode){
	lo64s = (s32)(((u64)rs32u) / ((u64)rt32u));
	hi64s = (s32)(((u64)rs32u) % ((u64)rt32u));
}
void ADD(reg32 opcode){
	s32 res;
	if(__builtin_add_overflow(rs32s, rt32s, &res)){
		exception(); return;
	}
	rd64s = res;
}
void ADDU(reg32 opcode){
	s32 res;
	if(!__builtin_add_overflow(rs32s, rt32s, &res)){
		rd64s = res;
	}
}
void SUB(reg32 opcode){
	s32 res;
	if(__builtin_sub_overflow(rs32s, rt32s, &res)){
		exception(); return;
	}
	rd64s = res;
}
void SUBU(reg32 opcode){
	s32 res;
	if(!__builtin_sub_overflow(rs32s, rt32s, &res)){
		rd64s = res;
	}
}
void AND(reg32 opcode){
	rd64u = rs64u & rt64u;
}
void OR(reg32 opcode){
	rd64u = rs64u | rt64u;
}
void XOR(reg32 opcode){
	rd64u = rs64u ^ rt64u;
}
void NOR(reg32 opcode){
	rd64u = ~(rs64u | rt64u);
}
void MFSA(reg32 opcode);
void MTSA(reg32 opcode);
void SLT(reg32 opcode){
	rd64s = rs64s < rt64s ? 1 : 0;
}
void SLTU(reg32 opcode){
	rd64s = rs64u < rt64u ? 1 : 0;
}
void DADD(reg32 opcode){
	s64 res;
	if(__builtin_add_overflow(rs64s, rt64s, &res)){
		exception(); return;
	}
	rd64s = res;
}
void DADDU(reg32 opcode){
	s64 res;
	if(!__builtin_add_overflow(rs64s, rt64s, &res)){
		rd64s = res;
	}
}
void DSUB(reg32 opcode){
	s64 res;
	if(__builtin_sub_overflow(rs64s, rt64s, &res)){
		exception(); return;
	}
	rd64s = res;
}
void DSUBU(reg32 opcode){
	s64 res;
	if(!__builtin_sub_overflow(rs64s, rt64s, &res)){
		rd64s = res;
	}
}
void TGE(reg32 opcode);
void TGEU(reg32 opcode);
void TLT(reg32 opcode);
void TLTU(reg32 opcode);
void TEQ(reg32 opcode);
void TNE(reg32 opcode);
void DSLL(reg32 opcode){
	rd64u = rt64u << sa();
}
void DSRL(reg32 opcode){
	rd64u = rt64u >> sa();
}
void DSRA(reg32 opcode){
	rd64s = rt64s >> sa();
}
void DSLL32(reg32 opcode){
	rd64u = rt64u << (sa()+32);
}
void DSRL32(reg32 opcode){
	rd64u = rt64u >> (sa()+32);
}
void DSRA32(reg32 opcode){
	rd64s = rt64s >> (sa()+32);
}
void BLTZ(reg32 opcode){
	if(rs64s < 0){
		pc32s += offset;
	}
}
void BGEZ(reg32 opcode){
	if(rs64s >= 0){
		pc32s += offset;
	}
}
void BLTZL(reg32 opcode){
	if(rs64s < 0){
		pc32s += offset;
	} else {
		pc32s += 4;
	}
}
void BGEZL(reg32 opcode){
	if(rs64s >= 0){
		pc32s += offset;
	} else {
		pc32s += 4;
	}
}
void TGEI(reg32 opcode);
void TGEIU(reg32 opcode);
void TLTI(reg32 opcode);
void TLTIU(reg32 opcode);
void TEQI(reg32 opcode);
void TNEI(reg32 opcode);
void BLTZAL(reg32 opcode){
	ra64s = pc32s + 8;
	if(rs64s < 0){
		pc32s += offset;
	}
}
void BGEZAL(reg32 opcode){
	ra64s = pc32s + 8;
	if(rs64s >= 0){
		pc32s += offset;
	}
}
void BLTZALL(reg32 opcode){
	ra64s = pc32s + 8;
	if(rs64s < 0){
		pc32s += offset;
	} else {
		pc32s += 4;
	}
}
void BGEZALL(reg32 opcode){
	ra64s = pc32s + 8;
	if(rs64s >= 0){
		pc32s += offset;
	} else {
		pc32s += 4;
	}
}
void MTSAB(reg32 opcode);
void MTSAH(reg32 opcode);
void MADD(reg32 opcode){
	reg res;
	res._s64[0] = ((s64)rs32s* (s64)rt32s) + (s64)((lo64u & 0xffffffffull) | (hi64u << 32));
	lo64s = res._s32[0];
	hi64s = res._s32[1];
	rd64s = res._s32[0];
}
void MADDU(reg32 opcode){
	reg res;
	res._u64[0] = ((u64)rs32u* (u64)rt32u) + ((lo64u & 0xffffffffull) | (hi64u << 32));
	lo64s = res._s32[0];
	hi64s = res._s32[1];
	rd64s = res._s32[0];
}
void PLZCW(reg32 opcode);
void MULT1(reg32 opcode);
void MULTU1(reg32 opcode);
void DIV1(reg32 opcode);
void DIVU1(reg32 opcode);
void MFC0(reg32 opcode);
void MTC0(reg32 opcode);
void BC0F(reg32 opcode);
void BC0T(reg32 opcode);
void BC0FL(reg32 opcode);
void BC0TL(reg32 opcode);
void TLBR(reg32 opcode);
void TLBWI(reg32 opcode);
void TLBWR(reg32 opcode);
void TLBP(reg32 opcode);
void ERET(reg32 opcode);
void EI(reg32 opcode);
void DI(reg32 opcode);
void MFC1(reg32 opcode);
void DMFC1(reg32 opcode);
void CFC1(reg32 opcode);
void MTC1(reg32 opcode);
void DMTC1(reg32 opcode);
void CTC1(reg32 opcode);
void BC1F(reg32 opcode);
void BC1T(reg32 opcode);
void BC1FL(reg32 opcode);
void BC1TL(reg32 opcode);
void MFC2(reg32 opcode);
void CFC2(reg32 opcode);
void MTC2(reg32 opcode);
void CTC2(reg32 opcode);
void BC1(reg32 opcode);
void BC2(reg32 opcode);
void LWC1(reg32 opcode);
void LWC2(reg32 opcode);
void PREF(reg32 opcode);
void LQC2(reg32 opcode);
void LD(reg32 opcode){
	rt64u = memread64(base32s + simm16);
}
void SWC1(reg32 opcode);
void SWC2(reg32 opcode);
void SQC2(reg32 opcode);
void SD(reg32 opcode){
	memwrite64(base32s + simm16, rt64u);
}
void J(reg32 opcode){
	pc32u = (pc32u & 0xf0000000) | ((opcode._u32[0] << 2) & 0x0fffffff);
}
void JAL(reg32 opcode){
	ra64u = pc32u + 8;
	pc32u = (pc32u & 0xf0000000) | ((opcode._u32[0] << 2) & 0x0fffffff);
}
void BEQ(reg32 opcode){
	if(rs64s == rt64s){
		pc32s += offset;
	}
}
void BNE(reg32 opcode){
	if(rs64s != rt64s){
		pc32s += offset;
	}
}
void BLEZ(reg32 opcode){
	if(rs64s <= 0){
		pc32s += offset;
	}
}
void BGTZ(reg32 opcode){
	if(rs64s > 0){
		pc32s += offset;
	}
}
void ADDI(reg32 opcode){
	s32 res;
	if(__builtin_add_overflow(rs32s, simm16, &res)){
		exception(); return;
	}
	rt64s = res;
}
void ADDIU(reg32 opcode){
	s32 res;
	if(!__builtin_add_overflow(rs32s, simm16, &res)){
		rt64s = res;
	}
}
void SLTI(reg32 opcode){
	rd64s = rs64s < simm16 ? 1 : 0;
}
void SLTIU(reg32 opcode){
	rd64s = rs64s < (u64)(s64)simm16 ? 1 : 0;
}
void ANDI(reg32 opcode){
	rt64u = rs64u & uimm16;
}
void ORI(reg32 opcode){
	rt64u = rs64u | uimm16;
}
void XORI(reg32 opcode){
	rt64u = rs64u ^ uimm16;
}
void LUI(reg32 opcode){
	rt64s = ((s32)simm16) << 16;
}
void BEQL(reg32 opcode){
	if(rs64s == rt64s){
		pc32s += offset;
	} else {
		pc32s +=4;
	}
}
void BNEL(reg32 opcode){
	if(rs64s != rt64s){
		pc32s += offset;
	} else {
		pc32s +=4;
	}
}
void BLEZL(reg32 opcode){
	if(rs64s <= 0){
		pc32s += offset;
	} else {
		pc32s +=4;
	}
}
void BGTZL(reg32 opcode){
	if(rs64s > 0){
		pc32s += offset;
	} else {
		pc32s +=4;
	}
}
void DADDI(reg32 opcode){
	s64 res;
	if(__builtin_add_overflow(rs64s, simm16, &res)){
		exception(); return;
	}
	rt64s = res;
}
void DADDIU(reg32 opcode){
	s64 res;
	if(!__builtin_add_overflow(rs64s, simm16, &res)){
		rt64s = res;
	}
}
void LDL(reg32 opcode);
void LDR(reg32 opcode);
void LQ(reg32 opcode);
void SQ(reg32 opcode);
void LB(reg32 opcode){
	rt64s = (s32)memread8(base32s + simm16);
}
void LH(reg32 opcode){
	rt64s = (s32)memread16(base32s + simm16);
}
void LWL(reg32 opcode);
void LW(reg32 opcode){
	rt64s = (s32)memread32(base32s + simm16);
}
void LBU(reg32 opcode){
	rt64u = memread8(base32s + simm16);
}
void LHU(reg32 opcode){
	rt64u = memread16(base32s + simm16);
}
void LWR(reg32 opcode);
void LWU(reg32 opcode);
void SB(reg32 opcode){
	memwrite8(base32s + simm16, (u8)rt32u);
}
void SH(reg32 opcode){
	memwrite16(base32s + simm16, (u16)rt32u);
}
void SWL(reg32 opcode);
void SW(reg32 opcode){
	memwrite32(base32s + simm16, (u32)rt32u);
}
void SDL(reg32 opcode);
void SDR(reg32 opcode);
void SWR(reg32 opcode);
void CACHE(reg32 opcode);
void PADDW(reg32 opcode){
	rdi(u32,0) = rsi(u32,0) + rti(u32,0);
	rdi(u32,1) = rsi(u32,1) + rti(u32,1);
	rdi(u32,2) = rsi(u32,2) + rti(u32,2);
	rdi(u32,3) = rsi(u32,3) + rti(u32,3);
}
void PSUBW(reg32 opcode){
	rdi(s32,0) = rsi(s32,0) + rti(s32,0);
	rdi(s32,1) = rsi(s32,1) + rti(s32,1);
	rdi(s32,2) = rsi(s32,2) + rti(s32,2);
	rdi(s32,3) = rsi(s32,3) + rti(s32,3);
}
void PCGTW(reg32 opcode);
void PMAXW(reg32 opcode);
void PADDH(reg32 opcode);
void PSUBH(reg32 opcode);
void PCGTH(reg32 opcode);
void PMAXH(reg32 opcode);
void PADDB(reg32 opcode);
void PSUBB(reg32 opcode);
void PCGTB(reg32 opcode);
void PADDSW(reg32 opcode);
void PSUBSW(reg32 opcode);
void PEXTLW(reg32 opcode);
void PPACW(reg32 opcode);
void PADDSH(reg32 opcode);
void PSUBSH(reg32 opcode);
void PEXTLH(reg32 opcode);
void PPACH(reg32 opcode);
void PADDSB(reg32 opcode);
void PSUBSB(reg32 opcode);
void PEXTLB(reg32 opcode);
void PPACB(reg32 opcode);
void PEXT5(reg32 opcode);
void PPAC5(reg32 opcode);
void PABSW(reg32 opcode);
void PCEQW(reg32 opcode);
void PMINW(reg32 opcode);
void PADSBH(reg32 opcode);
void PABSH(reg32 opcode);
void PCEQH(reg32 opcode);
void PMINH(reg32 opcode);
void PCEQB(reg32 opcode);
void PADDUW(reg32 opcode);
void PSUBUW(reg32 opcode);
void PEXTUW(reg32 opcode);
void PADDUH(reg32 opcode);
void PSUBUH(reg32 opcode);
void PEXTUH(reg32 opcode);
void PADDUB(reg32 opcode);
void PSUBUB(reg32 opcode);
void PEXTUB(reg32 opcode);
void QFSRV(reg32 opcode);
void PMADDW(reg32 opcode);
void PSLLVW(reg32 opcode);
void PSRLVW(reg32 opcode);
void PMSUBW(reg32 opcode);
void PMFHI(reg32 opcode);
void PMFLO(reg32 opcode);
void PINTH(reg32 opcode);
void PMULTW(reg32 opcode);
void PDIVW(reg32 opcode);
void PCPYLD(reg32 opcode);
void PMADDH(reg32 opcode);
void PHMADH(reg32 opcode);
void PAND(reg32 opcode){
	rdi(u128,0) = rsi(u128,0) & rti(u128,0);
}
void PXOR(reg32 opcode){
	rdi(u128,0) = rsi(u128,0) ^ rti(u128,0);
}
void PMSUBH(reg32 opcode);
void PHMSBH(reg32 opcode);
void PEXEH(reg32 opcode);
void PREVH(reg32 opcode);
void PMULTH(reg32 opcode);
void PDIVBW(reg32 opcode);
void PEXEW(reg32 opcode);
void PROT3W(reg32 opcode);
void PMADDUW(reg32 opcode);
void PSRAVW(reg32 opcode);
void PMTHI(reg32 opcode);
void PMTLO(reg32 opcode);
void PINTEH(reg32 opcode);
void PMULTUW(reg32 opcode);
void PDIVUW(reg32 opcode);
void PCPYUD(reg32 opcode);
void POR(reg32 opcode);
void PNOR(reg32 opcode);
void PEXCH(reg32 opcode);
void PCPYH(reg32 opcode);
void PEXCW(reg32 opcode);
void PMFHL(reg32 opcode);
void PMTHL(reg32 opcode);
void PSLLH(reg32 opcode);
void PSRLH(reg32 opcode);
void PSRAH(reg32 opcode);
void PSLLW(reg32 opcode);
void PSRLW(reg32 opcode);
void PSRAW(reg32 opcode);
void SRLV(reg32 opcode);
void SRAV(reg32 opcode);
void MFHI1(reg32 opcode);
void MTHI1(reg32 opcode);
void MFLO1(reg32 opcode);
void MTLO1(reg32 opcode);
void MADD1(reg32 opcode);
void MADDU1(reg32 opcode);
void TLB(reg32 opcode);
void ADD_S(reg32 opcode);
void SUB_S(reg32 opcode);
void MUL_S(reg32 opcode);
void DIV_S(reg32 opcode);
void SQRT_S(reg32 opcode);
void ABS_S(reg32 opcode);
void MOV_S(reg32 opcode);
void NEG_S(reg32 opcode);
void SUBA_S(reg32 opcode);
void MULA_S(reg32 opcode);
void MADD_S(reg32 opcode);
void MSUB_S(reg32 opcode);
void MADDA_S(reg32 opcode);
void MSUBA_S(reg32 opcode);
void CVT_W_S(reg32 opcode);
void MAX_S(reg32 opcode);
void MIN_S(reg32 opcode);
void C_F_S(reg32 opcode);
void C_EQ_S(reg32 opcode);

void C_LT_S(reg32 opcode);
void C_LE_S(reg32 opcode);
void RSQRT_S(reg32 opcode);
void CVT_S(reg32 opcode);
void CVT_S_W(reg32 opcode);
void ADDA_S(reg32 opcode);




void invalid() { /* Handle invalid opcode */ }


void OPCODE(reg32 opcode) {
	static const int baseval = __COUNTER__ + 1;

	switch (opcode._u32[0] >> 26) {
		ex(SPECIAL)
		ex(REGIMM)
		ex(J)
		ex(JAL)
		ex(BEQ)
		ex(BNE)
		ex(BLEZ)
		ex(BGTZ)
		ex(ADDI)
		ex(ADDIU)
		ex(SLTI)
		ex(SLTIU)
		ex(ANDI)
		ex(ORI)
		ex(XORI)
		ex(LUI)
		ex(COP0)
		ex(COP1)
		ex(COP2)
		null()
		ex(BEQL)
		ex(BNEL)
		ex(BLEZL)
		ex(BGTZL)
		ex(DADDI)
		ex(DADDIU)
		ex(LDL)
		ex(LDR)
		ex(MMI)
		null()
		ex(LQ)
		ex(SQ)
		ex(LB)
		ex(LH)
		ex(LWL)
		ex(LW)
		ex(LBU)
		ex(LHU)
		ex(LWR)
		ex(LWU)
		ex(SB)
		ex(SH)
		ex(SWL)
		ex(SW)
		ex(SDL)
		ex(SDR)
		ex(SWR)
		ex(CACHE)
		null()
		ex(LWC1)
		null()
		ex(PREF)
		null()
		null()
		ex(LQC2)
		ex(LD)
		null()
		ex(SWC1)
		null()
		null()
		null()
		null()
		ex(SQC2)
		ex(SD)
	}
}

void SPECIAL(reg32 opcode) {
	static const int baseval = __COUNTER__ + 1;

	switch (opcode._u32[0] & 0x3f) {
		ex(SLL)
		null()
		ex(SRL)
		ex(SRA)
		ex(SLLV)
		null()
		ex(SRLV)
		ex(SRAV)
		ex(JR)
		ex(JALR)
		ex(MOVZ)
		ex(MOVN)
		ex(SYSCALL)
		ex(BREAK)
		null()
		ex(SYNC)
		ex(MFHI)
		ex(MTHI)
		ex(MFLO)
		ex(MTLO)
		ex(DSLLV)
		null()
		ex(DSRLV)
		ex(DSRAV)
		ex(MULT)
		ex(MULTU)
		ex(DIV)
		ex(DIVU)
		null()
		null()
		null()
		null()
		ex(ADD)
		ex(ADDU)
		ex(SUB)
		ex(SUBU)
		ex(AND)
		ex(OR)
		ex(XOR)
		ex(NOR)
		ex(MFSA)
		ex(MTSA)
		ex(SLT)
		ex(SLTU)
		ex(DADD)
		ex(DADDU)
		ex(DSUB)
		ex(DSUBU)
		ex(TGE)
		ex(TGEU)
		ex(TLT)
		ex(TLTU)
		ex(TEQ)
		null()
		ex(TNE)
		null()
		ex(DSLL)
		null()
		ex(DSRL)
		ex(DSRA)
		ex(DSLL32)
		null()
		ex(DSRL32)
		ex(DSRA32)
	}
}

void REGIMM(reg32 opcode) {
	static const int baseval = __COUNTER__ + 1;

	switch ((opcode._u32[0]  >> 16) & 0x1f) {
		ex(BLTZ)
		ex(BGEZ)
		ex(BLTZL)
		ex(BGEZL)
		null()
		null()
		null()
		null()
		ex(TGEI)
		ex(TGEIU)
		ex(TLTI)
		ex(TLTIU)
		ex(TEQI)
		null()
		ex(TNEI)
		null()
		ex(BLTZAL)
		ex(BGEZAL)
		ex(BLTZALL)
		ex(BGEZALL)
		null()
		null()
		null()
		null()
		ex(MTSAB)
		ex(MTSAH)
		null()
		null()
		null()
		null()
		null()
		null()
	}
}

void MMI(reg32 opcode) {
	static const int baseval = __COUNTER__ + 1;

	switch (opcode._u32[0] & 0x3f) {
		ex(MADD)
		ex(MADDU)
		null()
		null()
		ex(PLZCW)
		null()
		null()
		null()
		ex(MMI0)
		ex(MMI2)
		null()
		null()
		null()
		null()
		null()
		null()
		ex(MFHI1)
		ex(MTHI1)
		ex(MFLO1)
		ex(MTLO1)
		null()
		null()
		null()
		null()
		ex(MULT1)
		ex(MULTU1)
		ex(DIV1)
		ex(DIVU1)
		null()
		null()
		null()
		null()
		ex(MADD1)
		ex(MADDU1)
		null()
		null()
		null()
		null()
		null()
		null()
		ex(MMI1)
		ex(MMI3)
		null()
		null()
		null()
		null()
		null()
		null()
		ex(PMFHL)
		ex(PMTHL)
		null()
		null()
		ex(PSLLH)
		null()
		ex(PSRLH)
		ex(PSRAH)
		null()
		null()
		null()
		null()
		ex(PSLLW)
		null()
		ex(PSRLW)
		ex(PSRAW)
	}
}


void MMI0(reg32 opcode) {
	static const int baseval = __COUNTER__ + 1;

	switch ((opcode._u32[0] >>6) & 0x1f) {
		ex(PADDW)
		ex(PSUBW)
		ex(PCGTW)
		ex(PMAXW)
		ex(PADDH)
		ex(PSUBH)
		ex(PCGTH)
		ex(PMAXH)
		ex(PADDB)
		ex(PSUBB)
		ex(PCGTB)
		null()
		null()
		null()
		null()
		null()
		ex(PADDSW)
		ex(PSUBSW)
		ex(PEXTLW)
		ex(PPACW)
		ex(PADDSH)
		ex(PSUBSH)
		ex(PEXTLH)
		ex(PPACH)
		ex(PADDSB)
		ex(PSUBSB)
		ex(PEXTLB)
		ex(PPACB)
		null()
		null()
		ex(PEXT5)
		ex(PPAC5)
	}
}

void MMI1(reg32 opcode) {
	static const int baseval = __COUNTER__ + 1;

	switch ((opcode._u32[0] >>6) & 0x1f) {
		null()
		ex(PABSW)
		ex(PCEQW)
		ex(PMINW)
		ex(PADSBH)
		ex(PABSH)
		ex(PCEQH)
		ex(PMINH)
		null()
		null()
		ex(PCEQB)
		null()
		null()
		null()
		null()
		null()
		ex(PADDUW)
		ex(PSUBUW)
		ex(PEXTUW)
		null()
		ex(PADDUH)
		ex(PSUBUH)
		ex(PEXTUH)
		null()
		ex(PADDUB)
		ex(PSUBUB)
		ex(PEXTUB)
		ex(QFSRV)
		null()
		null()
		null()
		null()
	}
}


void MMI2(reg32 opcode) {
	static const int baseval = __COUNTER__ + 1;

	switch ((opcode._u32[0] >>6) & 0x1f) {
		ex(PMADDW)
		null()
		ex(PSLLVW)
		ex(PSRLVW)
		ex(PMSUBW)
		null()
		null()
		null()
		ex(PMFHI)
		ex(PMFLO)
		ex(PINTH)
		null()
		ex(PMULTW)
		ex(PDIVW)
		ex(PCPYLD)
		null()
		ex(PMADDH)
		ex(PHMADH)
		ex(PAND)
		ex(PXOR)
		ex(PMSUBH)
		ex(PHMSBH)
		null()
		null()
		null()
		null()
		ex(PEXEH)
		ex(PREVH)
		ex(PMULTH)
		ex(PDIVBW)
		ex(PEXEW)
		ex(PROT3W)
	}
}


void MMI3(reg32 opcode) {
	static const int baseval = __COUNTER__ + 1;

	switch ((opcode._u32[0] >>6) & 0x1f) {
		ex(PMADDUW)
		null()
		null()
		ex(PSRAVW)
		null()
		null()
		null()
		null()
		ex(PMTHI)
		ex(PMTLO)
		ex(PINTEH)
		null()
		ex(PMULTUW)
		ex(PDIVUW)
		ex(PCPYUD)
		null()
		null()
		null()
		ex(POR)
		ex(PNOR)
		null()
		null()
		null()
		null()
		null()
		null()
		ex(PEXCH)
		ex(PCPYH)
		null()
		null()
		ex(PEXCW)
		null()
	}
}

void BC0(reg32 opcode) {
	static const int baseval = __COUNTER__ + 1;
	switch ((opcode._u32[0] >> 16) & 0x1f) {
		ex(BC0F)
		ex(BC0T)
		ex(BC0FL)
		ex(BC0TL)
	}
}

void COP0(reg32 opcode) {
	static const int baseval = __COUNTER__ + 1;

	switch ((opcode._u32[0] >> 21)&0x1f) {
		// 000
		ex(MFC0) null() null() null() ex(MTC0) null() null() null()
		// 001
		ex(BC0) null() null() null() null() null() null() null()
		// 010
		ex(TLB) null() null() null() null() null() null() null()
		// 011
		null() null() null() null() null() null() null() null()
		// 100
		null() null() null() null() null() null() null() null()
		// 101
		null() null() null() null() null() null() null() null()
		// 110
		null() null() null() null() null() null() null() null()
		// 111
		null() null() null() null() null() null() null() null()
	}
}

void TLB(reg32 opcode) {
	static const int baseval = __COUNTER__ + 1;

	switch (opcode._u32[0] & 0x3f) {
		ex(TLBR)
		ex(TLBWI)
		null()
		null()
		null()
		null()
		ex(TLBWR)
		null()
		null()
		null()
		null()
		null()
		null()
		null()
		null()
		null()
		null()
		ex(ERET)
		null()
		null()
		null()
		null()
		null()
		null()
		null()
		ex(EI)
		ex(DI)
	}
}

void COP1(reg32 opcode) {
	static const int baseval = __COUNTER__ + 1;

	switch ((opcode._u32[0] >> 21)&0x1f) {
		ex(MFC1)
		null()
		ex(CFC1)
		null()
		ex(MTC1)
		null()
		ex(CTC1)
		null()
		null()
		null()
		null()
		ex(CVT_S)
		null()
		null()
		null()
		null()
	}
}

void BC1(reg32 opcode) {
	static const int baseval = __COUNTER__ + 1;

	switch ((opcode._u32[0] >>16)&0x1f) {
		ex(BC1F)
		ex(BC1T)
		ex(BC1FL)
		ex(BC1TL)
		null()
		null()
		null()
		null()
	}
}

void FPU_S(reg32 opcode) {
	static const int baseval = __COUNTER__ + 1;

	switch (opcode._u32[0] & 0x3f) {
		ex(ADD_S)
		ex(SUB_S)
		ex(MUL_S)
		ex(DIV_S)
		ex(SQRT_S)
		ex(ABS_S)
		ex(MOV_S)
		ex(NEG_S)
		null()
		null()
		ex(RSQRT_S)
		null()
		ex(ADDA_S)
		ex(SUBA_S)
		ex(MULA_S)
		null()
		ex(MADD_S)
		ex(MSUB_S)
		ex(MADDA_S)
		ex(MSUBA_S)
		null()
		null()
		ex(CVT_W_S)
		null()
		null()
		null()
		null()
		null()
		null()
		ex(MAX_S)
		ex(MIN_S)
		null()
		null()
		ex(C_F_S)
		null()
		ex(C_EQ_S)
		null()
		ex(C_LT_S)
		null()
		ex(C_LE_S)
		null()
		null()
		null()
		null()
	}
}

void FPU_W(reg32 opcode) {
	static const int baseval = __COUNTER__ + 1;

	switch (opcode._u32[0] & 0x3f) {
		null()
		null()
		null()
		null()
		null()
		null()
		null()
		null()
		null()
		null()
		null()
		null()
		null()
		null()
		null()
		null()
		ex(CVT_S_W)
		null()
		null()
		null()
		null()
		null()
		null()
		null()
		null()
		null()
		null()
		null()
		null()
		null()
		null()
		null()
	}
}

int main(int argc, char** argv){
	init();
	fini();
}