#include "kettlingur.h"
#include "shorthand.h"

static inline void branch(u32 addr){
	ps2.ee._in_branch_delay = true;
	ps2.ee._pc_latch._u32[0] = addr;

}

static inline void *readwrite(u32 addr) {
	void *host = NULL;
		if (addr >= 0x80000000 && addr < 0xa0000000) {
			addr -= 0x80000000;
		} else if (addr >= 0xa0000000 && addr < 0xc0000000) {
			addr -= 0xa0000000;
		} else if (addr >= 0xc0000000 && addr < 0xe0000000) {
			addr -= 0xc0000000;
		} else {
			assert(false);
		}
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
			host = &ps2.ee.bios[addr - BIOS_START];
		} else if (addr >= EE_SCRATCHPAD_START && addr < EE_SCRATCHPAD_END) {
			host = &ps2.ee.scratch[addr - EE_REGS_START];
		} else if (addr >= EE_IOP_RAM_START && addr < EE_IOP_RAM_END) {
			host = &ps2.iop.ram[addr - EE_IOP_RAM_START];
		}
		assert(addr != 0xb000f500);
		assert(host != NULL);
		return host;
	}

static inline void exception() {}

static inline u8 memread8(u32 addr) {
	u8* a = readwrite(addr);
	return *a ;
}

static inline u16 memread16(u32 addr) {
	u16* a = readwrite(addr);
	return *a;
}

u32 memread32(u32 addr) {
	u32* a = readwrite(addr);
	return *a;
}

static inline u64 memread64(u32 addr) {
	u64* a = readwrite(addr);
	return *a;
}

static inline u128 memread128(u32 addr) {
	return *(u128 *) readwrite(addr);
}

static inline void memwrite8(u32 addr, u8 data) {
	u8* a = readwrite(addr);
	*a = data;
}

static inline void memwrite16(u32 addr, u16 data) {
	u16* a = readwrite(addr);
	*a = data;
}

static inline void memwrite32(u32 addr, u32 data) {
	u32* a = readwrite(addr);
	*a = data;
}

static inline void memwrite64(u32 addr, u64 data) {
	u64* a = readwrite(addr);
	*a = data;
}

static inline void memwrite128(u32 addr, u64 data) {
	*(u128 *) readwrite(addr) = data;
}



static inline void OPCODE(reg32 opcode);

static inline void SPECIAL(reg32 opcode);

static inline void REGIMM(reg32 opcode);

static inline void COP0(reg32 opcode);

static inline void COP1(reg32 opcode);

static inline void COP2(reg32 opcode);

static inline void MMI(reg32 opcode);

static inline void MMI0(reg32 opcode);

static inline void MMI1(reg32 opcode);

static inline void MMI2(reg32 opcode);

static inline void MMI3(reg32 opcode);

static inline void BC1(reg32 opcode);

static inline void TLB(reg32 opcode);

static inline void COP2(reg32 opcode) {}

static inline void SLL(reg32 opcode) {
	rd64s = rt32s << sa();
}

static inline void SRL(reg32 opcode) {
	rd64s = (s32) (rt32u >> sa());
}

static inline void SRA(reg32 opcode) {
	rd64s = rt32s >> sa();
}

static inline void SLLV(reg32 opcode) {
	rd64s = rt32s << (rt32s & 0x1f);
}

static inline void JR(reg32 opcode) {
	branch(rs32s);
}

static inline void JALR(reg32 opcode) {
	rd64s = pc32s + 8;
	branch(rs32s);
}

static inline void MOVZ(reg32 opcode) {
	if (rt64s == 0) {
		rd64s = rs64s;
	}
}

static inline void MOVN(reg32 opcode) {
	if (rt64s != 0) {
		rd64s = rs64s;
	}
}

static inline void SYSCALL(reg32 opcode) {}

static inline void BREAK(reg32 opcode) {}

static inline void SYNC(reg32 opcode) {}

static inline void MFHI(reg32 opcode) {
	rd64s = hi64s;
}

static inline void MTHI(reg32 opcode) {
	hi64s = rs64s;
}

static inline void MFLO(reg32 opcode) {
	rd64s = lo64s;
}

static inline void MTLO(reg32 opcode) {
	lo64s = rs64s;
}

static inline void DSLLV(reg32 opcode) {
	rd64u = rt64u << (rs64s & 0x3f);
}

static inline void DSRLV(reg32 opcode) {
	rd64u = rt64u >> (rs64s & 0x3f);
}

static inline void DSRAV(reg32 opcode) {
	rd64s = rt64s >> (rs64s & 0x3f);
}

static inline void MULT(reg32 opcode) {
	reg ret;
	ret._s64[0] = (s64) rs32s * (s64) rt32s;
	lo64s = ret._s32[0];
	hi64s = ret._s32[1];
}

static inline void MULTU(reg32 opcode) {
	reg ret;
	ret._u64[0] = (u64) rs32u * (u64) rt32u;
	lo64s = ret._s32[0];
	hi64s = ret._s32[1];
}

static inline void DIV(reg32 opcode) {
	lo64s = (s64) rs32s / (s64) rt32s;
	hi64s = (s64) rs32s % (s64) rt32s;
}

static inline void DIVU(reg32 opcode) {
	lo64s = (s32) (((u64) rs32u) / ((u64) rt32u));
	hi64s = (s32) (((u64) rs32u) % ((u64) rt32u));
}

static inline void ADD(reg32 opcode) {
	s32 res;
	if (__builtin_add_overflow(rs32s, rt32s, &res)) {
		exception();
		return;
	}
	rd64s = res;
}

static inline void ADDU(reg32 opcode) {
	s32 res;
	if (!__builtin_add_overflow(rs32s, rt32s, &res)) {
		rd64s = res;
	}
}

static inline void SUB(reg32 opcode) {
	s32 res;
	if (__builtin_sub_overflow(rs32s, rt32s, &res)) {
		exception();
		return;
	}
	rd64s = res;
}

static inline void SUBU(reg32 opcode) {
	s32 res;
	if (!__builtin_sub_overflow(rs32s, rt32s, &res)) {
		rd64s = res;
	}
}

static inline void AND(reg32 opcode) {
	rd64u = rs64u & rt64u;
}

static inline void OR(reg32 opcode) {
	rd64u = rs64u | rt64u;
}

static inline void XOR(reg32 opcode) {
	rd64u = rs64u ^ rt64u;
}

static inline void NOR(reg32 opcode) {
	rd64u = ~(rs64u | rt64u);
}

static inline void MFSA(reg32 opcode) {
	rd64u = ps2.ee.barrelshift._u64[0];
}

static inline void MTSA(reg32 opcode) {
	ps2.ee.barrelshift._u64[0] = rs64u;
}

static inline void SLT(reg32 opcode) {
	rd64s = rs64s < rt64s ? 1 : 0;
}

static inline void SLTU(reg32 opcode) {
	rd64s = rs64u < rt64u ? 1 : 0;
}

static inline void DADD(reg32 opcode) {
	s64 res;
	if (__builtin_add_overflow(rs64s, rt64s, &res)) {
		exception();
		return;
	}
	rd64s = res;
}

static inline void DADDU(reg32 opcode) {
	s64 res;
	if (!__builtin_add_overflow(rs64s, rt64s, &res)) {
		rd64s = res;
	}
}

static inline void DSUB(reg32 opcode) {
	s64 res;
	if (__builtin_sub_overflow(rs64s, rt64s, &res)) {
		exception();
		return;
	}
	rd64s = res;
}

static inline void DSUBU(reg32 opcode) {
	s64 res;
	if (!__builtin_sub_overflow(rs64s, rt64s, &res)) {
		rd64s = res;
	}
}

static inline void TGE(reg32 opcode) {}

static inline void TGEU(reg32 opcode) {}

static inline void TLT(reg32 opcode) {}

static inline void TLTU(reg32 opcode) {}

static inline void TEQ(reg32 opcode) {}

static inline void TNE(reg32 opcode) {}

static inline void DSLL(reg32 opcode) {
	rd64u = rt64u << sa();
}

static inline void DSRL(reg32 opcode) {
	rd64u = rt64u >> sa();
}

static inline void DSRA(reg32 opcode) {
	rd64s = rt64s >> sa();
}

static inline void DSLL32(reg32 opcode) {
	rd64u = rt64u << (sa() + 32);
}

static inline void DSRL32(reg32 opcode) {
	rd64u = rt64u >> (sa() + 32);
}

static inline void DSRA32(reg32 opcode) {
	rd64s = rt64s >> (sa() + 32);
}

static inline void BLTZ(reg32 opcode) {
	if (rs64s < 0) {
		pc32s += offset;
	}
}

static inline void BGEZ(reg32 opcode) {
	if (rs64s >= 0) {
		pc32s += offset;
	}
}

static inline void BLTZL(reg32 opcode) {
	if (rs64s < 0) {
		pc32s += offset;
	} else {
		pc32s += 4;
	}
}

static inline void BGEZL(reg32 opcode) {
	if (rs64s >= 0) {
		pc32s += offset;
	} else {
		pc32s += 4;
	}
}

static inline void TGEI(reg32 opcode) {}

static inline void TGEIU(reg32 opcode) {}

static inline void TLTI(reg32 opcode) {}

static inline void TLTIU(reg32 opcode) {}

static inline void TEQI(reg32 opcode) {}

static inline void TNEI(reg32 opcode) {}

static inline void BLTZAL(reg32 opcode) {
	ra64s = pc32s + 8;
	if (rs64s < 0) {
		pc32s += offset;
	}
}

static inline void BGEZAL(reg32 opcode) {
	ra64s = pc32s + 8;
	if (rs64s >= 0) {
		pc32s += offset;
	}
}

static inline void BLTZALL(reg32 opcode) {
	ra64s = pc32s + 8;
	if (rs64s < 0) {
		pc32s += offset;
	} else {
		pc32s += 4;
	}
}

static inline void BGEZALL(reg32 opcode) {
	ra64s = pc32s + 8;
	if (rs64s >= 0) {
		pc32s += offset;
	} else {
		pc32s += 4;
	}
}

static inline void MTSAB(reg32 opcode) {}

static inline void MTSAH(reg32 opcode) {}

static inline void MADD(reg32 opcode) {
	reg res;
	res._s64[0] = ((s64) rs32s * (s64) rt32s) + (s64) ((lo64u & 0xffffffffull) | (hi64u << 32));
	lo64s = res._s32[0];
	hi64s = res._s32[1];
	rd64s = res._s32[0];
}

static inline void MADDU(reg32 opcode) {
	reg res;
	res._u64[0] = ((u64) rs32u * (u64) rt32u) + ((lo64u & 0xffffffffull) | (hi64u << 32));
	lo64s = res._s32[0];
	hi64s = res._s32[1];
	rd64s = res._s32[0];
}

static inline void PLZCW(reg32 opcode) {}

static inline void MULT1(reg32 opcode) {}

static inline void MULTU1(reg32 opcode) {}

static inline void DIV1(reg32 opcode) {}

static inline void DIVU1(reg32 opcode) {}

static inline void MFC0(reg32 opcode) {
	rt32u = ps2.ee.cop0.raw[rd()]._u32[0];
}

static inline void MTC0(reg32 opcode) {
	ps2.ee.cop0.raw[rd()]._u32[0] = rt32u;
}

static inline void BC0F(reg32 opcode) {}

static inline void BC0T(reg32 opcode) {}

static inline void BC0FL(reg32 opcode) {}

static inline void BC0TL(reg32 opcode) {}

static inline void TLBR(reg32 opcode) {}

static inline void TLBWI(reg32 opcode) {}

static inline void TLBWR(reg32 opcode) {}

static inline void TLBP(reg32 opcode) {}

static inline void ERET(reg32 opcode) {}

static inline void EI(reg32 opcode) {}

static inline void DI(reg32 opcode) {}

static inline void MFC1(reg32 opcode) {}

static inline void DMFC1(reg32 opcode) {}

static inline void CFC1(reg32 opcode) {}

static inline void MTC1(reg32 opcode) {}

static inline void DMTC1(reg32 opcode) {}

static inline void CTC1(reg32 opcode) {}

static inline void BC1F(reg32 opcode) {}

static inline void BC1T(reg32 opcode) {}

static inline void BC1FL(reg32 opcode) {}

static inline void BC1TL(reg32 opcode) {}

static inline void MFC2(reg32 opcode) {}

static inline void CFC2(reg32 opcode) {}

static inline void MTC2(reg32 opcode) {}

static inline void CTC2(reg32 opcode) {}

static inline void BC2(reg32 opcode) {}

static inline void LWC1(reg32 opcode) {}

static inline void LWC2(reg32 opcode) {}

static inline void PREF(reg32 opcode) {}

static inline void LQC2(reg32 opcode) {}

static inline void LD(reg32 opcode) {
	rt64u = memread64(base32s + simm16);
}

static inline void SWC1(reg32 opcode) {}

static inline void SWC2(reg32 opcode) {}

static inline void SQC2(reg32 opcode) {}

static inline void SD(reg32 opcode) {
	memwrite64(base32s + simm16, rt64u);
}

static inline void J(reg32 opcode) {
	 branch((pc32u & 0xf0000000) | ((opcode._u32[0] << 2) & 0x0fffffff));
}

static inline void JAL(reg32 opcode) {
	ra64u = pc32u + 8;
	branch((pc32u & 0xf0000000) | ((opcode._u32[0] << 2) & 0x0fffffff));
}

static inline void BEQ(reg32 opcode) {
	if (rs64s == rt64s) {
		branch(pc32s + offset);
	}
}

static inline void BNE(reg32 opcode) {
	if (rs64s != rt64s) {
		branch(pc32s + offset);
	}
}

static inline void BLEZ(reg32 opcode) {
	if (rs64s <= 0) {
		branch(pc32s + offset);
	}
}

static inline void BGTZ(reg32 opcode) {
	if (rs64s > 0) {
		branch(pc32s + offset);
	}
}

static inline void ADDI(reg32 opcode) {
	s32 res;
	if (__builtin_add_overflow(rs32s, simm16, &res)) {
		exception();
		return;
	}
	rt64s = res;
}

static inline void ADDIU(reg32 opcode) {
	s32 res;
	if (!__builtin_add_overflow(rs32s, simm16, &res)) {
		rt64s = res;
	}
}

static inline void SLTI(reg32 opcode) {
	rd64s = rs64s < simm16 ? 1 : 0;
}

static inline void SLTIU(reg32 opcode) {
	rd64s = rs64s < (u64) (s64) simm16 ? 1 : 0;
}

static inline void ANDI(reg32 opcode) {
	rt64u = rs64u & uimm16;
}

static inline void ORI(reg32 opcode) {
	rt64u = rs64u | uimm16;
}

static inline void XORI(reg32 opcode) {
	rt64u = rs64u ^ uimm16;
}

static inline void LUI(reg32 opcode) {
	rt64s = ((s32) simm16) << 16;
}

static inline void BEQL(reg32 opcode) {
	if (rs64s == rt64s) {
		branch(pc32s + offset);
	} else {
		pc32s += 4;
	}
}

static inline void BNEL(reg32 opcode) {
	if (rs64s != rt64s) {
		branch(pc32s + offset);
	} else {
		pc32s += 4;
	}
}

static inline void BLEZL(reg32 opcode) {
	if (rs64s <= 0) {
		branch(pc32s + offset);
	} else {
		pc32s += 4;
	}
}

static inline void BGTZL(reg32 opcode) {
	if (rs64s > 0) {
		branch(pc32s + offset);
	} else {
		pc32s += 4;
	}
}

static inline void DADDI(reg32 opcode) {
	s64 res;
	if (__builtin_add_overflow(rs64s, simm16, &res)) {
		exception();
		return;
	}
	rt64s = res;
}

static inline void DADDIU(reg32 opcode) {
	s64 res;
	if (!__builtin_add_overflow(rs64s, simm16, &res)) {
		rt64s = res;
	}
}

static inline void LDL(reg32 opcode) {}

static inline void LDR(reg32 opcode) {}

static inline void LQ(reg32 opcode) {}

static inline void SQ(reg32 opcode) {}

static inline void LB(reg32 opcode) {
	rt64s = memread8(base32s + simm16);
}

static inline void LH(reg32 opcode) {
	rt64s = (s16) memread16(base32s + simm16);
}

static inline void LWL(reg32 opcode) {}

static inline void LW(reg32 opcode) {
	rt64s = (s32) memread32(base32s + simm16);
}

static inline void LBU(reg32 opcode) {
	rt64u = memread8(base32s + simm16);
}

static inline void LHU(reg32 opcode) {
	rt64u = memread16(base32s + simm16);
}

static inline void LWR(reg32 opcode) {}

static inline void LWU(reg32 opcode) {}

static inline void SB(reg32 opcode) {
	memwrite8(base32s + simm16, (u8) rt32u);
}

static inline void SH(reg32 opcode) {
	memwrite16(base32s + simm16, (u16) rt32u);
}

static inline void SWL(reg32 opcode) {}

static inline void SW(reg32 opcode) {
	memwrite32(base32s + simm16, (u32) rt32u);
}

static inline void SDL(reg32 opcode) {}

static inline void SDR(reg32 opcode) {}

static inline void SWR(reg32 opcode) {}

static inline void CACHE(reg32 opcode) {}

static inline void PADDW(reg32 opcode) {
	rdi(s32, 0) = rsi(s32, 0) + rti(s32, 0);
	rdi(s32, 1) = rsi(s32, 1) + rti(s32, 1);
	rdi(s32, 2) = rsi(s32, 2) + rti(s32, 2);
	rdi(s32, 3) = rsi(s32, 3) + rti(s32, 3);
}

static inline void PSUBW(reg32 opcode) {
	rdi(s32, 0) = rsi(s32, 0) + rti(s32, 0);
	rdi(s32, 1) = rsi(s32, 1) + rti(s32, 1);
	rdi(s32, 2) = rsi(s32, 2) + rti(s32, 2);
	rdi(s32, 3) = rsi(s32, 3) + rti(s32, 3);
}

static inline void PCGTW(reg32 opcode) {
	rdi(s32, 0) = rsi(s32, 0) > rti(s32, 0) ? -1 : 0;
	rdi(s32, 1) = rsi(s32, 1) > rti(s32, 1) ? -1 : 0;
	rdi(s32, 2) = rsi(s32, 2) > rti(s32, 2) ? -1 : 0;
	rdi(s32, 3) = rsi(s32, 3) > rti(s32, 3) ? -1 : 0;
}

static inline void PMAXW(reg32 opcode) {
	rdi(s32, 0) = rsi(s32, 0) > rti(s32, 0) ? rsi(s32, 0) : rti(s32, 0);
	rdi(s32, 1) = rsi(s32, 1) > rti(s32, 1) ? rsi(s32, 1) : rti(s32, 1);
	rdi(s32, 2) = rsi(s32, 2) > rti(s32, 2) ? rsi(s32, 2) : rti(s32, 2);
	rdi(s32, 3) = rsi(s32, 3) > rti(s32, 3) ? rsi(s32, 3) : rti(s32, 3);
}

static inline void PADDH(reg32 opcode) {
	rdi(s16, 0) = rsi(s16, 0) + rti(s16, 0);
	rdi(s16, 1) = rsi(s16, 1) + rti(s16, 1);
	rdi(s16, 2) = rsi(s16, 2) + rti(s16, 2);
	rdi(s16, 3) = rsi(s16, 3) + rti(s16, 3);
	rdi(s16, 4) = rsi(s16, 4) + rti(s16, 4);
	rdi(s16, 5) = rsi(s16, 5) + rti(s16, 5);
	rdi(s16, 6) = rsi(s16, 6) + rti(s16, 6);
	rdi(s16, 7) = rsi(s16, 7) + rti(s16, 7);
}

static inline void PSUBH(reg32 opcode) {

	rdi(s16, 0) = rsi(s16, 0) - rti(s16, 0);
	rdi(s16, 1) = rsi(s16, 1) - rti(s16, 1);
	rdi(s16, 2) = rsi(s16, 2) - rti(s16, 2);
	rdi(s16, 3) = rsi(s16, 3) - rti(s16, 3);
	rdi(s16, 4) = rsi(s16, 4) - rti(s16, 4);
	rdi(s16, 5) = rsi(s16, 5) - rti(s16, 5);
	rdi(s16, 6) = rsi(s16, 6) - rti(s16, 6);
	rdi(s16, 7) = rsi(s16, 7) - rti(s16, 7);
}

static inline void PCGTH(reg32 opcode) {

	rdi(s16, 0) = rsi(s16, 0) > rti(s16, 0) ? -1 : 0;
	rdi(s16, 1) = rsi(s16, 1) > rti(s16, 1) ? -1 : 0;
	rdi(s16, 2) = rsi(s16, 2) > rti(s16, 2) ? -1 : 0;
	rdi(s16, 3) = rsi(s16, 3) > rti(s16, 3) ? -1 : 0;
	rdi(s16, 4) = rsi(s16, 4) > rti(s16, 4) ? -1 : 0;
	rdi(s16, 5) = rsi(s16, 5) > rti(s16, 5) ? -1 : 0;
	rdi(s16, 6) = rsi(s16, 6) > rti(s16, 6) ? -1 : 0;
	rdi(s16, 7) = rsi(s16, 7) > rti(s16, 7) ? -1 : 0;
}

static inline void PMAXH(reg32 opcode) {

	rdi(s16, 0) = rsi(s16, 0) > rti(s16, 0) ? rsi(s16, 0) : rti(s16, 0);
	rdi(s16, 1) = rsi(s16, 1) > rti(s16, 1) ? rsi(s16, 1) : rti(s16, 1);
	rdi(s16, 2) = rsi(s16, 2) > rti(s16, 2) ? rsi(s16, 2) : rti(s16, 2);
	rdi(s16, 3) = rsi(s16, 3) > rti(s16, 3) ? rsi(s16, 3) : rti(s16, 3);
	rdi(s16, 4) = rsi(s16, 4) > rti(s16, 4) ? rsi(s16, 0) : rti(s16, 4);
	rdi(s16, 5) = rsi(s16, 5) > rti(s16, 5) ? rsi(s16, 1) : rti(s16, 5);
	rdi(s16, 6) = rsi(s16, 6) > rti(s16, 6) ? rsi(s16, 2) : rti(s16, 6);
	rdi(s16, 7) = rsi(s16, 7) > rti(s16, 7) ? rsi(s16, 3) : rti(s16, 7);
}

static inline void PADDB(reg32 opcode) {

	rdi(s8, 0) = rsi(s8, 0) + rti(s8, 0);
	rdi(s8, 1) = rsi(s8, 1) + rti(s8, 1);
	rdi(s8, 2) = rsi(s8, 2) + rti(s8, 2);
	rdi(s8, 3) = rsi(s8, 3) + rti(s8, 3);
	rdi(s8, 4) = rsi(s8, 4) + rti(s8, 4);
	rdi(s8, 5) = rsi(s8, 5) + rti(s8, 5);
	rdi(s8, 6) = rsi(s8, 6) + rti(s8, 6);
	rdi(s8, 7) = rsi(s8, 7) + rti(s8, 7);
	rdi(s8, 8) = rsi(s8, 8) + rti(s8, 8);
	rdi(s8, 9) = rsi(s8, 9) + rti(s8, 9);
	rdi(s8, 10) = rsi(s8, 10) + rti(s8, 10);
	rdi(s8, 11) = rsi(s8, 11) + rti(s8, 11);
	rdi(s8, 12) = rsi(s8, 12) + rti(s8, 12);
	rdi(s8, 13) = rsi(s8, 13) + rti(s8, 13);
	rdi(s8, 14) = rsi(s8, 14) + rti(s8, 14);
	rdi(s8, 15) = rsi(s8, 15) + rti(s8, 15);
}

static inline void PSUBB(reg32 opcode) {

	rdi(s8, 0) = rsi(s8, 0) - rti(s8, 0);
	rdi(s8, 1) = rsi(s8, 1) - rti(s8, 1);
	rdi(s8, 2) = rsi(s8, 2) - rti(s8, 2);
	rdi(s8, 3) = rsi(s8, 3) - rti(s8, 3);
	rdi(s8, 4) = rsi(s8, 4) - rti(s8, 4);
	rdi(s8, 5) = rsi(s8, 5) - rti(s8, 5);
	rdi(s8, 6) = rsi(s8, 6) - rti(s8, 6);
	rdi(s8, 7) = rsi(s8, 7) - rti(s8, 7);
	rdi(s8, 8) = rsi(s8, 8) - rti(s8, 8);
	rdi(s8, 9) = rsi(s8, 9) - rti(s8, 9);
	rdi(s8, 10) = rsi(s8, 10) - rti(s8, 10);
	rdi(s8, 11) = rsi(s8, 11) - rti(s8, 11);
	rdi(s8, 12) = rsi(s8, 12) - rti(s8, 12);
	rdi(s8, 13) = rsi(s8, 13) - rti(s8, 13);
	rdi(s8, 14) = rsi(s8, 14) - rti(s8, 14);
	rdi(s8, 15) = rsi(s8, 15) - rti(s8, 15);
}

static inline void PCGTB(reg32 opcode) {

	rdi(s8, 0) = rsi(s8, 0) > rti(s8, 0) ? -1 : 0;
	rdi(s8, 1) = rsi(s8, 1) > rti(s8, 1) ? -1 : 0;
	rdi(s8, 2) = rsi(s8, 2) > rti(s8, 2) ? -1 : 0;
	rdi(s8, 3) = rsi(s8, 3) > rti(s8, 3) ? -1 : 0;
	rdi(s8, 4) = rsi(s8, 4) > rti(s8, 4) ? -1 : 0;
	rdi(s8, 5) = rsi(s8, 5) > rti(s8, 5) ? -1 : 0;
	rdi(s8, 6) = rsi(s8, 6) > rti(s8, 6) ? -1 : 0;
	rdi(s8, 7) = rsi(s8, 7) > rti(s8, 7) ? -1 : 0;
	rdi(s8, 8) = rsi(s8, 8) > rti(s8, 8) ? -1 : 0;
	rdi(s8, 9) = rsi(s8, 9) > rti(s8, 9) ? -1 : 0;
	rdi(s8, 10) = rsi(s8, 10) > rti(s8, 10) ? -1 : 0;
	rdi(s8, 11) = rsi(s8, 11) > rti(s8, 11) ? -1 : 0;
	rdi(s8, 12) = rsi(s8, 12) > rti(s8, 12) ? -1 : 0;
	rdi(s8, 13) = rsi(s8, 13) > rti(s8, 13) ? -1 : 0;
	rdi(s8, 14) = rsi(s8, 14) > rti(s8, 14) ? -1 : 0;
	rdi(s8, 15) = rsi(s8, 15) > rti(s8, 15) ? -1 : 0;
}

static inline void PADDSW(reg32 opcode) {

	rdi(s32, 0) = satadd(rsi(s32, 0), rti(s32, 0));
	rdi(s32, 1) = satadd(rsi(s32, 1), rti(s32, 1));
	rdi(s32, 2) = satadd(rsi(s32, 2), rti(s32, 2));
	rdi(s32, 3) = satadd(rsi(s32, 3), rti(s32, 3));
}

static inline void PSUBSW(reg32 opcode) {

	rdi(s32, 0) = satsub(rsi(s32, 0), rti(s32, 0));
	rdi(s32, 1) = satsub(rsi(s32, 1), rti(s32, 1));
	rdi(s32, 2) = satsub(rsi(s32, 2), rti(s32, 2));
	rdi(s32, 3) = satsub(rsi(s32, 3), rti(s32, 3));
}

static inline void PEXTLW(reg32 opcode) {
	rdi(u32, 0) = rti(u32, 0);
	rdi(u32, 1) = rsi(u32, 0);
	rdi(u32, 2) = rti(u32, 1);
	rdi(u32, 3) = rsi(u32, 1);
}

static inline void PPACW(reg32 opcode) {
	rdi(s32, 0) = rti(s32, 0);
	rdi(s32, 1) = rti(s32, 2);
	rdi(s32, 2) = rsi(s32, 1);
	rdi(s32, 3) = rsi(s32, 3);
}

static inline void PADDSH(reg32 opcode) {
	rdi(s16, 0) = satadd(rsi(s16, 0), rti(s16, 0));
	rdi(s16, 1) = satadd(rsi(s16, 1), rti(s16, 1));
	rdi(s16, 2) = satadd(rsi(s16, 2), rti(s16, 2));
	rdi(s16, 3) = satadd(rsi(s16, 3), rti(s16, 3));
	rdi(s16, 4) = satadd(rsi(s16, 4), rti(s16, 4));
	rdi(s16, 5) = satadd(rsi(s16, 5), rti(s16, 5));
	rdi(s16, 6) = satadd(rsi(s16, 6), rti(s16, 6));
	rdi(s16, 7) = satadd(rsi(s16, 7), rti(s16, 7));
}

static inline void PSUBSH(reg32 opcode) {
	rdi(s16, 0) = satsub(rsi(s16, 0), rti(s16, 0));
	rdi(s16, 1) = satsub(rsi(s16, 1), rti(s16, 1));
	rdi(s16, 2) = satsub(rsi(s16, 2), rti(s16, 2));
	rdi(s16, 3) = satsub(rsi(s16, 3), rti(s16, 3));
	rdi(s16, 4) = satsub(rsi(s16, 4), rti(s16, 4));
	rdi(s16, 5) = satsub(rsi(s16, 5), rti(s16, 5));
	rdi(s16, 6) = satsub(rsi(s16, 6), rti(s16, 6));
	rdi(s16, 7) = satsub(rsi(s16, 7), rti(s16, 7));
}

static inline void PEXTLH(reg32 opcode) {}

static inline void PPACH(reg32 opcode) {}

static inline void PADDSB(reg32 opcode) {

	rdi(s8, 0) = satadd(rsi(s8, 0), rti(s8, 0));
	rdi(s8, 1) = satadd(rsi(s8, 1), rti(s8, 1));
	rdi(s8, 2) = satadd(rsi(s8, 2), rti(s8, 2));
	rdi(s8, 3) = satadd(rsi(s8, 3), rti(s8, 3));
	rdi(s8, 4) = satadd(rsi(s8, 4), rti(s8, 4));
	rdi(s8, 5) = satadd(rsi(s8, 5), rti(s8, 5));
	rdi(s8, 6) = satadd(rsi(s8, 6), rti(s8, 6));
	rdi(s8, 7) = satadd(rsi(s8, 7), rti(s8, 7));
	rdi(s8, 8) = satadd(rsi(s8, 9), rti(s8, 8));
	rdi(s8, 9) = satadd(rsi(s8, 0), rti(s8, 9));
	rdi(s8, 10) = satadd(rsi(s8, 10), rti(s8, 10));
	rdi(s8, 11) = satadd(rsi(s8, 11), rti(s8, 11));
	rdi(s8, 12) = satadd(rsi(s8, 12), rti(s8, 12));
	rdi(s8, 13) = satadd(rsi(s8, 13), rti(s8, 13));
	rdi(s8, 14) = satadd(rsi(s8, 14), rti(s8, 14));
	rdi(s8, 15) = satadd(rsi(s8, 15), rti(s8, 15));
}

static inline void PSUBSB(reg32 opcode) {

	rdi(s8, 0) = satsub(rsi(s8, 0), rti(s8, 0));
	rdi(s8, 1) = satsub(rsi(s8, 1), rti(s8, 1));
	rdi(s8, 2) = satsub(rsi(s8, 2), rti(s8, 2));
	rdi(s8, 3) = satsub(rsi(s8, 3), rti(s8, 3));
	rdi(s8, 4) = satsub(rsi(s8, 4), rti(s8, 4));
	rdi(s8, 5) = satsub(rsi(s8, 5), rti(s8, 5));
	rdi(s8, 6) = satsub(rsi(s8, 6), rti(s8, 6));
	rdi(s8, 7) = satsub(rsi(s8, 7), rti(s8, 7));
	rdi(s8, 8) = satsub(rsi(s8, 9), rti(s8, 8));
	rdi(s8, 9) = satsub(rsi(s8, 0), rti(s8, 9));
	rdi(s8, 10) = satsub(rsi(s8, 10), rti(s8, 10));
	rdi(s8, 11) = satsub(rsi(s8, 11), rti(s8, 11));
	rdi(s8, 12) = satsub(rsi(s8, 12), rti(s8, 12));
	rdi(s8, 13) = satsub(rsi(s8, 13), rti(s8, 13));
	rdi(s8, 14) = satsub(rsi(s8, 14), rti(s8, 14));
	rdi(s8, 15) = satsub(rsi(s8, 15), rti(s8, 15));
}

static inline void PEXTLB(reg32 opcode) {
	rdi(u8, 0) = rti(u8, 0);
	rdi(u8, 1) = rsi(u8, 0);
	rdi(u8, 2) = rti(u8, 1);
	rdi(u8, 3) = rsi(u8, 1);
	rdi(u8, 4) = rti(u8, 2);
	rdi(u8, 5) = rsi(u8, 2);
	rdi(u8, 6) = rti(u8, 3);
	rdi(u8, 7) = rsi(u8, 3);
	rdi(u8, 8) = rti(u8, 4);
	rdi(u8, 9) = rsi(u8, 4);
	rdi(u8, 10) = rti(u8, 5);
	rdi(u8, 11) = rsi(u8, 5);
	rdi(u8, 12) = rti(u8, 6);
	rdi(u8, 13) = rsi(u8, 6);
	rdi(u8, 14) = rti(u8, 7);
	rdi(u8, 15) = rsi(u8, 7);
}

static inline void PPACB(reg32 opcode) {
	rdi(u8, 0) = rti(u8, 0);
	rdi(u8, 1) = rti(u8, 2);
	rdi(u8, 2) = rti(u8, 4);
	rdi(u8, 3) = rti(u8, 6);
	rdi(u8, 4) = rti(u8, 8);
	rdi(u8, 5) = rti(u8, 10);
	rdi(u8, 6) = rti(u8, 12);
	rdi(u8, 7) = rti(u8, 14);
	rdi(u8, 8) = rsi(u8, 0);
	rdi(u8, 9) = rsi(u8, 2);
	rdi(u8, 10) = rsi(u8, 4);
	rdi(u8, 11) = rsi(u8, 6);
	rdi(u8, 12) = rsi(u8, 8);
	rdi(u8, 13) = rsi(u8, 10);
	rdi(u8, 14) = rsi(u8, 12);
	rdi(u8, 15) = rsi(u8, 14);
}

static inline void PEXT5(reg32 opcode) {
	rdi(u8, 0) = (rti(u32, 0) & 0x1f) << 3;
	rdi(u8, 1) = ((rti(u32, 0) >> 5) & 0x1f) << 3;
	rdi(u8, 2) = ((rti(u32, 0) >> 10) & 0x1f) << 3;
	rdi(u8, 3) = (rti(u32, 0) & 0x8000) ? 0x80 : 0;

	rdi(u8, 4) = (u8) (rti(u32, 1) & 0x1f) << 3;
	rdi(u8, 5) = (u8) ((rti(u32, 1) >> 5) & 0x1f) << 3;
	rdi(u8, 6) = (u8) ((rti(u32, 1) >> 10) & 0x1f) << 3;
	rdi(u8, 7) = (u8) (rti(u32, 1) & 0x8000) ? 0x80 : 0;
	rdi(u8, 8) = (u8) (rti(u32, 2) & 0x1f) << 3;
	rdi(u8, 9) = (u8) ((rti(u32, 2) >> 5) & 0x1f) << 3;
	rdi(u8, 10) = (u8) ((rti(u32, 2) >> 10) & 0x1f) << 3;
	rdi(u8, 11) = (u8) (rti(u32, 2) & 0x8000) ? 0x80 : 0;
	rdi(u8, 12) = (u8) (rti(u32, 3) & 0x1f) << 3;
	rdi(u8, 13) = (u8) ((rti(u32, 3) >> 5) & 0x1f) << 3;
	rdi(u8, 14) = (u8) ((rti(u32, 3) >> 10) & 0x1f) << 3;
	rdi(u8, 15) = (u8) (rti(u32, 3) & 0x8000) ? 0x80 : 0;


}

static inline void PPAC5(reg32 opcode) {}

static inline void PABSW(reg32 opcode) {}

static inline void PCEQW(reg32 opcode) {}

static inline void PMINW(reg32 opcode) {}

static inline void PADSBH(reg32 opcode) {}

static inline void PABSH(reg32 opcode) {}

static inline void PCEQH(reg32 opcode) {}

static inline void PMINH(reg32 opcode) {}

static inline void PCEQB(reg32 opcode) {}

static inline void PADDUW(reg32 opcode) {}

static inline void PSUBUW(reg32 opcode) {}

static inline void PEXTUW(reg32 opcode) {}

static inline void PADDUH(reg32 opcode) {}

static inline void PSUBUH(reg32 opcode) {}

static inline void PEXTUH(reg32 opcode) {}

static inline void PADDUB(reg32 opcode) {}

static inline void PSUBUB(reg32 opcode) {}

static inline void PEXTUB(reg32 opcode) {}

static inline void QFSRV(reg32 opcode) {}

static inline void PMADDW(reg32 opcode) {}

static inline void PSLLVW(reg32 opcode) {}

static inline void PSRLVW(reg32 opcode) {}

static inline void PMSUBW(reg32 opcode) {}

static inline void PMFHI(reg32 opcode) {}

static inline void PMFLO(reg32 opcode) {}

static inline void PINTH(reg32 opcode) {}

static inline void PMULTW(reg32 opcode) {}

static inline void PDIVW(reg32 opcode) {}

static inline void PCPYLD(reg32 opcode) {}

static inline void PMADDH(reg32 opcode) {}

static inline void PHMADH(reg32 opcode) {}

static inline void PAND(reg32 opcode) {
	rdi(u128, 0) = rsi(u128, 0) & rti(u128, 0);
}

static inline void PXOR(reg32 opcode) {
	rdi(u128, 0) = rsi(u128, 0) ^ rti(u128, 0);
}

static inline void PMSUBH(reg32 opcode) {}

static inline void PHMSBH(reg32 opcode) {}

static inline void PEXEH(reg32 opcode) {}

static inline void PREVH(reg32 opcode) {}

static inline void PMULTH(reg32 opcode) {}

static inline void PDIVBW(reg32 opcode) {}

static inline void PEXEW(reg32 opcode) {}

static inline void PROT3W(reg32 opcode) {}

static inline void PMADDUW(reg32 opcode) {}

static inline void PSRAVW(reg32 opcode) {}

static inline void PMTHI(reg32 opcode) {}

static inline void PMTLO(reg32 opcode) {}

static inline void PINTEH(reg32 opcode) {}

static inline void PMULTUW(reg32 opcode) {}

static inline void PDIVUW(reg32 opcode) {}

static inline void PCPYUD(reg32 opcode) {}

static inline void POR(reg32 opcode) {}

static inline void PNOR(reg32 opcode) {}

static inline void PEXCH(reg32 opcode) {}

static inline void PCPYH(reg32 opcode) {}

static inline void PEXCW(reg32 opcode) {}

static inline void PMFHL(reg32 opcode) {}

static inline void PMTHL(reg32 opcode) {}

static inline void PSLLH(reg32 opcode) {}

static inline void PSRLH(reg32 opcode) {}

static inline void PSRAH(reg32 opcode) {}

static inline void PSLLW(reg32 opcode) {}

static inline void PSRLW(reg32 opcode) {}

static inline void PSRAW(reg32 opcode) {}

static inline void SRLV(reg32 opcode) {}

static inline void SRAV(reg32 opcode) {}

static inline void MFHI1(reg32 opcode) {}

static inline void MTHI1(reg32 opcode) {}

static inline void MFLO1(reg32 opcode) {}

static inline void MTLO1(reg32 opcode) {}

static inline void MADD1(reg32 opcode) {}

static inline void MADDU1(reg32 opcode) {}

static inline void ADD_S(reg32 opcode) {}

static inline void SUB_S(reg32 opcode) {}

static inline void MUL_S(reg32 opcode) {}

static inline void DIV_S(reg32 opcode) {}

static inline void SQRT_S(reg32 opcode) {}

static inline void ABS_S(reg32 opcode) {}

static inline void MOV_S(reg32 opcode) {}

static inline void NEG_S(reg32 opcode) {}

static inline void SUBA_S(reg32 opcode) {}

static inline void MULA_S(reg32 opcode) {}

static inline void MADD_S(reg32 opcode) {}

static inline void MSUB_S(reg32 opcode) {}

static inline void MADDA_S(reg32 opcode) {}

static inline void MSUBA_S(reg32 opcode) {}

static inline void CVT_W_S(reg32 opcode) {}

static inline void MAX_S(reg32 opcode) {}

static inline void MIN_S(reg32 opcode) {}

static inline void C_F_S(reg32 opcode) {}

static inline void C_EQ_S(reg32 opcode) {}

static inline void C_LT_S(reg32 opcode) {}

static inline void C_LE_S(reg32 opcode) {}

static inline void RSQRT_S(reg32 opcode) {}

static inline void CVT_S(reg32 opcode) {}

static inline void CVT_S_W(reg32 opcode) {}

static inline void ADDA_S(reg32 opcode) {}

//vu macro mode
static inline void BC2F(reg32 opcode){}
static inline void BC2FL(reg32 opcode){}
static inline void BC2T(reg32 opcode){}
static inline void BC2TL(reg32 opcode){}
static inline void QMFC2(reg32 opcode){}
static inline void QMTC2(reg32 opcode){}
static inline void VABS(reg32 opcode){}
static inline void VADD(reg32 opcode){}
static inline void VADDi(reg32 opcode){}
static inline void VADDq(reg32 opcode){}
static inline void VADDbc(reg32 opcode){}
static inline void VADDA(reg32 opcode){}
static inline void VADDAi(reg32 opcode){}
static inline void VADDAq(reg32 opcode){}
static inline void VADDAbc(reg32 opcode){}
static inline void VCALLMS(reg32 opcode){}
static inline void VCALLMSR(reg32 opcode){}
static inline void VCLIP(reg32 opcode){}
static inline void VDIV(reg32 opcode){}
static inline void VFTOI0(reg32 opcode){}
static inline void VFTOI4(reg32 opcode){}
static inline void VFTOI12(reg32 opcode){}
static inline void VFTOI15(reg32 opcode){}
static inline void VIADD(reg32 opcode){}
static inline void VIADDI(reg32 opcode){}
static inline void VIAND(reg32 opcode){}
static inline void VILWR(reg32 opcode){}
static inline void VIOR(reg32 opcode){}
static inline void VISUB(reg32 opcode){}
static inline void VISWR(reg32 opcode){}
static inline void VITOF0(reg32 opcode){}
static inline void VITOF4(reg32 opcode){}
static inline void VITOF12(reg32 opcode){}
static inline void VITOF15(reg32 opcode){}

static inline void VLQD(reg32 opcode){}
static inline void VLQI(reg32 opcode){}
static inline void VMADD(reg32 opcode){}
static inline void VMADDi(reg32 opcode){}
static inline void VMADDq(reg32 opcode){}
static inline void VMADDbc(reg32 opcode){}
static inline void VMADDA(reg32 opcode){}
static inline void VMADDAi(reg32 opcode){}
static inline void VMADDAq(reg32 opcode){}
static inline void VMADDAbc(reg32 opcode){}
static inline void VMAX(reg32 opcode){}
static inline void VMAXi(reg32 opcode){}
static inline void VMAXbc(reg32 opcode){}

static inline void VMFIR(reg32 opcode){}

static inline void VMINI(reg32 opcode){}
static inline void VMINIi(reg32 opcode){}
static inline void VMINIbc(reg32 opcode){}

static inline void VMOVE(reg32 opcode){}
static inline void VMR32(reg32 opcode){}
static inline void VMSUB(reg32 opcode){}
static inline void VMSUBi(reg32 opcode){}
static inline void VMSUBq(reg32 opcode){}
static inline void VMSUBbc(reg32 opcode){}
static inline void VMSUBA(reg32 opcode){}
static inline void VMSUBAi(reg32 opcode){}
static inline void VMSUBAq(reg32 opcode){}
static inline void VMSUBAbc(reg32 opcode){}

static inline void VMTIR(reg32 opcode){}

static inline void VMUL(reg32 opcode){}
static inline void VMULi(reg32 opcode){}
static inline void VMULq(reg32 opcode){}
static inline void VMULbc(reg32 opcode){}
static inline void VMULA(reg32 opcode){}
static inline void VMULAi(reg32 opcode){}
static inline void VMULAq(reg32 opcode){}
static inline void VMULAbc(reg32 opcode){}
static inline void VNOP(reg32 opcode){}
static inline void VOPMULA(reg32 opcode){}
static inline void VOPMSUB(reg32 opcode){}
static inline void VRGET(reg32 opcode){}
static inline void VRINIT(reg32 opcode){}
static inline void VRNEXT(reg32 opcode){}
static inline void VRSQRT(reg32 opcode){}
static inline void VRXOR(reg32 opcode){}
static inline void VSQD(reg32 opcode){}
static inline void VSQI(reg32 opcode){}
static inline void VSQRT(reg32 opcode){}
static inline void VSUB(reg32 opcode){}
static inline void VSUBi(reg32 opcode){}
static inline void VSUBq(reg32 opcode){}
static inline void VSUBbc(reg32 opcode){}
static inline void VSUBA(reg32 opcode){}
static inline void VSUBAi(reg32 opcode){}
static inline void VSUBAq(reg32 opcode){}
static inline void VSUBAbc(reg32 opcode){}
static inline void VWAITQ(reg32 opcode){}



static inline void invalid() { /* Handle invalid opcode */ }

#define ex(func) case (__COUNTER__-baseval): func(opcode); break
#define null() case (__COUNTER__-baseval): invalid(); break


void interpret(u32 instr) {
	reg32 opcode;
	opcode._u32[0] = instr;

	static const int baseval = __COUNTER__ + 1;

	switch (opcode._u32[0] >> 26) {
		ex(SPECIAL);
		ex(REGIMM);
		ex(J);
		ex(JAL);
		ex(BEQ);
		ex(BNE);
		ex(BLEZ);
		ex(BGTZ);
		ex(ADDI);
		ex(ADDIU);
		ex(SLTI);
		ex(SLTIU);
		ex(ANDI);
		ex(ORI);
		ex(XORI);
		ex(LUI);
		ex(COP0);
		ex(COP1);
		ex(COP2);
		null();
		ex(BEQL);
		ex(BNEL);
		ex(BLEZL);
		ex(BGTZL);
		ex(DADDI);
		ex(DADDIU);
		ex(LDL);
		ex(LDR);
		ex(MMI);
		null();
		ex(LQ);
		ex(SQ);
		ex(LB);
		ex(LH);
		ex(LWL);
		ex(LW);
		ex(LBU);
		ex(LHU);
		ex(LWR);
		ex(LWU);
		ex(SB);
		ex(SH);
		ex(SWL);
		ex(SW);
		ex(SDL);
		ex(SDR);
		ex(SWR);
		ex(CACHE);
		null();
		ex(LWC1);
		null();
		ex(PREF);
		null();
		null();
		ex(LQC2);
		ex(LD);
		null();
		ex(SWC1);
		null();
		null();
		null();
		null();
		ex(SQC2);
		ex(SD);
	}
}

static inline void SPECIAL(reg32 opcode) {
	static const int baseval = __COUNTER__ + 1;

	switch (opcode._u32[0] & 0x3f) {
		ex(SLL);
		null();
		ex(SRL);
		ex(SRA);
		ex(SLLV);
		null();
		ex(SRLV);
		ex(SRAV);
		ex(JR);
		ex(JALR);
		ex(MOVZ);
		ex(MOVN);
		ex(SYSCALL);
		ex(BREAK);
		null();
		ex(SYNC);
		ex(MFHI);
		ex(MTHI);
		ex(MFLO);
		ex(MTLO);
		ex(DSLLV);
		null();
		ex(DSRLV);
		ex(DSRAV);
		ex(MULT);
		ex(MULTU);
		ex(DIV);
		ex(DIVU);
		null();
		null();
		null();
		null();
		ex(ADD);
		ex(ADDU);
		ex(SUB);
		ex(SUBU);
		ex(AND);
		ex(OR);
		ex(XOR);
		ex(NOR);
		ex(MFSA);
		ex(MTSA);
		ex(SLT);
		ex(SLTU);
		ex(DADD);
		ex(DADDU);
		ex(DSUB);
		ex(DSUBU);
		ex(TGE);
		ex(TGEU);
		ex(TLT);
		ex(TLTU);
		ex(TEQ);
		null();
		ex(TNE);
		null();
		ex(DSLL);
		null();
		ex(DSRL);
		ex(DSRA);
		ex(DSLL32);
		null();
		ex(DSRL32);
		ex(DSRA32);
	}
}

static inline void REGIMM(reg32 opcode) {
	static const int baseval = __COUNTER__ + 1;

	switch ((opcode._u32[0] >> 16) & 0x1f) {
		ex(BLTZ);
		ex(BGEZ);
		ex(BLTZL);
		ex(BGEZL);
		null();
		null();
		null();
		null();
		ex(TGEI);
		ex(TGEIU);
		ex(TLTI);
		ex(TLTIU);
		ex(TEQI);
		null();
		ex(TNEI);
		null();
		ex(BLTZAL);
		ex(BGEZAL);
		ex(BLTZALL);
		ex(BGEZALL);
		null();
		null();
		null();
		null();
		ex(MTSAB);
		ex(MTSAH);
		null();
		null();
		null();
		null();
		null();
		null();
	}
}

static inline void MMI(reg32 opcode) {
	static const int baseval = __COUNTER__ + 1;

	switch (opcode._u32[0] & 0x3f) {
		ex(MADD);
		ex(MADDU);
		null();
		null();
		ex(PLZCW);
		null();
		null();
		null();
		ex(MMI0);
		ex(MMI2);
		null();
		null();
		null();
		null();
		null();
		null();
		ex(MFHI1);
		ex(MTHI1);
		ex(MFLO1);
		ex(MTLO1);
		null();
		null();
		null();
		null();
		ex(MULT1);
		ex(MULTU1);
		ex(DIV1);
		ex(DIVU1);
		null();
		null();
		null();
		null();
		ex(MADD1);
		ex(MADDU1);
		null();
		null();
		null();
		null();
		null();
		null();
		ex(MMI1);
		ex(MMI3);
		null();
		null();
		null();
		null();
		null();
		null();
		ex(PMFHL);
		ex(PMTHL);
		null();
		null();
		ex(PSLLH);
		null();
		ex(PSRLH);
		ex(PSRAH);
		null();
		null();
		null();
		null();
		ex(PSLLW);
		null();
		ex(PSRLW);
		ex(PSRAW);
	}
}


static inline void MMI0(reg32 opcode) {
	static const int baseval = __COUNTER__ + 1;

	switch ((opcode._u32[0] >> 6) & 0x1f) {
		ex(PADDW);
		ex(PSUBW);
		ex(PCGTW);
		ex(PMAXW);
		ex(PADDH);
		ex(PSUBH);
		ex(PCGTH);
		ex(PMAXH);
		ex(PADDB);
		ex(PSUBB);
		ex(PCGTB);
		null();
		null();
		null();
		null();
		null();
		ex(PADDSW);
		ex(PSUBSW);
		ex(PEXTLW);
		ex(PPACW);
		ex(PADDSH);
		ex(PSUBSH);
		ex(PEXTLH);
		ex(PPACH);
		ex(PADDSB);
		ex(PSUBSB);
		ex(PEXTLB);
		ex(PPACB);
		null();
		null();
		ex(PEXT5);
		ex(PPAC5);
	}
}

static inline void MMI1(reg32 opcode) {
	static const int baseval = __COUNTER__ + 1;

	switch ((opcode._u32[0] >> 6) & 0x1f) {
		null();
		ex(PABSW);
		ex(PCEQW);
		ex(PMINW);
		ex(PADSBH);
		ex(PABSH);
		ex(PCEQH);
		ex(PMINH);
		null();
		null();
		ex(PCEQB);
		null();
		null();
		null();
		null();
		null();
		ex(PADDUW);
		ex(PSUBUW);
		ex(PEXTUW);
		null();
		ex(PADDUH);
		ex(PSUBUH);
		ex(PEXTUH);
		null();
		ex(PADDUB);
		ex(PSUBUB);
		ex(PEXTUB);
		ex(QFSRV);
		null();
		null();
		null();
		null();
	}
}


static inline void MMI2(reg32 opcode) {
	static const int baseval = __COUNTER__ + 1;

	switch ((opcode._u32[0] >> 6) & 0x1f) {
		ex(PMADDW);
		null();
		ex(PSLLVW);
		ex(PSRLVW);
		ex(PMSUBW);
		null();
		null();
		null();
		ex(PMFHI);
		ex(PMFLO);
		ex(PINTH);
		null();
		ex(PMULTW);
		ex(PDIVW);
		ex(PCPYLD);
		null();
		ex(PMADDH);
		ex(PHMADH);
		ex(PAND);
		ex(PXOR);
		ex(PMSUBH);
		ex(PHMSBH);
		null();
		null();
		null();
		null();
		ex(PEXEH);
		ex(PREVH);
		ex(PMULTH);
		ex(PDIVBW);
		ex(PEXEW);
		ex(PROT3W);
	}
}


static inline void MMI3(reg32 opcode) {
	static const int baseval = __COUNTER__ + 1;

	switch ((opcode._u32[0] >> 6) & 0x1f) {
		ex(PMADDUW);
		null();
		null();
		ex(PSRAVW);
		null();
		null();
		null();
		null();
		ex(PMTHI);
		ex(PMTLO);
		ex(PINTEH);
		null();
		ex(PMULTUW);
		ex(PDIVUW);
		ex(PCPYUD);
		null();
		null();
		null();
		ex(POR);
		ex(PNOR);
		null();
		null();
		null();
		null();
		null();
		null();
		ex(PEXCH);
		ex(PCPYH);
		null();
		null();
		ex(PEXCW);
		null();
	}
}

static inline void BC0(reg32 opcode) {
	static const int baseval = __COUNTER__ + 1;
	switch ((opcode._u32[0] >> 16) & 0x1f) {
		ex(BC0F);
		ex(BC0T);
		ex(BC0FL);
		ex(BC0TL);
	}
}

static inline void COP0(reg32 opcode) {
	static const int baseval = __COUNTER__ + 1;

	switch ((opcode._u32[0] >> 21) & 0x1f) {
		ex(MFC0);
		null();
		null();
		null();
		ex(MTC0);
		null();
		null();
		null();
		ex(BC0);
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		ex(TLB);
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
	}
}

static inline void TLB(reg32 opcode) {
	static const int baseval = __COUNTER__ + 1;

	switch (opcode._u32[0] & 0x3f) {
		ex(TLBR);
		ex(TLBWI);
		null();
		null();
		null();
		null();
		ex(TLBWR);
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		ex(ERET);
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		ex(EI);
		ex(DI);
	}
}

static inline void COP1(reg32 opcode) {
	static const int baseval = __COUNTER__ + 1;

	switch ((opcode._u32[0] >> 21) & 0x1f) {
		ex(MFC1);
		null();
		ex(CFC1);
		null();
		ex(MTC1);
		null();
		ex(CTC1);
		null();
		null();
		null();
		null();
		ex(CVT_S);
		null();
		null();
		null();
		null();
	}
}

static inline void BC1(reg32 opcode) {
	static const int baseval = __COUNTER__ + 1;

	switch ((opcode._u32[0] >> 16) & 0x1f) {
		ex(BC1F);
		ex(BC1T);
		ex(BC1FL);
		ex(BC1TL);
		null();
		null();
		null();
		null();
	}
}

static inline void FPU_S(reg32 opcode) {
	static const int baseval = __COUNTER__ + 1;

	switch (opcode._u32[0] & 0x3f) {
		ex(ADD_S);
		ex(SUB_S);
		ex(MUL_S);
		ex(DIV_S);
		ex(SQRT_S);
		ex(ABS_S);
		ex(MOV_S);
		ex(NEG_S);
		null();
		null();
		ex(RSQRT_S);
		null();
		ex(ADDA_S);
		ex(SUBA_S);
		ex(MULA_S);
		null();
		ex(MADD_S);
		ex(MSUB_S);
		ex(MADDA_S);
		ex(MSUBA_S);
		null();
		null();
		ex(CVT_W_S);
		null();
		null();
		null();
		null();
		null();
		null();
		ex(MAX_S);
		ex(MIN_S);
		null();
		null();
		ex(C_F_S);
		null();
		ex(C_EQ_S);
		null();
		ex(C_LT_S);
		null();
		ex(C_LE_S);
		null();
		null();
		null();
		null();
	}
}

static inline void FPU_W(reg32 opcode) {
	static const int baseval = __COUNTER__ + 1;

	switch (opcode._u32[0] & 0x3f) {
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		ex(CVT_S_W);
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
		null();
	}
}


void step(int instrs){
	for(int i = instrs; i > 0; i--) {
		reg32 op, pc;
		bool delay;
		u32 instr = memread32(pc32u);
		char dis[1024];
		disasm(instr, pc32u, dis);
		printf("%08x %s\n", pc32u, dis);


		delay = ps2.ee._in_branch_delay;
		interpret(instr);
		if(delay){
			//we just executed the delay slot so set new pc
			ps2.ee.pc = ps2.ee._pc_latch;
			ps2.ee._in_branch_delay = false;
		} else {
			pc32u+=4;
		}
	}
}
