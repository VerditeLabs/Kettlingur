#include "kettlingur.h"

char out[4096];



#define _rt ((opcode._u32[0] >> 16) & 0x1f)
#define _rd ((opcode._u32[0] >> 11) & 0x1f)
#define _rs ((opcode._u32[0] >> 21) & 0x1f)
#define _op decode_op(opcode)
#define _sa ((opcode._u32[0] >> 6) & 0x1f)
#define _ft ((opcode._u32[0] >> 11) & 0x1f)
#define _fd decode_sa(opcode)
#define _fs decode_rd(opcode)
#define _base _rs
#define _simm16  (opcode._s16[0])
#define _uimm16  (opcode._u16[0])
#define _funct decode_funct(opcode)
#define _branch decode_branch(opcode, pc)
#define _jump decode_jump(opcode, pc)

#define _rt_name reg_names[_rt]
#define _rd_name reg_names[_rd]
#define _rs_name reg_names[_rs]
#define _base_name reg_names[_rs]

#define _ft_name fpu_reg_names[_ft]

static const char *reg_names[] = {
		"ZERO", "AT", "V0", "V1", "A0", "A1", "A2", "A3", "T0", "T1", "T2",
                                  "T3",   "T4", "T5", "T6", "T7", "S0", "S1", "S2", "S3", "S4", "S5",
                                  "S6",   "S7", "T8", "T9", "K0", "K1", "GP", "SP", "FP", "RA"};

static const char *fpu_reg_names[] = {
		"f0",  "f1",  "f2",  "f3",  "f4",  "f5",  "f6",  "f7",  "f8",  "f9",  "f10",
                                      "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21",
                                      "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"};

static const char *cop0_reg_names[] = {
		"Index",    "Random",   "EntryLo0", "EntryLo1", "Context",  "PageMask", "Wired",    "(reserved)",
		"BadVAddr", "Count",    "EntryHi",  "Compare",  "Status",   "Cause",    "EPC",      "PRId",
		"Config",   "reserved", "reserved", "reserved", "reserved", "reserved", "reserved", "BadPAddr",
		"Debug",    "Perf",     "reserved", "reserved", "TagLo",    "TagHi",    "ErrorEPC", "reserved"};

static const char *num_to_reg_name(u32 num) {
	assert(num < 32);
	return reg_names[num];
}

static const char *num_to_cop0_reg_name(u32 num) {
	assert(num < 32);
	return cop0_reg_names[num];
}

static const char *num_to_fpu_reg_name(u32 num) {
	assert(num < 32);
	return fpu_reg_names[num];
}

static void rd_rs_rt(char *instr, reg32 opcode, reg32 pc, char *dest) {
	if (_rd == _rt) {
		sprintf(dest, "%s %s, %s", instr, _rd_name, _rs_name);
	} else if (_rd == _rs) {
		sprintf(dest, "%s %s, %s", instr, _rd_name, _rt_name);
	} else {
		sprintf(dest, "%s %s, %s, %s", instr, _rd_name, _rs_name, _rt_name);
	}
}

static void rt_rs_simm16(char *instr, reg32 opcode, reg32 pc, char *dest) {
	if (_rs == _rt) {
		sprintf(dest, "%s %s, %d", instr, _rt_name, _simm16);
	} else {
		sprintf(dest, "%s %s, %s, %d", instr, _rt_name, _rs_name, _simm16);
	}
}

static void rt_rs_uimm16(char *instr, reg32 opcode, reg32 pc, char *dest) {
	if (_rs == _rt) {
		sprintf(dest, "%s %s, 0x%04x", instr, _rt_name, _uimm16);
	} else {
		sprintf(dest, "%s %s, %s, 0x%04x", instr, _rt_name, _rs_name, _uimm16);
	}
}

static void rs_rt_offset(char *instr, reg32 opcode, reg32 pc, char *dest) {
	sprintf(dest, "%s %s, %s, 0x%08x", instr, _rs_name, _rt_name, _branch);
}

static void rs_offset(char *instr, reg32 opcode, reg32 pc, char *dest) {
	sprintf(dest, "%s %s, 0x%08x", instr, _rs_name, _branch);
}

static void rs_rt(char *instr, reg32 opcode, reg32 pc, char *dest) {
	sprintf(dest, "%s %s, %s", instr, _rs_name, _rt_name);
}

static void rd_rt_sa(char *instr, reg32 opcode, reg32 pc, char *dest) {
	if (_rd == _rt) {
		sprintf(dest, "%s %s, %d", instr, _rd_name, _sa);
	} else {
		sprintf(dest, "%s %s, %s, %d", instr, _rd_name, _rt_name, _sa);
	}
}

static void rd_rt_rs(char *instr, reg32 opcode, reg32 pc, char *dest) {
	sprintf(dest, "%s %s, %s, %s", instr, _rd_name, _rt_name, _rs_name);
}

static void rt_offset_base(char *instr, reg32 opcode, reg32 pc, char *dest) {
	sprintf(dest, "%s %s, %d(%s)", instr, _rt_name, _simm16, _base_name);
}

static void ft_offset_base(char *instr, reg32 opcode, reg32 pc, char *dest) {
	sprintf(dest, "%s %s, %d(%s)", instr, _ft_name, _simm16, _base_name);
}

static void rt_immediate(char *instr, reg32 opcode, reg32 pc, char *dest) {
	sprintf(dest, "%s %s, 0x%04x", instr, _rt_name, _uimm16);
}

static void rd(char *instr, reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "%s %s, ", instr, _rd_name); }

static void rs(char *instr, reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "%s %s, ", instr, _rs_name); }

static void offset(char *instr, reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "%s 0x%08x", instr, _branch); }

static void rt_rd(char *instr, reg32 opcode, reg32 pc, char *dest) {
	sprintf(dest, "%s %s, %s", instr, _rt_name, _rd_name);
}

static void rd_rt(char *instr, reg32 opcode, reg32 pc, char *dest) {
	sprintf(dest, "%s %s, %s", instr, _rd_name, _rt_name);
}
static void rd_rs(char *instr, reg32 opcode, reg32 pc, char *dest) {
	sprintf(dest, "%s %s, %s", instr, _rd_name, _rs_name);
}

static void dis_ADD(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("add", opcode, pc, dest); }

static void dis_ADDI(reg32 opcode, reg32 pc, char *dest) { rt_rs_simm16("addi", opcode, pc, dest); }

static void dis_ADDIU(reg32 opcode, reg32 pc, char *dest) { rt_rs_simm16("addiu", opcode, pc, dest); }

static void dis_ADDU(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("addu", opcode, pc, dest); }

static void dis_AND(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("and", opcode, pc, dest); }

static void dis_ANDI(reg32 opcode, reg32 pc, char *dest) { rt_rs_uimm16("andi", opcode, pc, dest); }

static void dis_BEQ(reg32 opcode, reg32 pc, char *dest) { rs_rt_offset("beq", opcode, pc, dest); }

static void dis_BEQL(reg32 opcode, reg32 pc, char *dest) { rs_rt_offset("beql", opcode, pc, dest); }

static void dis_BGEZ(reg32 opcode, reg32 pc, char *dest) { rs_offset("bgez", opcode, pc, dest); }

static void dis_BGEZAL(reg32 opcode, reg32 pc, char *dest) { rs_offset("bgezal", opcode, pc, dest); }

static void dis_BGEZALL(reg32 opcode, reg32 pc, char *dest) { rs_offset("bgezall", opcode, pc, dest); }

static void dis_BGEZL(reg32 opcode, reg32 pc, char *dest) { rs_offset("bgezl", opcode, pc, dest); }

static void dis_BGTZ(reg32 opcode, reg32 pc, char *dest) { rs_offset("bgtz", opcode, pc, dest); }

static void dis_BGTZL(reg32 opcode, reg32 pc, char *dest) { rs_offset("bgtzl", opcode, pc, dest); }

static void dis_BLEZ(reg32 opcode, reg32 pc, char *dest) { rs_offset("blez", opcode, pc, dest); }

static void dis_BLEZL(reg32 opcode, reg32 pc, char *dest) { rs_offset("blezl", opcode, pc, dest); }

static void dis_BLTZ(reg32 opcode, reg32 pc, char *dest) { rs_offset("bltz", opcode, pc, dest); }

static void dis_BLTZAL(reg32 opcode, reg32 pc, char *dest) { rs_offset("bltzal", opcode, pc, dest); }

static void dis_BLTZALL(reg32 opcode, reg32 pc, char *dest) { rs_offset("bltzall", opcode, pc, dest); }

static void dis_BLTZL(reg32 opcode, reg32 pc, char *dest) { rs_offset("bltzl", opcode, pc, dest); }

static void dis_BNE(reg32 opcode, reg32 pc, char *dest) { rs_rt_offset("bne", opcode, pc, dest); }

static void dis_BNEL(reg32 opcode, reg32 pc, char *dest) { rs_rt_offset("bnel", opcode, pc, dest); }

static void dis_BREAK(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "break"); }

static void dis_DADD(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("dadd", opcode, pc, dest); }

static void dis_DADDI(reg32 opcode, reg32 pc, char *dest) { rt_rs_simm16("daddi", opcode, pc, dest); }

static void dis_DADDIU(reg32 opcode, reg32 pc, char *dest) { rt_rs_simm16("daddiu", opcode, pc, dest); }

static void dis_DADDU(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("daddu", opcode, pc, dest); }

static void dis_DIV(reg32 opcode, reg32 pc, char *dest) { rs_rt("div", opcode, pc, dest); }

static void dis_DIVU(reg32 opcode, reg32 pc, char *dest) { rs_rt("divu", opcode, pc, dest); }

static void dis_DSLL(reg32 opcode, reg32 pc, char *dest) { rd_rt_sa("dsll", opcode, pc, dest); }

static void dis_DSLL32(reg32 opcode, reg32 pc, char *dest) { rd_rt_sa("dsll32", opcode, pc, dest); }

static void dis_DSLLV(reg32 opcode, reg32 pc, char *dest) { rd_rt_rs("dsllv", opcode, pc, dest); }

static void dis_DSRA(reg32 opcode, reg32 pc, char *dest) { rd_rt_sa("dsra", opcode, pc, dest); }

static void dis_DSRA32(reg32 opcode, reg32 pc, char *dest) { rd_rt_sa("dsra32", opcode, pc, dest); }

static void dis_DSRAV(reg32 opcode, reg32 pc, char *dest) { rd_rt_rs("dsrav", opcode, pc, dest); }

static void dis_DSRL(reg32 opcode, reg32 pc, char *dest) { rd_rt_sa("dsrl", opcode, pc, dest); }

static void dis_DSRL32(reg32 opcode, reg32 pc, char *dest) { rd_rt_sa("dsrl", opcode, pc, dest); }

static void dis_DSRLV(reg32 opcode, reg32 pc, char *dest) { rd_rt_rs("dsrlv", opcode, pc, dest); }

static void dis_DSUB(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("dsub", opcode, pc, dest); }

static void dis_DSUBU(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("dsubu", opcode, pc, dest); }

static void dis_j(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "j %08x", _jump); }

static void dis_JAL(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "jal %08x", _jump); }

static void dis_JALR(reg32 opcode, reg32 pc, char *dest) {
	if (_rs == 31) {
		sprintf(dest, "jalr %s", _rs_name);
	} else {
		sprintf(dest, "jalr %s, %s", _rd_name, _rs_name);
	}
}

static void dis_JR(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "jr %s", _rs_name); }

static void dis_LB(reg32 opcode, reg32 pc, char *dest) { rt_offset_base("lb", opcode, pc, dest); }

static void dis_LBU(reg32 opcode, reg32 pc, char *dest) { rt_offset_base("lbu", opcode, pc, dest); }

static void dis_LD(reg32 opcode, reg32 pc, char *dest) { rt_offset_base("ld", opcode, pc, dest); }

static void dis_LDL(reg32 opcode, reg32 pc, char *dest) { rt_offset_base("ldl", opcode, pc, dest); }

static void dis_LDR(reg32 opcode, reg32 pc, char *dest) { rt_offset_base("ldr", opcode, pc, dest); }

static void dis_LH(reg32 opcode, reg32 pc, char *dest) { rt_offset_base("lh", opcode, pc, dest); }

static void dis_LHU(reg32 opcode, reg32 pc, char *dest) { rt_offset_base("lhu", opcode, pc, dest); }

static void dis_LUI(reg32 opcode, reg32 pc, char *dest) { rt_immediate("lui", opcode, pc, dest); }

static void dis_LW(reg32 opcode, reg32 pc, char *dest) { rt_offset_base("lw", opcode, pc, dest); }

static void dis_LWL(reg32 opcode, reg32 pc, char *dest) { rt_offset_base("lwl", opcode, pc, dest); }

static void dis_LWR(reg32 opcode, reg32 pc, char *dest) { rt_offset_base("lwr", opcode, pc, dest); }

static void dis_LWU(reg32 opcode, reg32 pc, char *dest) { rt_offset_base("lwu", opcode, pc, dest); }

static void dis_MFHI(reg32 opcode, reg32 pc, char *dest) { rd("mfhi", opcode, pc, dest); }

static void dis_MFLO(reg32 opcode, reg32 pc, char *dest) { rd("mflo", opcode, pc, dest); }

static void dis_MOVN(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("movn", opcode, pc, dest); }

static void dis_MOVZ(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("movn", opcode, pc, dest); }

static void dis_MTHI(reg32 opcode, reg32 pc, char *dest) { rs("mthi", opcode, pc, dest); }

static void dis_MTLO(reg32 opcode, reg32 pc, char *dest) { rs("mtlo", opcode, pc, dest); }

static void dis_MULT(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("mult", opcode, pc, dest); }

static void dis_MULTU(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("multu", opcode, pc, dest); }

static void dis_NOR(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("nor", opcode, pc, dest); }

static void dis_OR(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("or", opcode, pc, dest); }

static void dis_ORI(reg32 opcode, reg32 pc, char *dest) { rt_rs_uimm16("ori", opcode, pc, dest); }

static void dis_PREF(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "pref"); }

static void dis_SB(reg32 opcode, reg32 pc, char *dest) { rt_offset_base("sb", opcode, pc, dest); }

static void dis_SD(reg32 opcode, reg32 pc, char *dest) { rt_offset_base("sd", opcode, pc, dest); }

static void dis_SDL(reg32 opcode, reg32 pc, char *dest) { rt_offset_base("sdl", opcode, pc, dest); }

static void dis_SDR(reg32 opcode, reg32 pc, char *dest) { rt_offset_base("sdr", opcode, pc, dest); }

static void dis_SH(reg32 opcode, reg32 pc, char *dest) { rt_offset_base("sh", opcode, pc, dest); }

static void dis_SLL(reg32 opcode, reg32 pc, char *dest) {
	if (opcode == 0x00000000) {
		sprintf(dest, "nop");
	} else {
		rd_rt_sa("sll", opcode, pc, dest);
	}
}

static void dis_SLLV(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("sllv", opcode, pc, dest); }

static void dis_SLT(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("slt", opcode, pc, dest); }

static void dis_SLTI(reg32 opcode, reg32 pc, char *dest) { rt_rs_simm16("slti", opcode, pc, dest); }

static void dis_SLTIU(reg32 opcode, reg32 pc, char *dest) { rt_rs_uimm16("sltiu", opcode, pc, dest); }

static void dis_SLTU(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("sltu", opcode, pc, dest); }

static void dis_SRA(reg32 opcode, reg32 pc, char *dest) { rd_rt_sa("sra", opcode, pc, dest); }

static void dis_SRAV(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("srav", opcode, pc, dest); }

static void dis_SRL(reg32 opcode, reg32 pc, char *dest) { rd_rt_sa("srl", opcode, pc, dest); }

static void dis_SRLV(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("srlv", opcode, pc, dest); }

static void dis_SUB(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("sub", opcode, pc, dest); }

static void dis_SUBU(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("subu", opcode, pc, dest); }

static void dis_SW(reg32 opcode, reg32 pc, char *dest) { rt_offset_base("sw", opcode, pc, dest); }

static void dis_SWL(reg32 opcode, reg32 pc, char *dest) { rt_offset_base("swl", opcode, pc, dest); }

static void dis_SWR(reg32 opcode, reg32 pc, char *dest) { rt_offset_base("swr", opcode, pc, dest); }

static void dis_SYNC(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "sync"); }

static void dis_SYSCALL(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "syscall"); }

static void dis_TEQ(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "teq"); }

static void dis_TEQI(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "teqi"); }

static void dis_TGE(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "tge"); }

static void dis_TGEI(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "tgei"); }

static void dis_TGEIU(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "tgeiu"); }

static void dis_TGEU(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "tgeu"); }

static void dis_TLT(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "tlt"); }

static void dis_TLTI(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "tlti"); }

static void dis_TLTIU(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "tltiu"); }

static void dis_TLTU(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "tltu"); }

static void dis_TNE(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "tne"); }

static void dis_TNEI(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "tnei"); }

static void dis_XOR(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("xor", opcode, pc, dest); }

static void dis_XORI(reg32 opcode, reg32 pc, char *dest) { rt_rs_uimm16("xori", opcode, pc, dest); }

static void dis_DIV1(reg32 opcode, reg32 pc, char *dest) { rs_rt("div1", opcode, pc, dest); }

static void dis_DIVU1(reg32 opcode, reg32 pc, char *dest) { rs_rt("divu1", opcode, pc, dest); }

static void dis_LQ(reg32 opcode, reg32 pc, char *dest) { rt_offset_base("lq", opcode, pc, dest); }

static void dis_MADD(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("madd", opcode, pc, dest); }

static void dis_MADD1(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("madd1", opcode, pc, dest); }

static void dis_MADDU(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("maddu", opcode, pc, dest); }

static void dis_MADDU1(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("maddu1", opcode, pc, dest); }

static void dis_MFHI1(reg32 opcode, reg32 pc, char *dest) { rd("mfhi1", opcode, pc, dest); }

static void dis_MFLO1(reg32 opcode, reg32 pc, char *dest) { rd("mflo1", opcode, pc, dest); }

static void dis_MFSA(reg32 opcode, reg32 pc, char *dest) { rd("mfsa", opcode, pc, dest); }

static void dis_MTHI1(reg32 opcode, reg32 pc, char *dest) { rs("mthi1", opcode, pc, dest); }

static void dis_MTLO1(reg32 opcode, reg32 pc, char *dest) { rs("mtlo1", opcode, pc, dest); }

static void dis_MTSA(reg32 opcode, reg32 pc, char *dest) { rs("mtsa", opcode, pc, dest); }

static void dis_MTSAB(reg32 opcode, reg32 pc, char *dest) {
	// rs_immediate("mtsab",opcode,pc,dest);
	sprintf(dest, "mtsab");
}

static void dis_MTSAH(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "mtsah"); }

static void dis_MULT1(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("mult1", opcode, pc, dest); }

static void dis_MULTU1(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("multu1", opcode, pc, dest); }

static void dis_PABSH(reg32 opcode, reg32 pc, char *dest) { rd_rt("pabsh", opcode, pc, dest); }

static void dis_PABSW(reg32 opcode, reg32 pc, char *dest) { rd_rt("pabsw", opcode, pc, dest); }

static void dis_PADDB(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("paddb", opcode, pc, dest); }

static void dis_PADDH(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("paddh", opcode, pc, dest); }

static void dis_PADDSB(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("paddsb", opcode, pc, dest); }

static void dis_PADDSH(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("paddsh", opcode, pc, dest); }

static void dis_PADDSW(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("paddsw", opcode, pc, dest); }

static void dis_PADDUB(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("paddub", opcode, pc, dest); }

static void dis_PADDUH(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("padduh", opcode, pc, dest); }

static void dis_PADDUW(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("padduw", opcode, pc, dest); }

static void dis_PADDW(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("paddw", opcode, pc, dest); }

static void dis_PADSBH(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("padsbh", opcode, pc, dest); }

static void dis_PAND(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("pand", opcode, pc, dest); }

static void dis_PCEQB(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("pceqb", opcode, pc, dest); }

static void dis_PCEQH(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("pceqh", opcode, pc, dest); }

static void dis_PCEQW(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("pceqw", opcode, pc, dest); }

static void dis_PCGTB(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("pcgtb", opcode, pc, dest); }

static void dis_PCGTH(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("pcgth", opcode, pc, dest); }

static void dis_PCGTW(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("pcgtw", opcode, pc, dest); }

static void dis_PCPYH(reg32 opcode, reg32 pc, char *dest) { rd_rt("pcpyh", opcode, pc, dest); }

static void dis_PCPYLD(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("pcpyld", opcode, pc, dest); }

static void dis_PCPYUD(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("pcpyud", opcode, pc, dest); }

static void dis_PDIVBW(reg32 opcode, reg32 pc, char *dest) { rs_rt("pdivbw", opcode, pc, dest); }

static void dis_PDIVUW(reg32 opcode, reg32 pc, char *dest) { rs_rt("pdivuw", opcode, pc, dest); }

static void dis_PDIVW(reg32 opcode, reg32 pc, char *dest) { rs_rt("pdivw", opcode, pc, dest); }

static void dis_PEXCH(reg32 opcode, reg32 pc, char *dest) { rd_rt("pexch", opcode, pc, dest); }

static void dis_PEXCW(reg32 opcode, reg32 pc, char *dest) { rd_rt("pexcw", opcode, pc, dest); }

static void dis_PEXEH(reg32 opcode, reg32 pc, char *dest) { rd_rt("pexeh", opcode, pc, dest); }

static void dis_PEXEW(reg32 opcode, reg32 pc, char *dest) { rd_rt("pexew", opcode, pc, dest); }

static void dis_PEXT5(reg32 opcode, reg32 pc, char *dest) { rd_rt("pext5", opcode, pc, dest); }

static void dis_PEXTLB(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("pextlb", opcode, pc, dest); }

static void dis_PEXTLH(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("pextlh", opcode, pc, dest); }

static void dis_PEXTLW(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("pextlw", opcode, pc, dest); }

static void dis_PEXTUB(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("pextub", opcode, pc, dest); }

static void dis_PEXTUH(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("pextuh", opcode, pc, dest); }

static void dis_PEXTUW(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("pextuw", opcode, pc, dest); }

static void dis_PHMADH(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("phmadh", opcode, pc, dest); }

static void dis_PHMSBH(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("phmsbh", opcode, pc, dest); }

static void dis_PINTEH(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("pinteh", opcode, pc, dest); }

static void dis_PINTH(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("pinth", opcode, pc, dest); }

static void dis_PLZCW(reg32 opcode, reg32 pc, char *dest) { rd_rs("plzcw", opcode, pc, dest); }

static void dis_PMADDH(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("pmaddh", opcode, pc, dest); }

static void dis_PMADDUW(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("pmadduw", opcode, pc, dest); }

static void dis_PMADDW(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("pmaddw", opcode, pc, dest); }

static void dis_PMAXH(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("pmaxh", opcode, pc, dest); }

static void dis_PMAXW(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("pmaxw", opcode, pc, dest); }

static void dis_PMFHI(reg32 opcode, reg32 pc, char *dest) { rd("pmfhi", opcode, pc, dest); }

static void dis_PMFHL_LW(reg32 opcode, reg32 pc, char *dest) { rd("pmfhl.lw", opcode, pc, dest); }

static void dis_PMFHL_UW(reg32 opcode, reg32 pc, char *dest) { rd("pmfhl.uw", opcode, pc, dest); }

static void dis_PMFHL_SLW(reg32 opcode, reg32 pc, char *dest) { rd("pmfhl.slw", opcode, pc, dest); }

static void dis_PMFHL_LH(reg32 opcode, reg32 pc, char *dest) { rd("pmfhl.lh", opcode, pc, dest); }

static void dis_PMFHL_SH(reg32 opcode, reg32 pc, char *dest) { rd("pmfhl.sh", opcode, pc, dest); }

static void dis_PMFLO(reg32 opcode, reg32 pc, char *dest) { rd("pmflo", opcode, pc, dest); }

static void dis_PMINH(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("pminh", opcode, pc, dest); }

static void dis_PMINW(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("pminw", opcode, pc, dest); }

static void dis_PMSUBH(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("pmsubh", opcode, pc, dest); }

static void dis_PMSUBW(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("pmsubw", opcode, pc, dest); }

static void dis_PMTHI(reg32 opcode, reg32 pc, char *dest) { rs("pmthi", opcode, pc, dest); }

static void dis_PMTHL_LW(reg32 opcode, reg32 pc, char *dest) { rs("pmthl.lw", opcode, pc, dest); }

static void dis_PMTLO(reg32 opcode, reg32 pc, char *dest) { rs("pmtlo", opcode, pc, dest); }

static void dis_PMULTH(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("pmulth", opcode, pc, dest); }

static void dis_PMULTUW(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("pmultuw", opcode, pc, dest); }

static void dis_PMULTW(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("pmultuw", opcode, pc, dest); }

static void dis_PNOR(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("pnor", opcode, pc, dest); }

static void dis_POR(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("por", opcode, pc, dest); }

static void dis_PPAC5(reg32 opcode, reg32 pc, char *dest) { rd_rt("ppac5", opcode, pc, dest); }

static void dis_PPACB(reg32 opcode, reg32 pc, char *dest) { rd_rt("ppacb", opcode, pc, dest); }

static void dis_PPACH(reg32 opcode, reg32 pc, char *dest) { rd_rt("ppach", opcode, pc, dest); }

static void dis_PPACW(reg32 opcode, reg32 pc, char *dest) { rd_rt("ppacw", opcode, pc, dest); }

static void dis_PREVH(reg32 opcode, reg32 pc, char *dest) { rd_rt("prevh", opcode, pc, dest); }

static void dis_PROT3W(reg32 opcode, reg32 pc, char *dest) { rd_rt("prot3w", opcode, pc, dest); }

static void dis_PSLLH(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("psllh", opcode, pc, dest); }

static void dis_PSLLVW(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("psllvw", opcode, pc, dest); }

static void dis_PSLLW(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("psllw", opcode, pc, dest); }

static void dis_PSRAH(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("psrah", opcode, pc, dest); }

static void dis_PSRAVW(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("psravw", opcode, pc, dest); }

static void dis_PSRAW(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("psraw", opcode, pc, dest); }

static void dis_PSRLH(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("psrlh", opcode, pc, dest); }

static void dis_PSRLVW(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("psrlvw", opcode, pc, dest); }

static void dis_PSRLW(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("psrlw", opcode, pc, dest); }

static void dis_PSUBB(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("psubb", opcode, pc, dest); }

static void dis_PSUBH(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("psubh", opcode, pc, dest); }

static void dis_PSUBSB(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("psubsb", opcode, pc, dest); }

static void dis_PSUBSH(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("psubsh", opcode, pc, dest); }

static void dis_PSUBSW(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("psubsw", opcode, pc, dest); }

static void dis_PSUBUB(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("psubub", opcode, pc, dest); }

static void dis_PSUBUH(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("psubuh", opcode, pc, dest); }

static void dis_PSUBUW(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("psubuw", opcode, pc, dest); }

static void dis_PSUBW(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("psubw", opcode, pc, dest); }

static void dis_PXOR(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("pxor", opcode, pc, dest); }

static void dis_QFSRV(reg32 opcode, reg32 pc, char *dest) { rd_rs_rt("qfsrv", opcode, pc, dest); }

static void dis_SQ(reg32 opcode, reg32 pc, char *dest) { rt_offset_base("sq", opcode, pc, dest); }

static void dis_BC0F(reg32 opcode, reg32 pc, char *dest) { offset("bc0f", opcode, pc, dest); }

static void dis_BC0FL(reg32 opcode, reg32 pc, char *dest) { offset("bc0fl", opcode, pc, dest); }

static void dis_BC0T(reg32 opcode, reg32 pc, char *dest) { offset("bc0t", opcode, pc, dest); }

static void dis_BC0TL(reg32 opcode, reg32 pc, char *dest) { offset("bc0tl", opcode, pc, dest); }

static void dis_CACHE(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "cache"); }

static void dis_DI(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "di"); }

static void dis_EI(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "ei"); }

static void dis_ERET(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "eret"); }

static void dis_MFBPC(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "mfbpc"); }

static void dis_MFC0(reg32 opcode, reg32 pc, char *dest) {
	sprintf(dest, "mfc0 %s, %s", _rt_name, num_to_cop0_reg_name(_rd));
}

static void dis_MFDAB(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "mfdab"); }

static void dis_MFDABM(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "mfdabm"); }

static void dis_MFDVB(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "mfdvb"); }

static void dis_MFDVBM(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "mfdvbm"); }

static void dis_MFIAB(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "mfiab"); }

static void dis_MFIABM(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "mfiabm"); }

static void dis_MFPC(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "mfpc"); }

static void dis_MFPS(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "mfps"); }

static void dis_MTBPC(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "mtbpc"); }

static void dis_MTC0(reg32 opcode, reg32 pc, char *dest) {
	sprintf(dest, "mtc0 %s, %s", _rt_name, num_to_cop0_reg_name(_rd));
}

static void dis_MTDAB(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "mtdab"); }

static void dis_MTDABM(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "mtdabm"); }

static void dis_MTDVB(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "mtdvb"); }

static void dis_MTDVBM(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "mtdvbm"); }

static void dis_MTIAB(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "mtiab"); }

static void dis_MTIABM(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "mtiabm"); }

static void dis_MTPC(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "mtpc"); }

static void dis_MTPS(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "mtps"); }

static void dis_TLBP(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "tlbp"); }

static void dis_TLBR(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "tlbr"); }

static void dis_TLBWI(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "tlbwi"); }

static void dis_TLBWR(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "tlbwr"); }

static void dis_ABS_S(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "abs.s"); }

static void dis_ADD_S(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "add.s"); }

static void dis_ADDA_S(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "adda.s"); }

static void dis_BC1F(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "bc1f"); }

static void dis_BC1FL(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "bc1fl"); }

static void dis_BC1T(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "bc1t"); }

static void dis_BC1TL(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "bc1tl"); }

static void dis_C_EQ_S(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "c.eq.s"); }

static void dis_C_F_W(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "c.f.s"); }

static void dis_C_LE_S(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "c.le.s"); }

static void dis_C_LT_S(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "c.lt.s"); }

static void dis_CFC1(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "cfc1"); }

static void dis_CTC1(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "ctc1"); }

static void dis_CVT_S_W(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "cvt.s.w"); }

static void dis_CVT_W_S(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "cvt.w.s"); }

static void dis_DIV_S(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "div.s"); }

static void dis_LWC1(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "lwc1"); }

static void dis_MADD_S(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "madd.s"); }

static void dis_MADDA_S(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "madda.s"); }

static void dis_MAX_S(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "max.s"); }

static void dis_MFC1(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "mfc1"); }

static void dis_MIN_S(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "min.s"); }

static void dis_MOV_S(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "mov.s"); }

static void dis_MSUB_S(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "msub.s"); }

static void dis_MSUBA_S(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "msuba.s"); }

static void dis_MTC1(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "mtc1"); }

static void dis_MUL_S(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "mul.s"); }

static void dis_MULA_S(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "mula.s"); }

static void dis_NEG_S(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "neg.s"); }

static void dis_RSQRT_S(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "rsqrt.s"); }

static void dis_SQRT_S(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "sqrt.s"); }

static void dis_SUB_S(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "mub.s"); }

static void dis_SUBA_S(reg32 opcode, reg32 pc, char *dest) { sprintf(dest, "suba.s"); }

static void dis_SWC1(reg32 opcode, reg32 pc, char *dest) { ft_offset_base("swc1", opcode, pc, dest); }

static void dis_SPECIAL(reg32 opcode, reg32 pc, char *dest);
static void dis_MMI(reg32 opcode, reg32 pc, char *dest);
static void dis_MMI0(reg32 opcode, reg32 pc, char *dest);
static void dis_MMI1(reg32 opcode, reg32 pc, char *dest);
static void dis_MMI2(reg32 opcode, reg32 pc, char *dest);
static void dis_MMI3(reg32 opcode, reg32 pc, char *dest);
static void dis_REGIMM(reg32 opcode, reg32 pc, char *dest);
static void dis_COP0(reg32 opcode, reg32 pc, char *dest);
static void dis_BC0(reg32 opcode, reg32 pc, char *dest);
static void dis_C0(reg32 opcode, reg32 pc, char *dest);
static void dis_COP1(reg32 opcode, reg32 pc, char *dest);
static void dis_BC1(reg32 opcode, reg32 pc, char *dest);
static void dis_S(reg32 opcode, reg32 pc, char *dest);
static void dis_W(reg32 opcode, reg32 pc, char *dest);
static void dis_COP2(reg32 opcode, reg32 pc, char *dest);

#undef su
#undef ex
#undef nullptr


char* invalid() { /* Handle invalid opcode */ }

#define ex(func) case (__COUNTER__-baseval): dis_##func(opcode, pc, dest); break;
#define null() case (__COUNTER__-baseval): invalid(); break;


char* disOPCODE(reg32 opcode, reg32 pc, char* dest) {
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

char* disSPECIAL(reg32 opcode, reg32 pc, char* dest) {
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

char* disREGIMM(reg32 opcode, reg32 pc, char* dest) {
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

char* disMMI(reg32 opcode, reg32 pc, char* dest) {
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


char* disMMI0(reg32 opcode, reg32 pc, char* dest) {
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

char* disMMI1(reg32 opcode, reg32 pc, char* dest) {
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


char* disMMI2(reg32 opcode, reg32 pc, char* dest) {
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


char* disMMI3(reg32 opcode, reg32 pc, char* dest) {
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

char* disBC0(reg32 opcode, reg32 pc, char* dest) {
	static const int baseval = __COUNTER__ + 1;
	switch ((opcode._u32[0] >> 16) & 0x1f) {
		ex(BC0F)
		ex(BC0T)
		ex(BC0FL)
		ex(BC0TL)
	}
}

char* disCOP0(reg32 opcode, reg32 pc, char* dest) {
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

char* disTLB(reg32 opcode, reg32 pc, char* dest) {
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

char* disCOP1(reg32 opcode, reg32 pc, char* dest) {
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

char* disBC1(reg32 opcode, reg32 pc, char* dest) {
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

char* disFPU_S(reg32 opcode, reg32 pc, char* dest) {
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

char* disFPU_W(reg32 opcode, reg32 pc, char* dest) {
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
