#include "kettlingur.h"
#include "shorthand.h"
#include "ee.h"

static void branch(u32 addr){
	ps2.ee._in_branch_delay = true;
	ps2.ee._pc_latch._u32[0] = addr;
}

static void *readwrite(u32 addr) {
	void *host = NULL;
		if (addr >= 0x80000000 && addr < 0xa0000000) {
			addr -= 0x80000000;
		} else if (addr >= 0xa0000000 && addr < 0xc0000000) {
			addr -= 0xa0000000;
		} else if (addr >= 0xc0000000 && addr < 0xe0000000) {
			addr -= 0xc0000000;
		} else if (addr >= 0xe0000000){
			assert(false);
		}
		if (addr >= EE_RAM_START && addr < EE_RAM_END) {
			host = &ps2.ee.ram[addr - EE_RAM_START];
		} else if (addr >= EE_RAM_UNCACHED_START && addr < EE_RAM_UNCACHED_END) {
			host = &ps2.ee.ram[addr - EE_RAM_UNCACHED_START];
		} else if (addr >= EE_RAM_ACCELERATED_START && addr < EE_RAM_ACCELERATED_END) {
			host = &ps2.ee.ram[addr - EE_RAM_ACCELERATED_START];
		} else if (addr >= EE_REGS_START && addr < EE_REGS_END) {
			printf("%s\n",regtostr(addr));
			host = &ps2.ee.regs[addr - EE_REGS_START];
		} else if (addr >= VU_REGS_START && addr < VU_REGS_END) {
			return 0;
		} else if (addr >= GS_REGS_START && addr < GS_REGS_END) {
			return 0;
		} else if (addr >= BIOS_START && addr < BIOS_END) {
			host = &ps2.ee.bios[addr - BIOS_START];
		} else if (addr >= EE_SCRATCHPAD_START && addr < EE_SCRATCHPAD_END) {
			host = &ps2.ee.scratch[addr - EE_SCRATCHPAD_START];
		} else if (addr >= EE_IOP_RAM_START && addr < EE_IOP_RAM_END) {
			host = &ps2.iop.ram[addr - EE_IOP_RAM_START];
		}
		assert(addr != 0xb000f500);
		assert(host != NULL);
		return host;
	}

static void exception() {}

static u8 memread8(u32 addr) {
	u8* a = readwrite(addr);
	return *a ;
}

static u16 memread16(u32 addr) {
	u16* a = readwrite(addr);
	return *a;
}

u32 memread32(u32 addr) {
	u32* a = readwrite(addr);
	return *a;
}

static u64 memread64(u32 addr) {
	u64* a = readwrite(addr);
	return *a;
}

static u128 memread128(u32 addr) {
	return *(u128 *) readwrite(addr);
}

static void memwrite8(u32 addr, u8 data) {
	u8* a = readwrite(addr);
	*a = data;
}

static void memwrite16(u32 addr, u16 data) {
	u16* a = readwrite(addr);
	*a = data;
}

static void memwrite32(u32 addr, u32 data) {
	u32* a = readwrite(addr);
	*a = data;
}

static void memwrite64(u32 addr, u64 data) {
	u64* a = readwrite(addr);
	*a = data;
}

static void memwrite128(u32 addr, u64 data) {
	*(u128 *) readwrite(addr) = data;
}



static void OPCODE(reg32 opcode);

static void SPECIAL(reg32 opcode);

static void REGIMM(reg32 opcode);

static void COP0(reg32 opcode);

static void COP1(reg32 opcode);

static void COP2(reg32 opcode);

static void MMI(reg32 opcode);

static void MMI0(reg32 opcode);

static void MMI1(reg32 opcode);

static void MMI2(reg32 opcode);

static void MMI3(reg32 opcode);
static void C0(reg32 opcode);
static void BC1(reg32 opcode);


static void COP2(reg32 opcode) {}

static void SLL(reg32 opcode) {
	rd64s = rt32s << sa();
}

static void SRL(reg32 opcode) {
	rd64s = (s32) (rt32u >> sa());
}

static void SRA(reg32 opcode) {
	rd64s = rt32s >> sa();
}

static void SLLV(reg32 opcode) {
	rd64s = rt32s << (rt32s & 0x1f);
}

static void JR(reg32 opcode) {
	branch(rs32s);
}

static void JALR(reg32 opcode) {
	rd64s = pc32s + 8;
	branch(rs32s);
}

static void MOVZ(reg32 opcode) {
	if (rt64s == 0) {
		rd64s = rs64s;
	}
}

static void MOVN(reg32 opcode) {
	if (rt64s != 0) {
		rd64s = rs64s;
	}
}

static void SYSCALL(reg32 opcode) {
	switch(ps2.ee.gpr.v1._s32[0]) {
		case 0x01:  printf(" void ResetEE(int reset_flag)\n"); break;
		case 0x02:  printf(" void SetGsCrt(bool interlaced, int display_mode, bool frame)\n"); break;
		case 0x04:  printf(" void Exit(int status)\n"); break;
		case 0x05:  printf(" void _ExceptionEpilogue/RFU005()\n"); break;
		case 0x06:  printf(" void LoadExecPS2(const char* filename, int argc, char** argv)\n"); break;
		case 0x07:  printf(" void ExecPS2(void* entry, void* gp, int argc, char** argv)\n"); break;
		case 0x09:  printf(" rfu009()\n"); break;
		case 0x0a:  printf(" AddSbusIntcHandler\n"); break;
		case 0x0b:  printf(" RemoveSbusIntcHandler\n"); break;
		case 0x0c:  printf(" Interrupt2Iop\n"); break;
		case 0x0d:  printf(" SetVTLBRefillHandler\n"); break;
		case 0x0e:  printf(" SetVCommonHandler\n"); break;
		case 0x0f:  printf(" SetVInterruptHandler\n"); break;
		case 0x10:  printf(" int AddIntcHandler(int int_cause, int (*handler)(int), int next, void* arg, int flag)\n"); break;
		case 0x11:  printf(" int RemoveIntcHandler(int int_cause, int handler_id)\n"); break;
		case 0x12:  printf(" int AddDmacHandler(int dma_cause, int (*handler)(int), int next, void* arg, int flag)\n"); break;
		case 0x13:  printf(" int RemoveDmacHandler(int dma_cause, int handler_id)\n"); break;
		case 0x14:  printf(" bool _EnableIntc(int cause_bit)\n"); break;
		case 0x15:  printf(" bool _DisableIntc(int cause_bit)\n"); break;
		case 0x16:  printf(" bool _EnableDmac(int cause_bit)\n"); break;
		case 0x17:  printf(" bool _DisableDmac(int cause_bit)\n"); break;
		case 0x18:  printf(" SetAlarm\n"); break;
		case 0x19:  printf(" ReleaseAlarm\n"); break;
		case -0x1a:  printf(" _iEnableIntc\n"); break;
		case -0x1b:  printf(" _iDisableIntc\n"); break;
		case -0x1c:  printf(" _iEnableDmac\n"); break;
		case -0x1d:  printf(" _iDisableDmac\n"); break;
		case -0x1e:  printf(" iSetAlarm\n"); break;
		case -0x1f:  printf(" iReleaseAlarm\n"); break;
		case 0x20:  printf(" int CreateThread(ThreadParam* t)\n"); break;
		case 0x21:  printf(" void DeleteThread(int thread_id)\n"); break;
		case 0x22:  printf(" void StartThread(int thread_id, void* arg)\n"); break;
		case 0x23:  printf(" void ExitThread()\n"); break;
		case 0x24:  printf(" void ExitDeleteThread()\n"); break;
		case 0x25:  printf(" void TerminateThread(int thread_id)\n"); break;
		case -0x26:  printf(" void iTerminateThread(int thread_id)\n"); break;
		case 0x27:  printf(" DisableDispatchThread\n"); break;
		case 0x28:  printf(" EnableDispatchThread\n"); break;
		case 0x29:  printf(" int ChangeThreadPriority(int thread_id, int priority)\n"); break;
		case -0x2A:  printf(" int iChangeThreadPriority(int thread_id, int priority)\n"); break;
		case 0x2B:  printf(" void RotateThreadReadyQueue(int priority)\n"); break;
		case -0x2C:  printf(" int _iRotateThreadReadyQueue(int priority)\n"); break;
		case 0x2D:  printf(" void ReleaseWaitThread(int thread_id)\n"); break;
		case -0x2E:  printf(" int iReleaseWaitThread(int thread_id)\n"); break;
		case 0x2F:  printf(" int GetThreadId()\n"); break;
		case 0x30:  printf(" int ReferThreadStatus(int thread_id, ThreadParam* status)\n"); break;
		case -0x31:  printf(" int iReferThreadStatus(int thread_id, ThreadParam* status)\n"); break;
		case 0x32:  printf(" void SleepThread()\n"); break;
		case 0x33:  printf(" void WakeupThread(int thread_id)\n"); break;
		case -0x34:  printf(" int iWakeupThread(int thread_id)\n"); break;
		case 0x35:  printf(" int CancelWakeupThread(int thread_id)\n"); break;
		case -0x36:  printf(" int iCancelWakeupThread(int thread_id)\n"); break;
		case 0x37:  printf(" int SuspendThread(int thread_id)\n"); break;
		case -0x38:  printf(" int iSuspendThread(int thread_id)\n"); break;
		case 0x39:  printf(" void ResumeThread(int thread_id)\n"); break;
		case -0x3A:  printf(" int iResumeThread(int thread_id)\n"); break;
		case 0x3B:  printf(" void JoinThread()\n"); break;
		case 0x3C:  printf(" void* InitMainThread/RFU060(uint32 gp, void* stack, int stack_size, char* args, int root)\n"); break;
		case 0x3D:  printf(" void* InitHeap/RFU061(void* heap, int heap_size)\n"); break;
		case 0x3E:  printf(" void* EndOfHeap()\n"); break;
		case 0x40:  printf(" int CreateSema(SemaParam* s)\n"); break;
		case 0x41:  printf(" int DeleteSema(int sema_id)\n"); break;
		case 0x42:  printf(" int SignalSema(int sema_id)\n"); break;
		case -0x43:  printf(" int iSignalSema(int sema_id)\n"); break;
		case 0x44:  printf(" void WaitSema(int sema_id)\n"); break;
		case 0x45:  printf(" int PollSema(int sema_id)\n"); break;
		case -0x46:  printf(" int iPollSema(int sema_id)\n"); break;
		case 0x47: printf("ReferSemaStatus\n"); break;
		case -0x48: printf("iReferSemaStatus\n"); break;
		case -0x49: printf("iDeleteSema\n"); break;
		case 0x4a: printf("SetOsdConfigParam\n"); break;
		case 0x4b: printf("GetOsdConfigParam\n"); break;
		case 0x4c: printf("GetGsHParam\n"); break;
		case 0x4d: printf("GetGsVParam\n"); break;
		case 0x4e: printf("SetGsHParam\n"); break;
		case 0x4f: printf("SetGsVParam\n"); break;
		case 0x50: printf("CreateEventFlag\n"); break;
		case 0x51: printf("DeleteEventFlag\n"); break;
		case 0x52: printf("SetEventFlag\n"); break;
		case 0x53: printf("iSetEventFlag\n"); break;
		case 0x54: printf("xlaunch\n"); break;
		case 0x55: printf("PutTLBEntry\n"); break;
		case -0x55: printf("iPutTLBEntry\n"); break;
		case 0x56: printf("SetTLBEntry\n"); break;
		case -0x56: printf("iSetTLBEntry\n"); break;
		case 0x57: printf("GetTLBEntry\n"); break;
		case -0x57: printf("iGetTLBEntry\n"); break;
		case 0x58: printf("ProbeTLBEntry\n"); break;
		case -0x58: printf("iProbeTLBEntry\n"); break;
		case 0x59: printf("ExpandScratchpad\n"); break;
		case 0x5a: printf("copy\n"); break;
		case 0x5b: printf("getentryaddr\n"); break;
		case 0x5c: printf("EnableIntcHandler\n"); break;
		case -0x5c: printf("iEnableIntcHandler\n"); break;
		case 0x5d: printf("DisableIntcHandler\n"); break;
		case -0x5d: printf("iDisableIntcHandler\n"); break;
		case 0x5e: printf("EnableDmacHandler\n"); break;
		case -0x5e: printf("iEnableDmacHandler\n"); break;
		case 0x5f: printf("DisableDmacHandler\n"); break;
		case -0x5f: printf("iDisableDmacHandler\n"); break;
		case 0x60: printf("KSeg0\n"); break;
		case 0x61: printf("EnableCache\n"); break;
		case 0x62: printf("DisableCache\n"); break;
		case 0x63: printf("GetCop0\n"); break;
		case 0x64: printf("FlushCache\n"); break;
		case 0x66: printf("CpuConfig\n"); break;
		case -0x67: printf("iGetCop0\n"); break;
		case -0x68: printf("iFlushCache\n"); break;
		case 0x69: printf("RFU105\n"); break;
		case -0x6a: printf("iCpuConfig\n"); break;
		case 0x6b: printf("SifStopDma\n"); break;
		case 0x6c: printf("SetCPUTimerHandler\n"); break;
		case 0x6d: printf("SetCPUTimer\n"); break;
		case 0x6e: printf("SetOsdConfigParam2\n"); break;
		case 0x6f: printf("GetOsdConfigParam2\n"); break;
		case 0x70: printf("GsGetIMR\n"); break;
		case -0x70: printf("iGsGetIMR\n"); break;
		case 0x71: printf("GsPutIMR\n"); break;
		case -0x71: printf("iGsPutIMR\n"); break;
		case 0x72: printf("SetPgifHandler\n"); break;
		case 0x73: printf("SetVSyncFlag\n"); break;
		case 0x74: printf("SetSyscall\n"); break;
		case 0x76: printf("SifDmaStat\n"); break;
		case -0x76: printf("iSifDmaStat\n"); break;
		case 0x77: printf("SifSetDma\n"); break;
		case -0x77: printf("iSifSetDma\n"); break;
		case 0x78: printf("SifSetDChain\n"); break;
		case -0x78: printf("iSifSetDChain\n"); break;
		case 0x79: printf("SifSetReg\n"); break;
		case 0x7a: printf("SifGetReg\n"); break;
		case 0x7b: printf("ExecOSD\n"); break;
		case 0x7c: printf("Deci2Call\n"); break;
		case 0x7d: printf("PSMode\n"); break;
		case 0x7e: printf("MachineType\n"); break;
		case 0x7f: printf("GetMemorySize\n"); break;
		case 0x80: printf("GetGsDxDyOffset\n"); break;
		case 0x82: printf("InitTLB\n"); break;
		case 0x83: printf("FindAddress\n"); break;
		case 0x85: printf("SetMemoryMode\n"); break;
		case 0x87: printf("ExecPSX\n"); break;
		case 0xfc: printf("setalarm\n"); break;
		case 0xfe: printf("releasealarm\n"); break;
		default:
			{printf("unknown syscall number %d\n", ps2.ee.gpr.v1._u32[0]); assert(false);}
	}


}

static void BREAK(reg32 opcode) {}

static void SYNC(reg32 opcode) {}

static void MFHI(reg32 opcode) {
	rd64s = hi64s;
}

static void MTHI(reg32 opcode) {
	hi64s = rs64s;
}

static void MFLO(reg32 opcode) {
	rd64s = lo64s;
}

static void MTLO(reg32 opcode) {
	lo64s = rs64s;
}

static void DSLLV(reg32 opcode) {
	rd64u = rt64u << (rs64s & 0x3f);
}

static void DSRLV(reg32 opcode) {
	rd64u = rt64u >> (rs64s & 0x3f);
}

static void DSRAV(reg32 opcode) {
	rd64s = rt64s >> (rs64s & 0x3f);
}

static void MULT(reg32 opcode) {
	reg ret;
	ret._s64[0] = (s64) rs32s * (s64) rt32s;
	lo64s = ret._s32[0];
	hi64s = ret._s32[1];
}

static void MULTU(reg32 opcode) {
	reg ret;
	ret._u64[0] = (u64) rs32u * (u64) rt32u;
	lo64s = ret._s32[0];
	hi64s = ret._s32[1];
}

static void DIV(reg32 opcode) {
	lo64s = (s64) rs32s / (s64) rt32s;
	hi64s = (s64) rs32s % (s64) rt32s;
}

static void DIVU(reg32 opcode) {
	lo64s = (s32) (((u64) rs32u) / ((u64) rt32u));
	hi64s = (s32) (((u64) rs32u) % ((u64) rt32u));
}

static void ADD(reg32 opcode) {
	s32 res;
	if (__builtin_add_overflow(rs32s, rt32s, &res)) {
		exception();
		return;
	}
	rd64s = res;
}

static void ADDU(reg32 opcode) {
	s32 res;
	if (!__builtin_add_overflow(rs32s, rt32s, &res)) {
		rd64s = res;
	}
}

static void SUB(reg32 opcode) {
	s32 res;
	if (__builtin_sub_overflow(rs32s, rt32s, &res)) {
		exception();
		return;
	}
	rd64s = res;
}

static void SUBU(reg32 opcode) {
	s32 res;
	if (!__builtin_sub_overflow(rs32s, rt32s, &res)) {
		rd64s = res;
	}
}

static void AND(reg32 opcode) {
	rd64u = rs64u & rt64u;
}

static void OR(reg32 opcode) {
	rd64u = rs64u | rt64u;
}

static void XOR(reg32 opcode) {
	rd64u = rs64u ^ rt64u;
}

static void NOR(reg32 opcode) {
	rd64u = ~(rs64u | rt64u);
}

static void MFSA(reg32 opcode) {
	rd64u = ps2.ee.barrelshift._u64[0];
}

static void MTSA(reg32 opcode) {
	ps2.ee.barrelshift._u64[0] = rs64u;
}

static void SLT(reg32 opcode) {
	rd64s = rs64s < rt64s ? 1 : 0;
}

static void SLTU(reg32 opcode) {
	rd64s = rs64u < rt64u ? 1 : 0;
}

static void DADD(reg32 opcode) {
	s64 res;
	if (__builtin_add_overflow(rs64s, rt64s, &res)) {
		exception();
		return;
	}
	rd64s = res;
}

static void DADDU(reg32 opcode) {
	s64 res;
	if (!__builtin_add_overflow(rs64s, rt64s, &res)) {
		rd64s = res;
	}
}

static void DSUB(reg32 opcode) {
	s64 res;
	if (__builtin_sub_overflow(rs64s, rt64s, &res)) {
		exception();
		return;
	}
	rd64s = res;
}

static void DSUBU(reg32 opcode) {
	s64 res;
	if (!__builtin_sub_overflow(rs64s, rt64s, &res)) {
		rd64s = res;
	}
}

static void TGE(reg32 opcode) {}

static void TGEU(reg32 opcode) {}

static void TLT(reg32 opcode) {}

static void TLTU(reg32 opcode) {}

static void TEQ(reg32 opcode) {}

static void TNE(reg32 opcode) {}

static void DSLL(reg32 opcode) {
	rd64u = rt64u << sa();
}

static void DSRL(reg32 opcode) {
	rd64u = rt64u >> sa();
}

static void DSRA(reg32 opcode) {
	rd64s = rt64s >> sa();
}

static void DSLL32(reg32 opcode) {
	rd64u = rt64u << (sa() + 32);
}

static void DSRL32(reg32 opcode) {
	rd64u = rt64u >> (sa() + 32);
}

static void DSRA32(reg32 opcode) {
	rd64s = rt64s >> (sa() + 32);
}

static void BLTZ(reg32 opcode) {
	if (rs64s < 0) {
		pc32s += offset;
	}
}

static void BGEZ(reg32 opcode) {
	if (rs64s >= 0) {
		pc32s += offset;
	}
}

static void BLTZL(reg32 opcode) {
	if (rs64s < 0) {
		pc32s += offset;
	} else {
		pc32s += 4;
	}
}

static void BGEZL(reg32 opcode) {
	if (rs64s >= 0) {
		pc32s += offset;
	} else {
		pc32s += 4;
	}
}

static void TGEI(reg32 opcode) {}

static void TGEIU(reg32 opcode) {}

static void TLTI(reg32 opcode) {}

static void TLTIU(reg32 opcode) {}

static void TEQI(reg32 opcode) {}

static void TNEI(reg32 opcode) {}

static void BLTZAL(reg32 opcode) {
	ra64s = pc32s + 8;
	if (rs64s < 0) {
		pc32s += offset;
	}
}

static void BGEZAL(reg32 opcode) {
	ra64s = pc32s + 8;
	if (rs64s >= 0) {
		pc32s += offset;
	}
}

static void BLTZALL(reg32 opcode) {
	ra64s = pc32s + 8;
	if (rs64s < 0) {
		pc32s += offset;
	} else {
		pc32s += 4;
	}
}

static void BGEZALL(reg32 opcode) {
	ra64s = pc32s + 8;
	if (rs64s >= 0) {
		pc32s += offset;
	} else {
		pc32s += 4;
	}
}

static void MTSAB(reg32 opcode) {}

static void MTSAH(reg32 opcode) {}

static void MADD(reg32 opcode) {
	reg res;
	res._s64[0] = ((s64) rs32s * (s64) rt32s) + (s64) ((lo64u & 0xffffffffull) | (hi64u << 32));
	lo64s = res._s32[0];
	hi64s = res._s32[1];
	rd64s = res._s32[0];
}

static void MADDU(reg32 opcode) {
	reg res;
	res._u64[0] = ((u64) rs32u * (u64) rt32u) + ((lo64u & 0xffffffffull) | (hi64u << 32));
	lo64s = res._s32[0];
	hi64s = res._s32[1];
	rd64s = res._s32[0];
}

static void PLZCW(reg32 opcode) {}

static void MULT1(reg32 opcode) {}

static void MULTU1(reg32 opcode) {}

static void DIV1(reg32 opcode) {}

static void DIVU1(reg32 opcode) {}

static void MFC0(reg32 opcode) {
	rt32u = ps2.ee.cop0.raw[rd()]._u32[0];
}

static void MTC0(reg32 opcode) {
	ps2.ee.cop0.raw[rd()]._u32[0] = rt32u;
}

static void BC0F(reg32 opcode) {}

static void BC0T(reg32 opcode) {}

static void BC0FL(reg32 opcode) {}

static void BC0TL(reg32 opcode) {}

static void TLBR(reg32 opcode) {}

static void TLBWI(reg32 opcode) {}

static void TLBWR(reg32 opcode) {}

static void TLBP(reg32 opcode) {}

static void ERET(reg32 opcode) {}

static void EI(reg32 opcode) {}

static void DI(reg32 opcode) {}

static void MFC1(reg32 opcode) {}

static void DMFC1(reg32 opcode) {}

static void CFC1(reg32 opcode) {}

static void MTC1(reg32 opcode) {}

static void DMTC1(reg32 opcode) {}

static void CTC1(reg32 opcode) {}

static void BC1F(reg32 opcode) {}

static void BC1T(reg32 opcode) {}

static void BC1FL(reg32 opcode) {}

static void BC1TL(reg32 opcode) {}

static void MFC2(reg32 opcode) {}

static void CFC2(reg32 opcode) {}

static void MTC2(reg32 opcode) {}

static void CTC2(reg32 opcode) {}

static void BC2(reg32 opcode) {}

static void LWC1(reg32 opcode) {}

static void LWC2(reg32 opcode) {}

static void PREF(reg32 opcode) {}

static void LQC2(reg32 opcode) {}

static void LD(reg32 opcode) {
	rt64u = memread64(base32s + simm16);
}

static void SWC1(reg32 opcode) {}

static void SWC2(reg32 opcode) {}

static void SQC2(reg32 opcode) {}

static void SD(reg32 opcode) {
	memwrite64(base32s + simm16, rt64u);
}

static void J(reg32 opcode) {
	 branch((pc32u & 0xf0000000) | ((opcode._u32[0] << 2) & 0x0fffffff));
}

static void JAL(reg32 opcode) {
	ra64u = pc32u + 8;
	branch((pc32u & 0xf0000000) | ((opcode._u32[0] << 2) & 0x0fffffff));
}

static void BEQ(reg32 opcode) {
	if (rs64s == rt64s) {
		branch(pc32s + offset + 4);
	}
}

static void BNE(reg32 opcode) {
	if (rs64s != rt64s) {
		branch(pc32s + offset + 4);
	}
}

static void BLEZ(reg32 opcode) {
	if (rs64s <= 0) {
		branch(pc32s + offset + 4);
	}
}

static void BGTZ(reg32 opcode) {
	if (rs64s > 0) {
		branch(pc32s + offset + 4);
	}
}

static void ADDI(reg32 opcode) {
	s32 res;
	if (__builtin_add_overflow(rs32s, simm16, &res)) {
		exception();
		return;
	}
	rt64s = res;
}

static void ADDIU(reg32 opcode) {
	s32 res;
	if (!__builtin_add_overflow(rs32s, simm16, &res)) {
		rt64s = res;
	}
}

static void SLTI(reg32 opcode) {
	rd64s = rs64s < simm16 ? 1 : 0;
}

static void SLTIU(reg32 opcode) {
	rd64s = rs64s < (u64) (s64) simm16 ? 1 : 0;
}

static void ANDI(reg32 opcode) {
	rt64u = rs64u & uimm16;
}

static void ORI(reg32 opcode) {
	rt64u = rs64u | uimm16;
}

static void XORI(reg32 opcode) {
	rt64u = rs64u ^ uimm16;
}

static void LUI(reg32 opcode) {
	rt64s = ((s32) simm16) << 16;
}

static void BEQL(reg32 opcode) {
	if (rs64s == rt64s) {
		branch(pc32s + offset + 4);
	} else {
		pc32s += 4;
	}
}

static void BNEL(reg32 opcode) {
	if (rs64s != rt64s) {
		branch(pc32s + offset + 4);
	} else {
		pc32s += 4;
	}
}

static void BLEZL(reg32 opcode) {
	if (rs64s <= 0) {
		branch(pc32s + offset + 4);
	} else {
		pc32s += 4;
	}
}

static void BGTZL(reg32 opcode) {
	if (rs64s > 0) {
		branch(pc32s + offset + 4);
	} else {
		pc32s += 4;
	}
}

static void DADDI(reg32 opcode) {
	s64 res;
	if (__builtin_add_overflow(rs64s, simm16, &res)) {
		exception();
		return;
	}
	rt64s = res;
}

static void DADDIU(reg32 opcode) {
	s64 res;
	if (!__builtin_add_overflow(rs64s, simm16, &res)) {
		rt64s = res;
	}
}

static void LDL(reg32 opcode) {}

static void LDR(reg32 opcode) {}

static void LQ(reg32 opcode) {
	rti(u128,0) = memread128(base32s + simm16);}

static void SQ(reg32 opcode) {
	memwrite128(base32s + simm16, rti(u128,0));
}

static void LB(reg32 opcode) {
	rt64s = memread8(base32s + simm16);
}

static void LH(reg32 opcode) {
	rt64s = (s16) memread16(base32s + simm16);
}

static void LWL(reg32 opcode) {}

static void LW(reg32 opcode) {
	rt64s = (s32) memread32(base32s + simm16);
}

static void LBU(reg32 opcode) {
	rt64u = memread8(base32s + simm16);
}

static void LHU(reg32 opcode) {
	rt64u = memread16(base32s + simm16);
}

static void LWR(reg32 opcode) {}

static void LWU(reg32 opcode) {}

static void SB(reg32 opcode) {
	memwrite8(base32s + simm16, (u8) rt32u);
}

static void SH(reg32 opcode) {
	memwrite16(base32s + simm16, (u16) rt32u);
}

static void SWL(reg32 opcode) {}

static void SW(reg32 opcode) {
	memwrite32(base32s + simm16, (u32) rt32u);
}

static void SDL(reg32 opcode) {}

static void SDR(reg32 opcode) {}

static void SWR(reg32 opcode) {}

static void CACHE(reg32 opcode) {}

static void PADDW(reg32 opcode) {
	rdi(s32, 0) = rsi(s32, 0) + rti(s32, 0);
	rdi(s32, 1) = rsi(s32, 1) + rti(s32, 1);
	rdi(s32, 2) = rsi(s32, 2) + rti(s32, 2);
	rdi(s32, 3) = rsi(s32, 3) + rti(s32, 3);
}

static void PSUBW(reg32 opcode) {
	rdi(s32, 0) = rsi(s32, 0) + rti(s32, 0);
	rdi(s32, 1) = rsi(s32, 1) + rti(s32, 1);
	rdi(s32, 2) = rsi(s32, 2) + rti(s32, 2);
	rdi(s32, 3) = rsi(s32, 3) + rti(s32, 3);
}

static void PCGTW(reg32 opcode) {
	rdi(s32, 0) = rsi(s32, 0) > rti(s32, 0) ? -1 : 0;
	rdi(s32, 1) = rsi(s32, 1) > rti(s32, 1) ? -1 : 0;
	rdi(s32, 2) = rsi(s32, 2) > rti(s32, 2) ? -1 : 0;
	rdi(s32, 3) = rsi(s32, 3) > rti(s32, 3) ? -1 : 0;
}

static void PMAXW(reg32 opcode) {
	rdi(s32, 0) = rsi(s32, 0) > rti(s32, 0) ? rsi(s32, 0) : rti(s32, 0);
	rdi(s32, 1) = rsi(s32, 1) > rti(s32, 1) ? rsi(s32, 1) : rti(s32, 1);
	rdi(s32, 2) = rsi(s32, 2) > rti(s32, 2) ? rsi(s32, 2) : rti(s32, 2);
	rdi(s32, 3) = rsi(s32, 3) > rti(s32, 3) ? rsi(s32, 3) : rti(s32, 3);
}

static void PADDH(reg32 opcode) {
	rdi(s16, 0) = rsi(s16, 0) + rti(s16, 0);
	rdi(s16, 1) = rsi(s16, 1) + rti(s16, 1);
	rdi(s16, 2) = rsi(s16, 2) + rti(s16, 2);
	rdi(s16, 3) = rsi(s16, 3) + rti(s16, 3);
	rdi(s16, 4) = rsi(s16, 4) + rti(s16, 4);
	rdi(s16, 5) = rsi(s16, 5) + rti(s16, 5);
	rdi(s16, 6) = rsi(s16, 6) + rti(s16, 6);
	rdi(s16, 7) = rsi(s16, 7) + rti(s16, 7);
}

static void PSUBH(reg32 opcode) {

	rdi(s16, 0) = rsi(s16, 0) - rti(s16, 0);
	rdi(s16, 1) = rsi(s16, 1) - rti(s16, 1);
	rdi(s16, 2) = rsi(s16, 2) - rti(s16, 2);
	rdi(s16, 3) = rsi(s16, 3) - rti(s16, 3);
	rdi(s16, 4) = rsi(s16, 4) - rti(s16, 4);
	rdi(s16, 5) = rsi(s16, 5) - rti(s16, 5);
	rdi(s16, 6) = rsi(s16, 6) - rti(s16, 6);
	rdi(s16, 7) = rsi(s16, 7) - rti(s16, 7);
}

static void PCGTH(reg32 opcode) {

	rdi(s16, 0) = rsi(s16, 0) > rti(s16, 0) ? -1 : 0;
	rdi(s16, 1) = rsi(s16, 1) > rti(s16, 1) ? -1 : 0;
	rdi(s16, 2) = rsi(s16, 2) > rti(s16, 2) ? -1 : 0;
	rdi(s16, 3) = rsi(s16, 3) > rti(s16, 3) ? -1 : 0;
	rdi(s16, 4) = rsi(s16, 4) > rti(s16, 4) ? -1 : 0;
	rdi(s16, 5) = rsi(s16, 5) > rti(s16, 5) ? -1 : 0;
	rdi(s16, 6) = rsi(s16, 6) > rti(s16, 6) ? -1 : 0;
	rdi(s16, 7) = rsi(s16, 7) > rti(s16, 7) ? -1 : 0;
}

static void PMAXH(reg32 opcode) {

	rdi(s16, 0) = rsi(s16, 0) > rti(s16, 0) ? rsi(s16, 0) : rti(s16, 0);
	rdi(s16, 1) = rsi(s16, 1) > rti(s16, 1) ? rsi(s16, 1) : rti(s16, 1);
	rdi(s16, 2) = rsi(s16, 2) > rti(s16, 2) ? rsi(s16, 2) : rti(s16, 2);
	rdi(s16, 3) = rsi(s16, 3) > rti(s16, 3) ? rsi(s16, 3) : rti(s16, 3);
	rdi(s16, 4) = rsi(s16, 4) > rti(s16, 4) ? rsi(s16, 0) : rti(s16, 4);
	rdi(s16, 5) = rsi(s16, 5) > rti(s16, 5) ? rsi(s16, 1) : rti(s16, 5);
	rdi(s16, 6) = rsi(s16, 6) > rti(s16, 6) ? rsi(s16, 2) : rti(s16, 6);
	rdi(s16, 7) = rsi(s16, 7) > rti(s16, 7) ? rsi(s16, 3) : rti(s16, 7);
}

static void PADDB(reg32 opcode) {

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

static void PSUBB(reg32 opcode) {

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

static void PCGTB(reg32 opcode) {

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

static void PADDSW(reg32 opcode) {

	rdi(s32, 0) = satadd(rsi(s32, 0), rti(s32, 0));
	rdi(s32, 1) = satadd(rsi(s32, 1), rti(s32, 1));
	rdi(s32, 2) = satadd(rsi(s32, 2), rti(s32, 2));
	rdi(s32, 3) = satadd(rsi(s32, 3), rti(s32, 3));
}

static void PSUBSW(reg32 opcode) {

	rdi(s32, 0) = satsub(rsi(s32, 0), rti(s32, 0));
	rdi(s32, 1) = satsub(rsi(s32, 1), rti(s32, 1));
	rdi(s32, 2) = satsub(rsi(s32, 2), rti(s32, 2));
	rdi(s32, 3) = satsub(rsi(s32, 3), rti(s32, 3));
}

static void PEXTLW(reg32 opcode) {
	rdi(u32, 0) = rti(u32, 0);
	rdi(u32, 1) = rsi(u32, 0);
	rdi(u32, 2) = rti(u32, 1);
	rdi(u32, 3) = rsi(u32, 1);
}

static void PPACW(reg32 opcode) {
	rdi(s32, 0) = rti(s32, 0);
	rdi(s32, 1) = rti(s32, 2);
	rdi(s32, 2) = rsi(s32, 1);
	rdi(s32, 3) = rsi(s32, 3);
}

static void PADDSH(reg32 opcode) {
	rdi(s16, 0) = satadd(rsi(s16, 0), rti(s16, 0));
	rdi(s16, 1) = satadd(rsi(s16, 1), rti(s16, 1));
	rdi(s16, 2) = satadd(rsi(s16, 2), rti(s16, 2));
	rdi(s16, 3) = satadd(rsi(s16, 3), rti(s16, 3));
	rdi(s16, 4) = satadd(rsi(s16, 4), rti(s16, 4));
	rdi(s16, 5) = satadd(rsi(s16, 5), rti(s16, 5));
	rdi(s16, 6) = satadd(rsi(s16, 6), rti(s16, 6));
	rdi(s16, 7) = satadd(rsi(s16, 7), rti(s16, 7));
}

static void PSUBSH(reg32 opcode) {
	rdi(s16, 0) = satsub(rsi(s16, 0), rti(s16, 0));
	rdi(s16, 1) = satsub(rsi(s16, 1), rti(s16, 1));
	rdi(s16, 2) = satsub(rsi(s16, 2), rti(s16, 2));
	rdi(s16, 3) = satsub(rsi(s16, 3), rti(s16, 3));
	rdi(s16, 4) = satsub(rsi(s16, 4), rti(s16, 4));
	rdi(s16, 5) = satsub(rsi(s16, 5), rti(s16, 5));
	rdi(s16, 6) = satsub(rsi(s16, 6), rti(s16, 6));
	rdi(s16, 7) = satsub(rsi(s16, 7), rti(s16, 7));
}

static void PEXTLH(reg32 opcode) {}

static void PPACH(reg32 opcode) {}

static void PADDSB(reg32 opcode) {

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

static void PSUBSB(reg32 opcode) {

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

static void PEXTLB(reg32 opcode) {
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

static void PPACB(reg32 opcode) {
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

static void PEXT5(reg32 opcode) {
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

static void PPAC5(reg32 opcode) {}

static void PABSW(reg32 opcode) {}

static void PCEQW(reg32 opcode) {}

static void PMINW(reg32 opcode) {}

static void PADSBH(reg32 opcode) {}

static void PABSH(reg32 opcode) {}

static void PCEQH(reg32 opcode) {}

static void PMINH(reg32 opcode) {}

static void PCEQB(reg32 opcode) {}

static void PADDUW(reg32 opcode) {}

static void PSUBUW(reg32 opcode) {}

static void PEXTUW(reg32 opcode) {}

static void PADDUH(reg32 opcode) {}

static void PSUBUH(reg32 opcode) {}

static void PEXTUH(reg32 opcode) {}

static void PADDUB(reg32 opcode) {}

static void PSUBUB(reg32 opcode) {}

static void PEXTUB(reg32 opcode) {}

static void QFSRV(reg32 opcode) {}

static void PMADDW(reg32 opcode) {}

static void PSLLVW(reg32 opcode) {}

static void PSRLVW(reg32 opcode) {}

static void PMSUBW(reg32 opcode) {}

static void PMFHI(reg32 opcode) {}

static void PMFLO(reg32 opcode) {}

static void PINTH(reg32 opcode) {}

static void PMULTW(reg32 opcode) {}

static void PDIVW(reg32 opcode) {}

static void PCPYLD(reg32 opcode) {}

static void PMADDH(reg32 opcode) {}

static void PHMADH(reg32 opcode) {}

static void PAND(reg32 opcode) {
	rdi(u128, 0) = rsi(u128, 0) & rti(u128, 0);
}

static void PXOR(reg32 opcode) {
	rdi(u128, 0) = rsi(u128, 0) ^ rti(u128, 0);
}

static void PMSUBH(reg32 opcode) {}

static void PHMSBH(reg32 opcode) {}

static void PEXEH(reg32 opcode) {}

static void PREVH(reg32 opcode) {}

static void PMULTH(reg32 opcode) {}

static void PDIVBW(reg32 opcode) {}

static void PEXEW(reg32 opcode) {}

static void PROT3W(reg32 opcode) {}

static void PMADDUW(reg32 opcode) {}

static void PSRAVW(reg32 opcode) {}

static void PMTHI(reg32 opcode) {}

static void PMTLO(reg32 opcode) {}

static void PINTEH(reg32 opcode) {}

static void PMULTUW(reg32 opcode) {}

static void PDIVUW(reg32 opcode) {}

static void PCPYUD(reg32 opcode) {}

static void POR(reg32 opcode) {}

static void PNOR(reg32 opcode) {}

static void PEXCH(reg32 opcode) {}

static void PCPYH(reg32 opcode) {}

static void PEXCW(reg32 opcode) {}

static void PMFHL(reg32 opcode) {}

static void PMTHL(reg32 opcode) {}

static void PSLLH(reg32 opcode) {}

static void PSRLH(reg32 opcode) {}

static void PSRAH(reg32 opcode) {}

static void PSLLW(reg32 opcode) {}

static void PSRLW(reg32 opcode) {}

static void PSRAW(reg32 opcode) {}

static void SRLV(reg32 opcode) {}

static void SRAV(reg32 opcode) {}

static void MFHI1(reg32 opcode) {}

static void MTHI1(reg32 opcode) {}

static void MFLO1(reg32 opcode) {}

static void MTLO1(reg32 opcode) {}

static void MADD1(reg32 opcode) {}

static void MADDU1(reg32 opcode) {}

static void ADD_S(reg32 opcode) {}

static void SUB_S(reg32 opcode) {}

static void MUL_S(reg32 opcode) {}

static void DIV_S(reg32 opcode) {}

static void SQRT_S(reg32 opcode) {}

static void ABS_S(reg32 opcode) {}

static void MOV_S(reg32 opcode) {}

static void NEG_S(reg32 opcode) {}

static void SUBA_S(reg32 opcode) {}

static void MULA_S(reg32 opcode) {}

static void MADD_S(reg32 opcode) {}

static void MSUB_S(reg32 opcode) {}

static void MADDA_S(reg32 opcode) {}

static void MSUBA_S(reg32 opcode) {}

static void CVT_W_S(reg32 opcode) {}

static void MAX_S(reg32 opcode) {}

static void MIN_S(reg32 opcode) {}

static void C_F_S(reg32 opcode) {}

static void C_EQ_S(reg32 opcode) {}

static void C_LT_S(reg32 opcode) {}

static void C_LE_S(reg32 opcode) {}

static void RSQRT_S(reg32 opcode) {}

static void CVT_S(reg32 opcode) {}

static void CVT_S_W(reg32 opcode) {}

static void ADDA_S(reg32 opcode) {}

//vu macro mode
static void BC2F(reg32 opcode){}
static void BC2FL(reg32 opcode){}
static void BC2T(reg32 opcode){}
static void BC2TL(reg32 opcode){}
static void QMFC2(reg32 opcode){}
static void QMTC2(reg32 opcode){}
static void VABS(reg32 opcode){}
static void VADD(reg32 opcode){}
static void VADDi(reg32 opcode){}
static void VADDq(reg32 opcode){}
static void VADDbc(reg32 opcode){}
static void VADDA(reg32 opcode){}
static void VADDAi(reg32 opcode){}
static void VADDAq(reg32 opcode){}
static void VADDAbc(reg32 opcode){}
static void VCALLMS(reg32 opcode){}
static void VCALLMSR(reg32 opcode){}
static void VCLIP(reg32 opcode){}
static void VDIV(reg32 opcode){}
static void VFTOI0(reg32 opcode){}
static void VFTOI4(reg32 opcode){}
static void VFTOI12(reg32 opcode){}
static void VFTOI15(reg32 opcode){}
static void VIADD(reg32 opcode){}
static void VIADDI(reg32 opcode){}
static void VIAND(reg32 opcode){}
static void VILWR(reg32 opcode){}
static void VIOR(reg32 opcode){}
static void VISUB(reg32 opcode){}
static void VISWR(reg32 opcode){}
static void VITOF0(reg32 opcode){}
static void VITOF4(reg32 opcode){}
static void VITOF12(reg32 opcode){}
static void VITOF15(reg32 opcode){}

static void VLQD(reg32 opcode){}
static void VLQI(reg32 opcode){}
static void VMADD(reg32 opcode){}
static void VMADDi(reg32 opcode){}
static void VMADDq(reg32 opcode){}
static void VMADDbc(reg32 opcode){}
static void VMADDA(reg32 opcode){}
static void VMADDAi(reg32 opcode){}
static void VMADDAq(reg32 opcode){}
static void VMADDAbc(reg32 opcode){}
static void VMAX(reg32 opcode){}
static void VMAXi(reg32 opcode){}
static void VMAXbc(reg32 opcode){}

static void VMFIR(reg32 opcode){}

static void VMINI(reg32 opcode){}
static void VMINIi(reg32 opcode){}
static void VMINIbc(reg32 opcode){}

static void VMOVE(reg32 opcode){}
static void VMR32(reg32 opcode){}
static void VMSUB(reg32 opcode){}
static void VMSUBi(reg32 opcode){}
static void VMSUBq(reg32 opcode){}
static void VMSUBbc(reg32 opcode){}
static void VMSUBA(reg32 opcode){}
static void VMSUBAi(reg32 opcode){}
static void VMSUBAq(reg32 opcode){}
static void VMSUBAbc(reg32 opcode){}

static void VMTIR(reg32 opcode){}

static void VMUL(reg32 opcode){}
static void VMULi(reg32 opcode){}
static void VMULq(reg32 opcode){}
static void VMULbc(reg32 opcode){}
static void VMULA(reg32 opcode){}
static void VMULAi(reg32 opcode){}
static void VMULAq(reg32 opcode){}
static void VMULAbc(reg32 opcode){}
static void VNOP(reg32 opcode){}
static void VOPMULA(reg32 opcode){}
static void VOPMSUB(reg32 opcode){}
static void VRGET(reg32 opcode){}
static void VRINIT(reg32 opcode){}
static void VRNEXT(reg32 opcode){}
static void VRSQRT(reg32 opcode){}
static void VRXOR(reg32 opcode){}
static void VSQD(reg32 opcode){}
static void VSQI(reg32 opcode){}
static void VSQRT(reg32 opcode){}
static void VSUB(reg32 opcode){}
static void VSUBi(reg32 opcode){}
static void VSUBq(reg32 opcode){}
static void VSUBbc(reg32 opcode){}
static void VSUBA(reg32 opcode){}
static void VSUBAi(reg32 opcode){}
static void VSUBAq(reg32 opcode){}
static void VSUBAbc(reg32 opcode){}
static void VWAITQ(reg32 opcode){}



static void invalid(reg32 opcode) { printf("invalid %08x\n",opcode._u32[0]); assert(false); }

#define ex(func) case (__COUNTER__-baseval): func(opcode); break
#define null() case (__COUNTER__-baseval): invalid(opcode); break


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

static void SPECIAL(reg32 opcode) {
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

static void REGIMM(reg32 opcode) {
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

static void MMI(reg32 opcode) {
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


static void MMI0(reg32 opcode) {
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

static void MMI1(reg32 opcode) {
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


static void MMI2(reg32 opcode) {
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


static void MMI3(reg32 opcode) {
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

static void BC0(reg32 opcode) {
	static const int baseval = __COUNTER__ + 1;
	switch ((opcode._u32[0] >> 16) & 0x1f) {
		ex(BC0F);
		ex(BC0T);
		ex(BC0FL);
		ex(BC0TL);
	}
}

static void COP0(reg32 opcode) {
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
		ex(C0);
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

static void C0(reg32 opcode) {
	static const int baseval = __COUNTER__ + 1;

	switch (opcode._u32[0] & 0x3f) {
		null();
		ex(TLBR);
		ex(TLBWI);
		null();
		null();
		null();
		ex(TLBWR);
		null();
		ex(TLBP);
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
		ex(ERET);
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
		ex(EI);
		ex(DI);
	}
}

static void COP1(reg32 opcode) {
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

static void BC1(reg32 opcode) {
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

static void FPU_S(reg32 opcode) {
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

static void FPU_W(reg32 opcode) {
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
