#include "ps2.h"

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <thirdparty/elf.h>

#define true 1
#define false 0
#define bool _Bool

#define overload __attribute__((overloadable))
#define auto __auto_type


#define die(msg, ...)                                                                                                 \
  do {                                                                                                                 \
    printf("encountered fatal error at in file %s:%d\n", __FILE__,__LINE__);                                          \
    printf(msg, ##__VA_ARGS__);                                                                                        \
    exit(1);                                                                                                           \
  } while (0)

#define dieif(condition, msg, ...)                                                                                   \
  do {                                                                                                                 \
    if (condition) {                                                                                                   \
      die(msg, ##__VA_ARGS__);                                                                                        \
    }                                                                                                                  \
  } while (0)

#define dieifnot(condition, msg, ...)                                                                               \
  do {                                                                                                                 \
    if (!(condition)) {                                                                                                \
      die(msg, ##__VA_ARGS__);                                                                                        \
    }                                                                                                                  \
  } while (0)

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


#define RCNT0_COUNT			0x10000000
#define RCNT0_MODE			0x10000010
#define RCNT0_TARGET		0x10000020
#define RCNT0_HOLD			0x10000030
#define RCNT1_COUNT			0x10000800
#define RCNT1_MODE			0x10000810
#define RCNT1_TARGET		0x10000820
#define RCNT1_HOLD			0x10000830
#define RCNT2_COUNT			0x10001000
#define RCNT2_MODE			0x10001010
#define RCNT2_TARGET		0x10001020
#define RCNT3_COUNT			0x10001800
#define RCNT3_MODE			0x10001810
#define RCNT3_TARGET		0x10001820
#define IPU_CMD				0x10002000
#define IPU_CTRL			0x10002010
#define IPU_BP				0x10002020
#define IPU_TOP				0x10002030
#define GIF_CTRL			0x10003000
#define GIF_MODE			0x10003010
#define GIF_STAT			0x10003020
#define GIF_TAG0			0x10003040
#define GIF_TAG1			0x10003050
#define GIF_TAG2			0x10003060
#define GIF_TAG3			0x10003070
#define GIF_CNT				0x10003080
#define GIF_P3CNT			0x10003090
#define GIF_P3TAG			0x100030A0
#define VIF0_STAT		 	0x10003800
#define VIF0_FBRST		 	0x10003810
#define VIF0_ERR		 	0x10003820
#define VIF0_MARK		 	0x10003830
#define VIF0_CYCLE			0x10003840
#define VIF0_MODE		 	0x10003850
#define VIF0_NUM		 	0x10003860
#define VIF0_MASK		 	0x10003870
#define VIF0_CODE		 	0x10003880
#define VIF0_ITOPS			0x10003890
#define VIF0_ITOP		 	0x100038d0
#define VIF0_TOP			0x100038e0
#define VIF0_ROW0		 	0x10003900
#define VIF0_ROW1		 	0x10003910
#define VIF0_ROW2		 	0x10003920
#define VIF0_ROW3			0x10003930
#define VIF0_COL0		 	0x10003940
#define VIF0_COL1		 	0x10003950
#define VIF0_COL2			0x10003960
#define VIF0_COL3		 	0x10003970
#define VIF1_STAT			0x10003c00
#define VIF1_FBRST			0x10003c10
#define VIF1_ERR		 	0x10003c20
#define VIF1_MARK		 	0x10003c30
#define VIF1_CYCLE			0x10003c40
#define VIF1_MODE		 	0x10003c50
#define VIF1_NUM			0x10003c60
#define VIF1_MASK			0x10003c70
#define VIF1_CODE			0x10003c80
#define VIF1_ITOPS		 	0x10003c90
#define VIF1_BASE			0x10003ca0
#define VIF1_OFST		 	0x10003cb0
#define VIF1_TOPS		 	0x10003cc0
#define VIF1_ITOP		 	0x10003cd0
#define VIF1_TOP		 	0x10003ce0
#define VIF1_ROW0		 	0x10003d00
#define VIF1_ROW1		 	0x10003d10
#define VIF1_ROW2			0x10003d20
#define VIF1_ROW3			0x10003d30
#define VIF1_COL0			0x10003d40
#define VIF1_COL1			0x10003d50
#define VIF1_COL2		 	0x10003d60
#define VIF1_COL3		 	0x10003d70
#define VIF0_FIFO			0x10004000
#define VIF1_FIFO			0x10005000
#define GIF_FIFO			0x10006000
#define IPUout_FIFO			0x10007000
#define IPUin_FIFO			0x10007010
#define D0_CHCR				0x10008000
#define D0_MADR				0x10008010
#define D0_QWC				0x10008020
#define D0_TADR				0x10008030
#define D0_ASR0				0x10008040
#define D0_ASR1				0x10008050
#define VIF0_CHCR			0x10008000
#define VIF0_MADR			0x10008010
#define VIF0_QWC			0x10008020
#define VIF0_TADR			0x10008030
#define VIF0_ASR0			0x10008040
#define VIF0_ASR1			0x10008050
#define D1_CHCR				0x10009000
#define D1_MADR				0x10009010
#define D1_QWC				0x10009020
#define D1_TADR				0x10009030
#define D1_ASR0				0x10009040
#define D1_ASR1				0x10009050
#define VIF1_CHCR			0x10009000
#define VIF1_MADR			0x10009010
#define VIF1_QWC			0x10009020
#define VIF1_TADR			0x10009030
#define VIF1_ASR0			0x10009040
#define VIF1_ASR1			0x10009050
#define D2_CHCR				0x1000A000
#define D2_MADR				0x1000A010
#define D2_QWC				0x1000A020
#define D2_TADR				0x1000A030
#define D2_ASR0				0x1000A040
#define D2_ASR1				0x1000A050
#define GIF_CHCR			0x1000A000
#define GIF_MADR			0x1000A010
#define GIF_QWC				0x1000A020
#define GIF_TADR			0x1000A030
#define GIF_ASR0			0x1000A040
#define GIF_ASR1			0x1000A050
#define D3_CHCR				0x1000B000
#define D3_MADR				0x1000B010
#define D3_QWC				0x1000B020
#define fromIPU_CHCR		0x1000B000
#define fromIPU_MADR		0x1000B010
#define fromIPU_QWC			0x1000B020
#define D4_CHCR				0x1000B400
#define D4_MADR				0x1000B410
#define D4_QWC				0x1000B420
#define D4_TADR				0x1000B430
#define toIPU_CHCR			0x1000B400
#define toIPU_MADR			0x1000B410
#define toIPU_QWC			0x1000B420
#define toIPU_TADR			0x1000B430
#define D5_CHCR				0x1000C000
#define D5_MADR				0x1000C010
#define D5_QWC				0x1000C020
#define SIF0_CHCR			0x1000C000
#define SIF0_MADR			0x1000C010
#define SIF0_QWC			0x1000C020
#define D6_CHCR				0x1000C400
#define D6_MADR				0x1000C410
#define D6_QWC				0x1000C420
#define D6_TADR				0x1000C430
#define SIF1_CHCR			0x1000C400
#define SIF1_MADR			0x1000C410
#define SIF1_QWC			0x1000C420
#define SIF1_TADR			0x1000C430
#define D7_CHCR				0x1000C800
#define D7_MADR				0x1000C810
#define D7_QWC				0x1000C820
#define SIF2_CHCR			0x1000C800
#define SIF2_MADR			0x1000C810
#define SIF2_QWC			0x1000C820
#define D8_CHCR				0x1000D000
#define D8_MADR				0x1000D010
#define D8_QWC				0x1000D020
#define D8_SADR				0x1000D080
#define fromSPR_CHCR		0x1000D000
#define fromSPR_MADR		0x1000D010
#define fromSPR_QWC			0x1000D020
#define fromSPR_SADR		0x1000D080
#define D9_CHCR				0x1000D400
#define D9_MADR				0x1000D410
#define D9_QWC				0x1000D420
#define D9_TADR				0x1000D430
#define D9_SADR				0x1000D480
#define toSPR_CHCR			0x1000D400
#define toSPR_MADR			0x1000D410
#define toSPR_QWC			0x1000D420
#define toSPR_TADR			0x1000D430
#define toSPR_SADR			0x1000D480
#define DMAC_CTRL			0x1000E000
#define DMAC_STAT			0x1000E010
#define DMAC_PCR			0x1000E020
#define DMAC_SQWC			0x1000E030
#define DMAC_RBSR			0x1000E040
#define DMAC_RBOR			0x1000E050
#define DMAC_STADR			0x1000E060
#define DMAC_FAKESTAT		0x1000E100
#define INTC_STAT			0x1000F000
#define INTC_MASK			0x1000F010
#define SIO_LCR				0x1000F100
#define SIO_LSR				0x1000F110
#define SIO_IER				0x1000F120
#define SIO_ISR				0x1000F130
#define SIO_FCR				0x1000F140
#define SIO_BGR				0x1000F150
#define SIO_TXFIFO			0x1000F180
#define SIO_RXFIFO			0x1000F1C0
#define SBUS_F200			0x1000F200
#define SBUS_F210			0x1000F210
#define SBUS_F220			0x1000F220
#define SBUS_F230			0x1000F230
#define SBUS_F240			0x1000F240
#define SBUS_F250			0x1000F250
#define SBUS_F260			0x1000F260
#define SBUS_F300			0x1000F300
#define SBUS_F380			0x1000F380
#define MCH_RICM			0x1000F430
#define MCH_DRD				0x1000F440
#define DMAC_ENABLER		0x1000F520
#define DMAC_ENABLEW		0x1000F590


#define EXC2CODE_RESET 0
#define EXC2CODE_NMI 1
#define EXC2CODE_PERF 2
#define EXC2CODE_DEBUG 4
#define EXCCODE_INTERRUPT 0
#define EXCCODE_TLBMODIFIED 1
#define EXCCODE_TLBREFILL_LOAD 2
#define EXCCODE_TLBREFILL_STORE 3
#define EXCCODE_TLBINVALID_LOAD 2
#define EXCCODE_TLBINVALID_STORE 3
#define EXCCODE_ADDRERROR_LOAD 4
#define EXCCODE_ADDRERROR_STORE 5
#define EXCCODE_ADDRERROR_STORE 5
#define EXCCODE_BUSERROR_FETCH 6
#define EXCCODE_BUSERROR_LOADSTORE 7
#define EXCCODE_SYSCALL 8
#define EXCCODE_BREAK 9
#define EXCCODE_RESERVED 10
#define EXCCODE_COPUNUSABLE 11
#define EXCCODE_OVERFLOW 12
#define EXCCODE_TRAP 13



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

static overload s8 satadd(s8 a, s8 b){
	s16 ret = (s16)a + (s16)b;
	if(ret > 0x7f){
		return 0x7f;
	} else if(ret < -0x80){
		return (s8)0x80;
	}
	return (s8)ret;
}

static overload s16 satadd(s16 a, s16 b){
	s32 ret = ((s32)a) + (s32)b;
	if(ret > 0x7fff){
		return 0x7fff;
	} else if(ret < -0x8000){
		return (s16)0x8000;
	}
	return (s16)ret;
}

static overload s32 satadd(s32 a, s32 b){
	s64 ret = ((s64)a) + (s64)b;
	if(ret > 0x7fffffff){
		return 0x7fffffff;
	} else if(ret < -0x80000000ll){
		return (s32)0x80000000;
	}
	return (s32)ret;
}


static overload u8 satadd(u8 a, u8 b){
	u16 ret = (u16)a + (u16)b;
	if(ret > 0x7f){
		return 0x7f;
	}return ret;
}

static overload u16 satadd(u16 a, u16 b){
	u32 ret = (u32)a + (u32)b;
	if(ret > 0x7fff){
		return 0x7fff;
	}
	return ret;
}

static overload u32 satadd(u32 a, u32 b){
	u64 ret = (u64)a + (u64)b;
	if(ret > 0x7fffffff){
		return 0x7fffffff;
	}
	return ret;
}

static overload s8 satsub(s8 a, s8 b){
	s16 ret = ((s16)a) - (s16)b;
	if(ret > 0x7f){
		return 0x7f;
	} else if(ret < -0x80){
		return (s8)0x80;
	}
	return (s8)ret;
}

static overload s16 satsub(s16 a, s16 b){
	s32 ret = ((s32)a) - (s32)b;
	if(ret > 0x7fff){
		return 0x7fff;
	} else if(ret < -0x8000){
		return (s16)0x8000;
	}
	return (s16)ret;
}

static overload s32 satsub(s32 a, s32 b){
	s64 ret = ((s64)a) - (s64)b;
	if(ret > 0x7fffffff){
		return 0x7fffffff;
	} else if(ret < -0x80000000ll){
		return (s32)0x80000000;
	}
	return (s32)ret;
}


static overload u8 satsub(u8 a, u8 b){
	if(b > a) return 0;
	return a - b;
}

static overload u16 satsub(u16 a, u16 b){
	if(b > a) return 0;
	return a - b;
}

static overload u32 satsub(u32 a, u32 b){
	if(b > a) return 0;
	return a - b;
}


static overload s32 satabs(s32 val) {
	if(val == 0x80000000) {
		return 0x7fffffff;
	}
	if(val < 0) {
		return val * -1;
	}
	return val;
}


static overload s16 satabs(s16 val) {
	if(val == 0x8000) {
		return 0x7fff;
	}
	if(val < 0) {
		return val * -1;
	}
	return val;
}



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
	struct {f32 x,y,z,w;};
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
		reg index,random,entrylo0, entrylo1,context,pagemask,wired,_inv7,count,entryhi,compare,status,cause,epc,prid,config,
				inv17,inv18,inv19,inv20,inv21,inv22,badpaddr,debug,perf,inv26,inv27,taglo,taghi,errorepc,inv31;
	};
};
struct cop1 {
	reg32 fpr[32];
	reg32 fcr[32];
};

struct mips {
	reg32 pc, _pc_latch;
	union gpr gpr;
	bool _in_branch_delay;

	reg lo, hi, barrelshift;
	u8* ram, *regs, *scratch, *bios;
	union cop0 cop0;
	struct cop1 cop1;
};

struct ps2{
	struct mips ee;
	struct mips iop;

	struct{
		reg vf[32];
		u16 vi[16];
		reg acc;
		f32 q,p,r,i;
		u32 mac;
		u32 clip;
		u32 status;
		u8 *code, *data;
	}vu0, vu1;


	struct {
		u8* regs;
	}gs;

};


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


static char* regtostr(u32 addr) {
	switch(addr){
		case RCNT0_COUNT: return "RCNT0_COUNT"; break;
		case RCNT0_MODE: return "RCNT0_MODE"; break;
		case RCNT0_TARGET: return "RCNT0_TARGET"; break;
		case RCNT0_HOLD: return "RCNT0_HOLD"; break;
		case RCNT1_COUNT: return "RCNT1_COUNT"; break;
		case RCNT1_MODE: return "RCNT1_MODE"; break;
		case RCNT1_TARGET: return "RCNT1_TARGET"; break;
		case RCNT1_HOLD: return "RCNT1_HOLD"; break;
		case RCNT2_COUNT: return "RCNT2_COUNT"; break;
		case RCNT2_MODE: return "RCNT2_MODE"; break;
		case RCNT2_TARGET: return "RCNT2_TARGET"; break;
		case RCNT3_COUNT: return "RCNT3_COUNT"; break;
		case RCNT3_MODE: return "RCNT3_MODE"; break;
		case RCNT3_TARGET: return "RCNT3_TARGET"; break;
		case IPU_CMD: return "IPU_CMD"; break;
		case IPU_CTRL: return "IPU_CTRL"; break;
		case IPU_BP: return "IPU_BP"; break;
		case IPU_TOP: return "IPU_TOP"; break;
		case GIF_CTRL: return "GIF_CTRL"; break;
		case GIF_MODE: return "GIF_MODE"; break;
		case GIF_STAT: return "GIF_STAT"; break;
		case GIF_TAG0: return "GIF_TAG0"; break;
		case GIF_TAG1: return "GIF_TAG1"; break;
		case GIF_TAG2: return "GIF_TAG2"; break;
		case GIF_TAG3: return "GIF_TAG3"; break;
		case GIF_CNT: return "GIF_CNT"; break;
		case GIF_P3CNT: return "GIF_P3CNT"; break;
		case GIF_P3TAG: return "GIF_P3TAG"; break;
		case VIF0_STAT: return "VIF0_STAT"; break;
		case VIF0_FBRST: return "VIF0_FBRST"; break;
		case VIF0_ERR: return "VIF0_ERR"; break;
		case VIF0_MARK: return "VIF0_MARK"; break;
		case VIF0_CYCLE: return "VIF0_CYCLE"; break;
		case VIF0_MODE: return "VIF0_MODE"; break;
		case VIF0_NUM: return "VIF0_NUM"; break;
		case VIF0_MASK: return "VIF0_MASK"; break;
		case VIF0_CODE: return "VIF0_CODE"; break;
		case VIF0_ITOPS: return "VIF0_ITOPS"; break;
		case VIF0_ITOP: return "VIF0_ITOP"; break;
		case VIF0_TOP: return "VIF0_TOP"; break;
		case VIF0_ROW0: return "VIF0_ROW0"; break;
		case VIF0_ROW1: return "VIF0_ROW1"; break;
		case VIF0_ROW2: return "VIF0_ROW2"; break;
		case VIF0_ROW3: return "VIF0_ROW3"; break;
		case VIF0_COL0: return "VIF0_COL0"; break;
		case VIF0_COL1: return "VIF0_COL1"; break;
		case VIF0_COL2: return "VIF0_COL2"; break;
		case VIF0_COL3: return "VIF0_COL3"; break;
		case VIF1_STAT: return "VIF1_STAT"; break;
		case VIF1_FBRST: return "VIF1_FBRST"; break;
		case VIF1_ERR: return "VIF1_ERR"; break;
		case VIF1_MARK: return "VIF1_MARK"; break;
		case VIF1_CYCLE: return "VIF1_CYCLE"; break;
		case VIF1_MODE: return "VIF1_MODE"; break;
		case VIF1_NUM: return "VIF1_NUM"; break;
		case VIF1_MASK: return "VIF1_MASK"; break;
		case VIF1_CODE: return "VIF1_CODE"; break;
		case VIF1_ITOPS: return "VIF1_ITOPS"; break;
		case VIF1_BASE: return "VIF1_BASE"; break;
		case VIF1_OFST: return "VIF1_OFST"; break;
		case VIF1_TOPS: return "VIF1_TOPS"; break;
		case VIF1_ITOP: return "VIF1_ITOP"; break;
		case VIF1_TOP: return "VIF1_TOP"; break;
		case VIF1_ROW0: return "VIF1_ROW0"; break;
		case VIF1_ROW1: return "VIF1_ROW1"; break;
		case VIF1_ROW2: return "VIF1_ROW2"; break;
		case VIF1_ROW3: return "VIF1_ROW3"; break;
		case VIF1_COL0: return "VIF1_COL0"; break;
		case VIF1_COL1: return "VIF1_COL1"; break;
		case VIF1_COL2: return "VIF1_COL2"; break;
		case VIF1_COL3: return "VIF1_COL3"; break;
		case VIF0_FIFO: return "VIF0_FIFO"; break;
		case VIF1_FIFO: return "VIF1_FIFO"; break;
		case GIF_FIFO: return "GIF_FIFO"; break;
		case IPUout_FIFO: return "IPUout_FIFO"; break;
		case IPUin_FIFO: return "IPUin_FIFO"; break;
		case D0_CHCR: return "D0_CHCR"; break;
		case D0_MADR: return "D0_MADR"; break;
		case D0_QWC: return "D0_QWC"; break;
		case D0_TADR: return "D0_TADR"; break;
		case D0_ASR0: return "D0_ASR0"; break;
		case D0_ASR1: return "D0_ASR1"; break;
		//case VIF0_CHCR: return "VIF0_CHCR"; break;
		//case VIF0_MADR: return "VIF0_MADR"; break;
		//case VIF0_QWC: return "VIF0_QWC"; break;
		//case VIF0_TADR: return "VIF0_TADR"; break;
		//case VIF0_ASR0: return "VIF0_ASR0"; break;
		//case VIF0_ASR1: return "VIF0_ASR1"; break;
		case D1_CHCR: return "D1_CHCR"; break;
		case D1_MADR: return "D1_MADR"; break;
		case D1_QWC: return "D1_QWC"; break;
		case D1_TADR: return "D1_TADR"; break;
		case D1_ASR0: return "D1_ASR0"; break;
		case D1_ASR1: return "D1_ASR1"; break;
		//case VIF1_CHCR: return "VIF1_CHCR"; break;
		//case VIF1_MADR: return "VIF1_MADR"; break;
		//case VIF1_QWC: return "VIF1_QWC"; break;
		//case VIF1_TADR: return "VIF1_TADR"; break;
		//case VIF1_ASR0: return "VIF1_ASR0"; break;
		//case VIF1_ASR1: return "VIF1_ASR1"; break;
		case D2_CHCR: return "D2_CHCR"; break;
		case D2_MADR: return "D2_MADR"; break;
		case D2_QWC: return "D2_QWC"; break;
		case D2_TADR: return "D2_TADR"; break;
		case D2_ASR0: return "D2_ASR0"; break;
		case D2_ASR1: return "D2_ASR1"; break;
		//case GIF_CHCR: return "GIF_CHCR"; break;
		//case GIF_MADR: return "GIF_MADR"; break;
		//case GIF_QWC: return "GIF_QWC"; break;
		//case GIF_TADR: return "GIF_TADR"; break;
		//case GIF_ASR0: return "GIF_ASR0"; break;
		//case GIF_ASR1: return "GIF_ASR1"; break;
		case D3_CHCR: return "D3_CHCR"; break;
		case D3_MADR: return "D3_MADR"; break;
		case D3_QWC: return "D3_QWC"; break;
		//case fromIPU_CHCR: return "fromIPU_CHCR"; break;
		//case fromIPU_MADR: return "fromIPU_MADR"; break;
		//case fromIPU_QWC: return "fromIPU_QWC"; break;
		case D4_CHCR: return "D4_CHCR"; break;
		case D4_MADR: return "D4_MADR"; break;
		case D4_QWC: return "D4_QWC"; break;
		case D4_TADR: return "D4_TADR"; break;
		//case toIPU_CHCR: return "toIPU_CHCR"; break;
		//case toIPU_MADR: return "toIPU_MADR"; break;
		//case toIPU_QWC: return "toIPU_QWC"; break;
		//case toIPU_TADR: return "toIPU_TADR"; break;
		case D5_CHCR: return "D5_CHCR"; break;
		case D5_MADR: return "D5_MADR"; break;
		case D5_QWC: return "D5_QWC"; break;
		//case SIF0_CHCR: return "SIF0_CHCR"; break;
		//case SIF0_MADR: return "SIF0_MADR"; break;
		//case SIF0_QWC: return "SIF0_QWC"; break;
		case D6_CHCR: return "D6_CHCR"; break;
		case D6_MADR: return "D6_MADR"; break;
		case D6_QWC: return "D6_QWC"; break;
		case D6_TADR: return "D6_TADR"; break;
		//case SIF1_CHCR: return "SIF1_CHCR"; break;
		//case SIF1_MADR: return "SIF1_MADR"; break;
		//case SIF1_QWC: return "SIF1_QWC"; break;
		//case SIF1_TADR: return "SIF1_TADR"; break;
		case D7_CHCR: return "D7_CHCR"; break;
		case D7_MADR: return "D7_MADR"; break;
		case D7_QWC: return "D7_QWC"; break;
		//case SIF2_CHCR: return "SIF2_CHCR"; break;
		//case SIF2_MADR: return "SIF2_MADR"; break;
		//case SIF2_QWC: return "SIF2_QWC"; break;
		case D8_CHCR: return "D8_CHCR"; break;
		case D8_MADR: return "D8_MADR"; break;
		case D8_QWC: return "D8_QWC"; break;
		case D8_SADR: return "D8_SADR"; break;
		//case fromSPR_CHCR: return "fromSPR_CHCR"; break;
		//case fromSPR_MADR: return "fromSPR_MADR"; break;
		//case fromSPR_QWC: return "fromSPR_QWC"; break;
		//case fromSPR_SADR: return "fromSPR_SADR"; break;
		case D9_CHCR: return "D9_CHCR"; break;
		case D9_MADR: return "D9_MADR"; break;
		case D9_QWC: return "D9_QWC"; break;
		case D9_TADR: return "D9_TADR"; break;
		case D9_SADR: return "D9_SADR"; break;
		//case toSPR_CHCR: return "toSPR_CHCR"; break;
		//case toSPR_MADR: return "toSPR_MADR"; break;
		//case toSPR_QWC: return "toSPR_QWC"; break;
		//case toSPR_TADR: return "toSPR_TADR"; break;
		//case toSPR_SADR: return "toSPR_SADR"; break;
		case DMAC_CTRL: return "DMAC_CTRL"; break;
		case DMAC_STAT: return "DMAC_STAT"; break;
		case DMAC_PCR: return "DMAC_PCR"; break;
		case DMAC_SQWC: return "DMAC_SQWC"; break;
		case DMAC_RBSR: return "DMAC_RBSR"; break;
		case DMAC_RBOR: return "DMAC_RBOR"; break;
		case DMAC_STADR: return "DMAC_STADR"; break;
		case DMAC_FAKESTAT: return "DMAC_FAKESTAT"; break;
		case INTC_STAT: return "INTC_STAT"; break;
		case INTC_MASK: return "INTC_MASK"; break;
		case SIO_LCR: return "SIO_LCR"; break;
		case SIO_LSR: return "SIO_LSR"; break;
		case SIO_IER: return "SIO_IER"; break;
		case SIO_ISR: return "SIO_ISR"; break;
		case SIO_FCR: return "SIO_FCR"; break;
		case SIO_BGR: return "SIO_BGR"; break;
		case SIO_TXFIFO: return "SIO_TXFIFO"; break;
		case SIO_RXFIFO: return "SIO_RXFIFO"; break;
		case SBUS_F200: return "SBUS_F200"; break;
		case SBUS_F210: return "SBUS_F210"; break;
		case SBUS_F220: return "SBUS_F220"; break;
		case SBUS_F230: return "SBUS_F230"; break;
		case SBUS_F240: return "SBUS_F240"; break;
		case SBUS_F250: return "SBUS_F250"; break;
		case SBUS_F260: return "SBUS_F260"; break;
		case SBUS_F300: return "SBUS_F300"; break;
		case SBUS_F380: return "SBUS_F380"; break;
		case MCH_RICM: return "MCH_RICM"; break;
		case MCH_DRD: return "MCH_DRD"; break;
		case DMAC_ENABLER: return "DMAC_ENABLER"; break;
		case DMAC_ENABLEW: return "DMAC_ENABLEW"; break;
		default: return "invalid reg";
		}

	return "invalid reg";
}


static void *readwrite(struct ps2* ps2, u32 addr) {
	void *host = NULL;
	u32 virtaddr = addr;
	if (addr >= 0x80000000 && addr < 0xa0000000) {
		addr -= 0x80000000;
	} else if (addr >= 0xa0000000 && addr < 0xc0000000) {
		addr -= 0xa0000000;
	} else if (addr >= 0xc0000000 && addr < 0xe0000000) {
		addr -= 0xc0000000;
	} else if (addr >= 0xe0000000){
		die("addr > 0xe0000000");
	}
	if (addr >= EE_RAM_START && addr < EE_RAM_END) {
		host = &ps2->ee.ram[addr - EE_RAM_START];
	} else if (addr >= EE_RAM_UNCACHED_START && addr < EE_RAM_UNCACHED_END) {
		host = &ps2->ee.ram[addr - EE_RAM_UNCACHED_START];
	} else if (addr >= EE_RAM_ACCELERATED_START && addr < EE_RAM_ACCELERATED_END) {
		host = &ps2->ee.ram[addr - EE_RAM_ACCELERATED_START];
	} else if (addr >= EE_REGS_START && addr < EE_REGS_END) {
		printf("%s\n",regtostr(addr));
		host = &ps2->ee.regs[addr - EE_REGS_START];
	} else if (addr >= VU_REGS_START && addr < VU_REGS_END) {
		return 0;
	} else if (addr >= GS_REGS_START && addr < GS_REGS_END) {
		return 0;
	} else if (addr >= BIOS_START && addr < BIOS_END) {
		host = &ps2->ee.bios[addr - BIOS_START];
	} else if (addr >= EE_SCRATCHPAD_START && addr < EE_SCRATCHPAD_END) {
		host = &ps2->ee.scratch[addr - EE_SCRATCHPAD_START];
	} else if (addr >= EE_IOP_RAM_START && addr < EE_IOP_RAM_END) {
		host = &ps2->iop.ram[addr - EE_IOP_RAM_START];
	} else {
		die("unhandled addr virt %08x phys %08x",virtaddr,addr);
	}
	assert(addr != 0xb000f500);
	assert(host != NULL);
	return host;
}

static void exception() {}

static u8 memread8(struct ps2* ps2, u32 addr) {
	u8* a = readwrite(ps2,addr);
	return *a ;
}

static u16 memread16(struct ps2* ps2, u32 addr) {
	u16* a = readwrite(ps2,addr);
	return *a;
}

u32 memread32(struct ps2* ps2, u32 addr) {
	u32* a = readwrite(ps2,addr);
	return *a;
}

static u64 memread64(struct ps2* ps2, u32 addr) {
	u64* a = readwrite(ps2,addr);
	return *a;
}

static u128 memread128(struct ps2* ps2, u32 addr) {
	u128 *a = readwrite(ps2,addr);
	return *a;
}

static void memwrite8(struct ps2* ps2, u32 addr, u8 data) {
	u8* a = readwrite(ps2,addr);
	*a = data;
}

static void memwrite16(struct ps2* ps2, u32 addr, u16 data) {
	u16* a = readwrite(ps2,addr);
	*a = data;
}

static void memwrite32(struct ps2* ps2, u32 addr, u32 data) {
	u32* a = readwrite(ps2,addr);
	*a = data;
}

static void memwrite64(struct ps2* ps2, u32 addr, u64 data) {
	u64* a = readwrite(ps2,addr);
	*a = data;
}

static void memwrite128(struct ps2* ps2, u32 addr, u64 data) {
	u128 * a = readwrite(ps2,addr);
	*a = data;
}

//shamelessly stolen from pcsx2
static const u64 LDL_MASK[8] =
{	0x00ffffffffffffffULL, 0x0000ffffffffffffULL, 0x000000ffffffffffULL, 0x00000000ffffffffULL,
	0x0000000000ffffffULL, 0x000000000000ffffULL, 0x00000000000000ffULL, 0x0000000000000000ULL
};
static const u64 LDR_MASK[8] =
{	0x0000000000000000ULL, 0xff00000000000000ULL, 0xffff000000000000ULL, 0xffffff0000000000ULL,
	0xffffffff00000000ULL, 0xffffffffff000000ULL, 0xffffffffffff0000ULL, 0xffffffffffffff00ULL
};
static const u32 LWL_MASK[4] = { 0xffffff, 0x0000ffff, 0x000000ff, 0x00000000 };
static const u32 LWR_MASK[4] = { 0x000000, 0xff000000, 0xffff0000, 0xffffff00 };
static const u8 LWL_SHIFT[4] = { 24, 16, 8, 0 };
static const u8 LWR_SHIFT[4] = { 0, 8, 16, 24 };

static const u8 LDR_SHIFT[8] = { 0, 8, 16, 24, 32, 40, 48, 56 };
static const u8 LDL_SHIFT[8] = { 56, 48, 40, 32, 24, 16, 8, 0 };

void interpret(struct ps2* ps2, u32 opcode, bool isps2, bool disasm) {
	char dest [1024];
	struct mips* mips = isps2 ? &ps2->ee : &ps2->iop;
	s32 ress32;
	s64 ress64;
	u32 resu32;
	u64 resu64;

#define rs32s mips->gpr.raw[((opcode >> 21) & 0x1f)]._s32[0]
#define rt32s mips->gpr.raw[((opcode >> 16) & 0x1f)]._s32[0]
#define rd32s mips->gpr.raw[((opcode >> 11) & 0x1f)]._s32[0]
#define rs32u mips->gpr.raw[((opcode >> 21) & 0x1f)]._u32[0]
#define rt32u mips->gpr.raw[((opcode >> 16) & 0x1f)]._u32[0]
#define rd32u mips->gpr.raw[((opcode >> 11) & 0x1f)]._u32[0]

#define rs64s mips->gpr.raw[((opcode >> 21) & 0x1f)]._s64[0]
#define rt64s mips->gpr.raw[((opcode >> 16) & 0x1f)]._s64[0]
#define rd64s mips->gpr.raw[((opcode >> 11) & 0x1f)]._s64[0]
#define rs64u mips->gpr.raw[((opcode >> 21) & 0x1f)]._u64[0]
#define rt64u mips->gpr.raw[((opcode >> 16) & 0x1f)]._u64[0]
#define rd64u mips->gpr.raw[((opcode >> 11) & 0x1f)]._u64[0]

#define rs128s mips->gpr.raw[((opcode >> 21) & 0x1f)]._s128[0]
#define rt128s mips->gpr.raw[((opcode >> 16) & 0x1f)]._s128[0]
#define rd128s mips->gpr.raw[((opcode >> 11) & 0x1f)]._s128[0]


#define rsi(dt, i) mips->gpr.raw[((opcode >> 21) & 0x1f)]._##dt[i]
#define rti(dt, i) mips->gpr.raw[((opcode >> 16) & 0x1f)]._##dt[i]
#define rdi(dt, i) mips->gpr.raw[((opcode >> 11) & 0x1f)]._##dt[i]

#define loi(dt, i) mips->lo._##dt[i]
#define hii(dt, i) mips->hi._##dt[i]

#define base32s mips->gpr.raw[((opcode >> 21) & 0x1f)]._s32[0]

#define lo32s mips->lo._s32[0]
#define hi32s mips->hi._s32[0]
#define lo32u mips->lo._u32[0]
#define hi32u mips->hi._u32[0]
#define lo64s mips->lo._s64[0]
#define hi64s mips->hi._s64[0]
#define lo64u mips->lo._u64[0]
#define hi64u mips->hi._u64[0]

#define lo128s mips->lo._s128[0]
#define hi128s mips->hi._s128[0]


#define pc32s mips->pc._s32[0]
#define pc32u mips->pc._u32[0]


#define ra32s mips->gpr.ra._s32[0]
#define ra32u mips->gpr.ra._u32[0]
#define ra64s mips->gpr.ra._s64[0]
#define ra64u mips->gpr.ra._u64[0]

#define branch(addr)  do{mips->_in_branch_delay = true; mips->_pc_latch._u32[0] = addr;}while(0)
#define exception(code) do{printf("exception %d\n", code);}while(0)

#define dis_rd_rs_rt(instr) if(disasm)sprintf(dest, "%s %s, %s, %s", instr, reg_names[rd], reg_names[rs], reg_names[rt]);
#define dis_rt_rs_simm16(instr) if(disasm)sprintf(dest, "%s %s, %s, %d", instr, reg_names[rt], reg_names[rs], simm16);
#define dis_rt_rs_uimm16(instr) if(disasm)sprintf(dest, "%s %s, 0x%04x", instr, reg_names[rt], uimm16);
#define dis_rs_rt_offset(instr) if(disasm)sprintf(dest, "%s %s, %s, 0x%08x", instr, reg_names[rs], reg_names[rt], (s32)(s16)opcode << 2 + 4);
#define dis_rs_offset(instr) if(disasm)sprintf(dest, "%s %s, 0x%08x", instr, reg_names[rs], (s32)(s16)opcode << 2 + 4);
#define dis_rs_rt(instr) if(disasm)sprintf(dest, "%s %s, %s", instr, reg_names[rs], reg_names[rt]);
#define dis_rd_rt_sa(instr) if(disasm)sprintf(dest, "%s %s, %s, %d", instr, reg_names[rd], reg_names[rt], sa);
#define dis_rd_rt_rs(instr) if(disasm)sprintf(dest, "%s %s, %s, %s", instr, reg_names[rd], reg_names[rt], reg_names[rs]);
#define dis_rt_offset_base(instr) if(disasm)sprintf(dest, "%s %s, %d(%s)", instr, reg_names[rt], simm16, reg_names[rs]);
#define dis_ft_offset_base(instr) if(disasm)sprintf(dest, "%s %s, %d(%s)", instr, fpu_reg_names[rd], simm16, reg_names[rs]);
#define dis_rt_immediate(instr) if(disasm)sprintf(dest, "%s %s, 0x%04x", instr, reg_names[rt], uimm16)
#define dis_rd(instr) if(disasm)sprintf(dest, "%s %s ", instr, reg_names[rd])
#define dis_rs(instr) if(disasm)sprintf(dest, "%s %s ", instr, reg_names[rs])
#define dis_offset(instr) if(disasm)sprintf(dest, "%s 0x%08x", instr, (s32)(s16)opcode << 2 + 4)
#define dis_rt_rd(instr) if(disasm)sprintf(dest, "%s %s, %s", instr, reg_names[rt], reg_names[rd])
#define dis_rd_rt(instr) if(disasm)sprintf(dest, "%s %s, %s", instr, reg_names[rd], reg_names[rt])
#define dis_rd_rs(instr) if(disasm)sprintf(dest, "%s %s, %s", instr, reg_names[rd], reg_names[rs])
#define dis_jump(instr) if(disasm)sprintf(dest, "%s %08x",instr, jumptarget);
#define dis_(instr) if(disasm)sprintf(dest, "%s",instr);

#define illegal() dis_("illegal")

	auto op = opcode >> 26;
	auto funct6 = opcode & 0x3f;
	auto sa = opcode >> 6 & 0x1f;
	auto rt = opcode >> 16 & 0x1f;
	auto rs = opcode >> 21 & 0x1f;
	auto rd = opcode >> 11 & 0x1f;
	auto simm16 = (s16)(u16) opcode;
	auto uimm16 = (u16) opcode;
	auto branchtarget = (s32)(s16)opcode << 2 + 4 + pc32s;
	auto jumptarget = (opcode << 2 & 0x0fffffff) | (pc32u & 0xf0000000);
	auto base = rs;

	switch(op) {
		case 0: switch(funct6) {
			/*special*/
			case 0: /*sll*/ rd64s = rt32s << sa; dis_rd_rt_sa("sll"); break;
			case 1: /*illegal*/ illegal(); break;
			case 2: /*srl*/ rd64s = (s32)(rt32u >> sa); dis_rd_rt_sa("srl"); break;
			case 3: /*sra*/ rd64s = rs32s >> sa; dis_rd_rt_sa("sra"); break;
			case 4: /*sllv*/ rd64s = rt32s << rs32s & 0x1f; dis_rd_rs_rt("sllv"); break;
			case 5: /*illegal*/ illegal(); break;
			case 6: /*srlv*/ rd64s = (s32)(rt32u >> rs32s & 0x1f); dis_rd_rs_rt("srlv");break;
			case 7: /*srav*/ rd64s = rt32s >> rs32s & 0x1f; dis_rd_rs_rt("srav"); break;
			case 8: /*jr*/ branch(rs32u); dis_rs("jr"); break;
			case 9: /*jalr*/ rd64u = pc32u + 8; branch(jumptarget); dis_rd_rs("jalr"); break;
			case 10: /*movz*/ rd64u = rt64u == 0 ? rs64u : rd64u; dis_rd_rs_rt("movz"); break;
			case 11: /*movn*/ rd64u = rt64u != 0 ? rs64u : rd64u; dis_rd_rs_rt("movn"); break;
			case 12: /*syscall*/ exception(EXCCODE_SYSCALL); dis_("syscall"); break;
			case 13: /*break*/ exception(EXCCODE_BREAK); dis_("break"); break;
			case 14: /*illegal*/ illegal(); break;
			case 15: /*sync*/ dis_("sync"); break;
			case 16: /*mfhi*/ rd64u = hi64u; dis_rd("mfhi"); break;
			case 17: /*mthi*/ hi64u = rs64u; dis_rs("mthi"); break;
			case 18: /*mflo*/ rd64u = lo64u; dis_rd("mflo"); break;
			case 19: /*mtlo*/ lo64u = rs64u; dis_rs("mtlo"); break;
			case 20: /*dsllv*/ rd64u = rt64u << rs64u & 0x3f; dis_rd_rs_rt("dsllv"); break;
			case 21: /*illegal*/ illegal(); break;
			case 22: /*dsrlv*/ rd64u = rt64u >> rs64u & 0x3f; dis_rd_rs_rt("dsrlv"); break;
			case 23: /*dsrav*/ rd64s = rt64s >> rs64s & 0x3f; dis_rd_rs_rt("dsrav"); break;
			case 24: /*mult*/ lo64s = (s32)((s64)rs32s * (s64)rt32s); hi64s = (s32)(((s64)rs32s * (s64)rt32s)>>32); rd64s = lo64s; dis_rs_rt("mult"); break;
			case 25: /*multu*/lo64s = (s32)((u64)rs32u * (u64)rt32u); hi64s = (s32)(((u64)rs32u * (u64)rt32u)>>32); rd64s = lo64s; dis_rs_rt("multu"); break;
			case 26: /*div*/  lo64s = rs32s / rt32s; hi64s = rs32s % rt32s; dis_rs_rt("div"); break;
			case 27: /*divu*/ lo64s = rs32u / rt32u; hi64s = rs32u % rt32u; dis_rs_rt("divu"); break;
			case 28: /*illegal*/ illegal(); break;
			case 29: /*illegal*/ illegal(); break;
			case 30: /*illegal*/ illegal(); break;
			case 31: /*illegal*/ illegal(); break;
			case 32: /*add*/ if (__builtin_add_overflow(rs32s, rt32s, &ress32)) {exception(EXCCODE_OVERFLOW);}else{rd64s = ress32;} dis_rd_rs_rt("add"); break;
			case 33: /*addu*/ if (!__builtin_add_overflow(rs32s, rt32s, &ress32)) {rd64s = ress32;} dis_rd_rs_rt("addu"); break;
			case 34: /*sub*/ if (__builtin_sub_overflow(rs32s, rt32s, &ress32)) {exception(EXCCODE_OVERFLOW);}else{rd64s = ress32;} dis_rd_rs_rt("sub"); break;
			case 35: /*subu*/ if (!__builtin_sub_overflow(rs32s, rt32s, &ress32)) {rd64s = ress32;} dis_rd_rs_rt("subu"); break;
			case 36: /*and*/ rd64u = rs64u & rt64u; dis_rd_rs_rt("and"); break;
			case 37: /*or*/ rd64u = rs64u | rt64u; dis_rd_rs_rt("or"); break;
			case 38: /*xor*/ rd64u = rs64u ^ rt64u; dis_rd_rs_rt("xor"); break;
			case 39: /*nor*/ rd64u = ~(rs64u | rt64u); dis_rd_rs_rt("nor"); break;
			case 40: /*mfsa*/ dis_("mfsa"); break;
			case 41: /*mtsa*/ dis_("mtsa"); break;
			case 42: /*slt*/rd64u = rs64s < rt64s ? 1 : 0; dis_rd_rs_rt("slt"); break;
			case 43: /*sltu*/ rd64u = rs64u < rt64u ? 1 : 0; dis_rd_rs_rt("sltu"); break;
			case 44: /*dadd*/ if (__builtin_add_overflow(rs64s, rt64s, &ress64)) {exception(EXCCODE_OVERFLOW);}else{rd64s = ress64;} dis_rd_rs_rt("dadd"); break;
			case 45: /*daddu*/ if (!__builtin_add_overflow(rs64s, rt64s, &ress64)) {rd64s = ress64;} dis_rd_rs_rt("daddu"); break;
			case 46: /*dsub*/ if (__builtin_sub_overflow(rs64s, rt64s, &ress64)) {exception(EXCCODE_OVERFLOW);}else{rd64s = ress64;} dis_rd_rs_rt("dsub");break;
			case 47: /*dsubu*/ if (!__builtin_sub_overflow(rs64s, rt64s, &ress64)) {rd64s = ress64;} dis_rd_rs_rt("dsubu"); break;
			case 48: /*tge*/ if(rs64s >= rt64s)exception(EXCCODE_TRAP); dis_("tge"); break;
			case 49: /*tgeu*/ if(rs64u >= rt64u)exception(EXCCODE_TRAP);dis_("tgeu"); break;
			case 50: /*tlt*/ if(rs64s < rt64s)exception(EXCCODE_TRAP);dis_("tlt"); break;
			case 51: /*tltu*/ if(rs64u < rt64u)exception(EXCCODE_TRAP);dis_("tltu"); break;
			case 52: /*teq*/ if(rs64s == rt64s)exception(EXCCODE_TRAP);dis_("teq"); break;
			case 53: /*illegal*/ illegal(); break;
			case 54: /*tne*/ if(rs64s != rt64s)exception(EXCCODE_TRAP);dis_("tne"); break;
			case 55: /*illegal*/ illegal(); break;
			case 56: /*dsll*/ rd64u = rt64u << sa; dis_rd_rt_sa("dsll"); break;
			case 57: /*illegal*/ illegal(); break;
			case 58: /*dsrl*/  rd64u = rt64u >> sa; dis_rd_rt_sa("dsrl"); break;
			case 59: /*dsra*/  rd64s = rt64s >> sa; dis_rd_rt_sa("dsra"); break;
			case 60: /*dsll32*/ rd64u = rt64u << (sa+32); dis_rd_rt_sa("dsll32"); break;
			case 61: /*illegal*/ illegal(); break;
			case 62: /*dsrl32*/  rd64u = rt64u >> sa+32; dis_rd_rt_sa("dsrl32"); break;
			case 63: /*dsra32*/ rd64s = rt64s >> sa+32; dis_rd_rt_sa("dsra32"); break;
			default: illegal(); break;
		} break;
		case 1: switch(rt) {
			/*regimm*/
			case 0: /*bltz*/ if(rs64s < 0) {branch(branchtarget);} dis_rs_offset("bltz"); break;
			case 1: /*bgez*/ if(rs64s >= 0) {branch(branchtarget);} dis_rs_offset("bgez");break;
			case 2: /*bltzl*/ if(rs64s < 0) {branch(branchtarget);}else{pc32s+=4;} dis_rs_offset("bltzl"); break;
			case 3: /*bgezl*/ if(rs64s >= 0) {branch(branchtarget);}else{pc32s+=4;} dis_rs_offset("bgezl"); break;
			case 4: /*illegal*/ illegal();break;
			case 5: /*illegal**/ illegal();break;
			case 6: /*illegal*/ illegal();break;
			case 7: /*illegal*/ illegal();break;
			case 8: /*tgei*/ if(rs64s > simm16) exception(EXCCODE_TRAP); dis_("tgei"); break;
			case 9: /*tgeiu*/ if(rs64u > uimm16) exception(EXCCODE_TRAP);dis_("tgeiu"); break;
			case 10: /*tlti*/ if(rs64s < simm16) exception(EXCCODE_TRAP);dis_("tlti"); break;
			case 11: /*tltiu*/ if(rs64u < uimm16) exception(EXCCODE_TRAP); dis_("tltiu"); break;
			case 12: /*teqi*/ if(rs64s == simm16) exception(EXCCODE_TRAP); dis_("teqi"); break;
			case 13: /*illegal*/ illegal(); break;
			case 14: /*tnei*/ if(rs64s != simm16) exception(EXCCODE_TRAP); dis_("tnei"); break;
			case 15: /*illegal*/ illegal(); break;
			case 16: /*bltzal*/ ra64s = pc32s + 8; if(rs64s < 0){ branch(branchtarget);} dis_rs_offset("bltzal");break;
			case 17: /*bgezal*/ ra64s = pc32s + 8; if(rs64s >= 0){ branch(branchtarget);} dis_rs_offset("bezal");break;
			case 18: /*bltzall*/ ra64s = pc32s + 8; if(rs64s < 0){ branch(branchtarget);}else{pc32s+=4;} dis_rs_offset("bltzall");break;
			case 19: /*bgezall*/ ra64s = pc32s + 8; if(rs64s >= 0){ branch(branchtarget);}else{pc32s+=4;} dis_rs_offset("bgezall");break;
			case 20: /*illegal*/ illegal(); break;
			case 21: /*illegal*/ illegal(); break;
			case 22: /*illegal*/ illegal(); break;
			case 23: /*illegal*/ illegal(); break;
			case 24: /*mtsab*/ mips->barrelshift._u32[0] = ((rs32u ^ uimm16)&0xf)*8; dis_("mtsab"); break;
			case 25: /*mtsah*/ mips->barrelshift._u32[0] = ((rs32u ^ uimm16)&0x7)*16; dis_("mtsah"); break;
			case 26: /*illegal*/ illegal(); break;
			case 27: /*illegal*/ illegal(); break;
			case 28: /*illegal*/ illegal(); break;
			case 29: /*illegal*/ illegal(); break;
			case 30: /*illegal*/ illegal(); break;
			case 31: /*illegal*/ illegal(); break;
			default: illegal(); break;
		} break;
		case 2: /*j*/ branch(jumptarget); dis_jump("j"); break;
		case 3: /*jal*/ ra64s = pc32s + 8; branch(jumptarget); dis_jump("jal"); break;
		case 4: /*beq*/ if(rs64s == rt64s) {branch(branchtarget);} dis_rs_rt_offset("beq"); break;
		case 5: /*bne*/ if(rs64s != rt64s) {branch(branchtarget);} dis_rs_rt_offset("bne"); break;
		case 6: /*blez*/ if(rs64s <= 0) {branch(branchtarget);} dis_rs_offset("blez"); break;
		case 7: /*bgtz*/ if(rs64s > 0) {branch(branchtarget);} dis_rs_offset("bgtz"); break;
		case 8: /*addi*/ if (__builtin_add_overflow(rs32s, simm16, &ress32)) {exception(EXCCODE_OVERFLOW);}else{rd64s = ress32;} dis_rd_rs_rt("addi");break;
		case 9: /*addiu*/  if (!__builtin_add_overflow(rs32s, simm16, &ress32)) {rd64s = ress32;} dis_rd_rs_rt("addiu"); break;
		case 10: /*slti*/ rt64u = rs64s < simm16 ? 1 : 0; dis_rt_rs_simm16("slti"); break;
		case 11: /*sltiu*/  rt64u = rs64u < (u64)(s64)simm16 ? 1 : 0;  dis_rt_rs_uimm16("sltiu")break;
		case 12: /*andi*/ rt64u = rs64u & uimm16; dis_rt_rs_uimm16("andi"); break;
		case 13: /*ori*/ rt64u = rs64u | uimm16;  dis_rt_rs_uimm16("ori");  break;
		case 14: /*xori*/ rt64u = rs64u ^ uimm16;  dis_rt_rs_uimm16("xori"); break;
		case 15: /*lui*/ rt64s = (s32)simm16 << 16; dis_rt_immediate("lui"); break;
		case 16: switch(rs) {
			/*cop0*/
			case 0: /*mf0*/ switch(opcode & 0x7ff) {
				case 0: /*mfc0 */ rt64s = mips->cop0.raw[rd]._s32[0]; dis_rt_rd("mfc0"); break;
				default: dis_("move from debug registers"); break;
			} break;
			case 4: switch(opcode & 0x7ff) {
				case 0: /*mtc0*/ mips->cop0.raw[rd]._s32[0] = rt32s; dis_rt_rd("mtc0"); break;
				default: dis_("move to debug registers"); break;
			} break;
			case 8: switch(rt) {
				/*bc0*/
				case 0: /*bc0f*/ dis_("bc0f"); break;
				case 1: /*bc0t*/ dis_("bc0t"); break;
				case 2: /*bc0fl*/ dis_("bc0fl"); break;
				case 3: /*bc0tl*/ dis_("bc0tl"); break;
				default: illegal(); break;
			} break;
			case 16: switch(funct6) {
				/*c0*/
				case 1: /*tlbr*/ dis_("tlbr"); break;
				case 2: /*tlbwi*/ dis_("tlbwi"); break;
				case 6: /*tlbwr*/ dis_("tlbwr"); break;
				case 8: /*tlbp*/ dis_("tlbp"); break;
				case 24: /*eret*/ dis_("eret"); break;
				case 58: /*ei*/ dis_("ei"); break;
				case 59: /*di*/ dis_("di"); break;
				default: illegal(); break;
			} break;
			default: illegal(); break;
		} break;
		case 17: switch(rs) {
			/*cop1*/
			case 0: /*mfc1*/ dis_("mfc1"); break;
			case 2: /*cfc1*/ dis_("cfc1"); break;
			case 4: /*mtc1*/ dis_("mtc1"); break;
			case 6: /*ctc1*/ dis_("ctc1"); break;
			case 8: switch(rt) {
				/*bc1*/
				case 0: /*bc1f*/ dis_("bc1f"); break;
				case 1: /*bc1t*/ dis_("bc1t"); break;
				case 2: /*bc1fl*/ dis_("bc1fl"); break;
				case 3: /*bc1tl*/ dis_("bc1tl"); break;
				default: /*illegal*/ illegal(); break;
			} break;
			case 16: switch(funct6) {
				/*S*/
				case 0: /*add*/ dis_("add.s"); break;
				case 1: /*sub*/ dis_("sub.s"); break;
				case 2: /*mul*/ dis_("mul.s"); break;
				case 3: /*div*/ dis_("div.s"); break;
				case 4: /*sqrt*/ dis_("sqrt.s"); break;
				case 5: /*abs*/ dis_("abs.s"); break;
				case 6: /*mov*/ dis_("mov.s"); break;
				case 7: /*neg*/ dis_("neg.s"); break;
				case 22: /*rsqrt*/ dis_("rsqrt.s"); break;
				case 24: /*adda*/ dis_("adda.s"); break;
				case 25: /*suba*/ dis_("suba.s"); break;
				case 26: /*mula*/ dis_("mula.s"); break;
				case 27: /*illegal*/ illegal(); break;
				case 28: /*madd*/ dis_("madd.s"); break;
				case 29: /*msub*/ dis_("msub.s"); break;
				case 30: /*madda*/ dis_("madda.s"); break;
				case 31: /*msuba*/ dis_("msuba.s"); break;
				case 36: /*cvtw*/ dis_("cvtw"); break;
				case 40: /*max*/ dis_("max.s"); break;
				case 41: /*min*/ dis_("min.s"); break;
				case 48: /*c.f*/ dis_("c.f.s"); break;
				case 50: /*c.eq*/ dis_("c.eq.s"); break;
				case 52: /*c.lt*/ dis_("d.lt.s"); break;
				case 54: /*c.le*/ dis_("c.le.s"); break;
				default: /*illegal*/ illegal(); break;
			} break;
			case 20: switch(funct6) {
				/*W*/
				case 32: /*cvts*/ dis_("cvts"); break;
				default: /*illegal*/ illegal(); break;
			} break;
			default: /*illegal*/ illegal(); break;
		} break;
		case 18: switch(rs) {
			/*cop2*/
			default: /*illegal*/ dis_("cop2"); break;
		} break;
		case 19: /*illegal*/ illegal(); break;
		case 20: /*beql*/ if(rs64s == rt64s) {branch(branchtarget);} else {pc32s +=4;} dis_rs_rt_offset("beql"); break;
		case 21: /*bnel*/ if(rs64s != rt64s) {branch(branchtarget);} else {pc32s += 4;} dis_rs_rt_offset("bnel"); break;
		case 22: /*blezl*/ if(rs64s <= 0) {branch(branchtarget);} else {pc32s+=4;} dis_rs_offset("blezl"); break;
		case 23: /*bgtzl*/ if(rs64s > 0) {branch(branchtarget);} else {pc32s+=4;} dis_rs_offset("bgtzl"); break;
		case 24: /*daddi*/ if (__builtin_add_overflow(rs64s, simm16, &ress64)) {exception(EXCCODE_OVERFLOW);}else{rd64s = ress64;} dis_rd_rs_rt("daddi"); break;
		case 25: /*daddiu*/ if (!__builtin_add_overflow(rs64s, simm16, &ress64)) {rd64s = ress64;} dis_rd_rs_rt("daddiu"); break;
		case 26: /*ldl*/ dis_("ldl"); break;
		case 27: /*ldr*/ dis_("ldr");break;
		case 28: switch(funct6) {
			case 0: /*madd*/ dis_rd_rs_rt("madd"); break;
			case 1: /*maddu*/ dis_rd_rs_rt("maddu"); break;
			case 4: /*plzcw*/ dis_rd_rs("plzcw"); break;
			case 8: switch(sa) {
				/*mmi0*/
				case 0: /*paddw*/ for(int i = 0; i < 4; i++) rdi(s32,i) = rsi(s32,i) + rti(s32,i); dis_rd_rs_rt("paddw"); break;
				case 1: /*psubw*/ for(int i = 0; i < 4; i++) rdi(s32,i) = rsi(s32,i) - rti(s32,i); dis_rd_rs_rt("psubw"); break;
				case 2: /*pcgtw*/ for(int i = 0; i < 4; i++) rdi(s32,i) = rsi(s32,i) > rti(s32,i) ? -1 : 0; dis_rd_rs_rt("pcgtw"); break;
				case 3: /*pmaxw*/ for(int i = 0; i < 4; i++) rdi(s32,i) = rsi(s32,i) > rti(s32,i) ? rsi(s32,i) : rti(s32,i); dis_rd_rs_rt("pmaxw"); break;
				case 4: /*paddh*/ for(int i = 0; i < 8; i++) rdi(s16,i) = rsi(s16,i) + rti(s16,i); dis_rd_rs_rt("paddh"); break;
				case 5: /*psubh*/ for(int i = 0; i < 8; i++) rdi(s16,i) = rsi(s16,i) - rti(s16,i); dis_rd_rs_rt("psubh"); break;
				case 6: /*pcgth*/ for(int i = 0; i < 8; i++) rdi(s16,i) = rsi(s16,i) > rti(s16,i) ? -1 : 0; dis_rd_rs_rt("pcgth"); break;
				case 7: /*pmaxh*/ for(int i = 0; i < 8; i++) rdi(s16,i) = rsi(s16,i) > rti(s16,i) ? rsi(s16,i) : rti(s16,i); dis_rd_rs_rt("pmaxh"); break;
				case 8: /*paddb*/ for(int i = 0; i < 16; i++) rdi(s8,i) = rsi(s8,i) + rti(s8,i); dis_rd_rs_rt("paddb"); break;
				case 9: /*psubb*/ for(int i = 0; i < 16; i++) rdi(s8,i) = rsi(s8,i) - rti(s8,i); dis_rd_rs_rt("psubb"); break;
				case 10: /*pcgtb*/ for(int i = 0; i < 16; i++) rdi(s8,i) = rsi(s8,i) > rti(s8,i) ? -1 : 0; dis_rd_rs_rt("pcgtb"); break;
				case 11: /*illegal*/ illegal(); break;
				case 12: /*illegal*/ illegal(); break;
				case 13: /*illegal*/ illegal(); break;
				case 14: /*illegal*/ illegal(); break;
				case 15: /*illegal*/ illegal(); break;
				case 16: /*paddsw*/ for(int i = 0; i < 4; i++) rdi(s32,i) = satadd(rsi(s32,i), rti(s32,i)); dis_rd_rs_rt("paddsw"); break;
				case 17: /*psubsw*/ for(int i = 0; i < 4; i++) rdi(s32,i) = satsub(rsi(s32,i), rti(s32,i)); dis_rd_rs_rt("psubsw"); break;
				case 18: /*pextlw*/ for(int i = 0; i < 4; i+=2){rdi(s32,i) = rti(s32,i/2); rdi(s32,i+1) = rsi(s32,i);} dis_rd_rs_rt("pextlw"); break;
				case 19: /*ppacw*/ rdi(s32,0) = rti(s32,0); rdi(s32,1) = rti(s32,2); rdi(s32,2) = rsi(s32,0); rdi(s32,3) = rsi(s32,2); dis_rd_rs_rt("ppacw"); break;
				case 20: /*paddsh*/ for(int i = 0; i < 8; i++) rdi(s16,i) = satadd(rsi(s16,i), rti(s16,i)); dis_rd_rs_rt("paddsh"); break;
				case 21: /*psubsh*/ for(int i = 0; i < 8; i++) rdi(s16,i) = satsub(rsi(s16,i), rti(s16,i)); dis_rd_rs_rt("psubh"); break;
				case 22: /*pextlh*/ for(int i = 0; i < 8; i+=2){rdi(s16,i) = rti(s16,i/2); rdi(s16,i+1) = rsi(s16,i);} dis_rd_rs_rt("pextlh"); break;
				case 23: /*ppach*/ dis_rd_rs_rt("ppach"); break;
				case 24: /*paddsb*/ for(int i = 0; i < 16; i++) rdi(s8,i) = satadd(rsi(s8,i), rti(s8,i)); dis_rd_rs_rt("paddsb"); break;
				case 25: /*psubsb*/ for(int i = 0; i < 16; i++) rdi(s8,i) = satsub(rsi(s8,i), rti(s8,i)); dis_rd_rs_rt("psubsb"); break;
				case 26: /*pextlb*/ dis_rd_rs_rt("pextlb"); break;
				case 27: /*ppacb*/ dis_rd_rs_rt("ppacb"); break;
				case 28: /*illegal*/ illegal(); break;
				case 29: /*illegal*/ illegal(); break;
				case 30: /*pext5*/ dis_rd_rs_rt("pext5"); break;
				case 31: /*ppac5*/ dis_rd_rs_rt("ppac5"); break;
				default: illegal(); break;
			} break;
			case 9: switch(sa) {
				/*mmi2*/
				case 0: /*pmaddw*/ dis_rd_rs_rt("pmaddw"); break;
				case 1: /*illegal*/ break;
				case 2: /*psllvw*/ rdi(s64,0) = (s64)rti(s32,0) << rsi(s32,0)&0x1f; rdi(s64,1) = (s64)rti(s32,2) << rsi(s32,2)&0x1f; dis_rd_rs_rt("psllvw"); break;
				case 3: /*psrlvw*/ rdi(s64,0) = (s64)(rti(u32,0) >> rsi(s32,0)&0x1f); rdi(s64,1) = (s64)(rti(u32,2) >> rsi(s32,2)&0x1f); dis_rd_rs_rt("psrlvw"); break;
				case 4: /*pmsubw*/ dis_rd_rs_rt("pmsubw"); break;
				case 5: /*illegal*/ break;
				case 6: /*illegal*/ break;
				case 7: /*illegal*/ break;
				case 8: /*pmfhi*/ rd128s = hi128s; dis_rd_rs_rt("pmfhi"); break;
				case 9: /*pmflo*/ rd128s = lo128s; dis_rd_rs_rt("pmflo"); break;
				case 10: /*pinth*/ for(int i = 0; i < 8; i+=2){rdi(s16,i) = rti(s16,i/2); rdi(s16,i+1) = rsi(s16, i/2+4);} dis_rd_rs_rt("pinth"); break;
				case 11: /*illegal*/ break;
				case 12: /*pmultw*/ rdi(s64,0) = (s64)rsi(s32,0) * (s64)rdi(s32,0); loi(s64,0) = rdi(s32,0); hii(s64,0) = rdi(s32,1); rdi(s64,1) = (s64)rsi(s32,2) * (s64)rdi(s32,2); loi(s64,1) = rdi(s32,2); hii(s64,1) = rdi(s32,3);dis_rd_rs_rt("pmultw"); break;
				case 13: /*pdivw*/ loi(s64,0) = rsi(s32,0)/rti(s32,0); hii(s64,0) = rsi(s32,0)%rti(s32,0); loi(s64,1) = rsi(s32,2)/rti(s32,2); hii(s64,1) = rsi(s32,2)%rti(s32,2); dis_rd_rs_rt("pdivw"); break;
				case 14: /*pcpyld*/ rdi(s64,0) = rti(s64,0); rdi(s64,1) = rsi(s64,0); dis_rd_rs_rt("pcpyld"); break;
				case 15: /*illegal*/ break;
				case 16: /*pmaddh*/ dis_rd_rs_rt("pmaddh"); break;
				case 17: /*phmadh*/ dis_rd_rs_rt("phmadh"); break;
				case 18: /*pand*/ rd128s = rs128s & rt128s; dis_rd_rs_rt("pand"); break;
				case 19: /*pxor*/ rd128s = rs128s ^ rt128s; dis_rd_rs_rt("por"); break;
				case 20: /*pmsubh*/ dis_rd_rs_rt("pmsubh"); break;
				case 21: /*phmsbh*/ dis_rd_rs_rt("phmsbh"); break;
				case 22: /*illegal*/ break;
				case 23: /*illegal*/ break;
				case 24: /*illegal*/ break;
				case 25: /*illegal*/ break;
				case 26: /*pexeh*/ rdi(s16,0) = rti(s16,2); rdi(s16,1) = rti(s16,1); rdi(s16,2) = rti(s16,0); rdi(s16,3) = rti(s16,3); rdi(s16,4) = rti(s16,6); rdi(s16,5) = rti(s16,5); rdi(s16,6) = rti(s16,4); rdi(s16,7) = rti(s16,7); dis_rd_rs_rt("pexeh"); break;
				case 27: /*prevh*/ rdi(s16,0) = rti(s16,3); rdi(s16,1) = rti(s16,2); rdi(s16,2) = rti(s16,1); rdi(s16,3) = rti(s16,0); rdi(s16,4) = rti(s16,7); rdi(s16,5) = rti(s16,6); rdi(s16,6) = rti(s16,5); rdi(s16,7) = rti(s16,4);dis_rd_rs_rt("prevh"); break;
				case 28: /*pmulth*/ dis_rd_rs_rt("pmulth"); break;
				case 29: /*pdivbw*/ dis_rd_rs_rt("pdivbw"); break;
				case 30: /*pexeq*/ dis_rd_rs_rt("pexeq"); break;
				case 31: /*prot3w*/ dis_rd_rs_rt("prot3w"); break;
				default: illegal(); break;
			} break;
			case 16:/*mfhi1*/ rd64u = hii(u64,1); dis_rd("mfhi1"); break;
			case 17:/*mthi1*/ hii(u64,1) = rsi(u64,0); dis_rs("mthi1"); break;
			case 18:/*mflo1*/ rd64u = loi(u64,1); dis_rd("mflo1"); break;
			case 19:/*mtlo1*/ loi(u64,1) = rsi(u64,0); dis_rs("mtlo"); break;
			case 24:/*mult1*/ loi(s64,1) = (s32)((s64)rs32s * (s64)rt32s); hii(s64,1) = (s32)(((s64)rs32s * (s64)rt32s)>>32); rd64s = loi(s64,1); dis_rs_rt("mult1"); break;
			case 25:/*multu1*/ loi(s64,1) = (s32)((u64)rs32u * (u64)rt32u); hii(s64,1) = (s32)(((u64)rs32u * (u64)rt32u)>>32); rd64s = loi(s64,1); dis_rs_rt("multu1"); break;
			case 26:/*div1*/ loi(s64,1) = rs32s / rt32s; hii(s64,1) = rs32s % rt32s; dis_rs_rt("div1"); break;
			case 27:/*divu1*/ loi(s64,1) = rs32u / rt32u; hii(s64,1) = rs32u % rt32u; dis_rs_rt("divu1madd"); break;
			case 32:/*madd1*/ dis_rd_rs_rt("madd1"); break;
			case 33:/*maddu1*/ dis_rd_rs_rt("maddu1"); break;
			case 40: switch(sa) {
				/*mmi1*/
				case 0: /*illegal*/ illegal(); break;
				case 1: /*pabsw*/ for(int i = 0; i < 4; i++) rdi(s32,i) = satabs(rti(s32,i)); dis_rd_rt("pabsw"); break;
				case 2: /*pceqw*/ for(int i = 0; i < 4; i++) rdi(s32,i) = rsi(s32,i) == rti(s32,i) ? 0xffffffff : 0; dis_rd_rs_rt("pceqw"); break;
				case 3: /*pminw*/ for(int i = 0; i < 4; i++) rdi(s32,i) = rsi(s32,i) < rti(s32,i) ? rsi(s32,i) : rti(s32,i); dis_rd_rs_rt("pminw"); break;
				case 4: /*padsbh*/ for(int i = 0; i < 4; i++) {rdi(s16,i) = rsi(s16,i) - rti(s16,i); rdi(s16,i+4) = rsi(s16,i+4) + rti(s16,i+4);}dis_rd_rs_rt("padsbh"); break;
				case 5: /*pabsh*/ for(int i = 0; i < 8; i++) rdi(s16,i) = satabs(rti(s16,i)); dis_rd_rs_rt("pabsh"); break;
				case 6: /*pceqh*/ for(int i = 0; i < 8; i++) rdi(s16,i) = rsi(s16,i) == rti(s16,i) ? 0xffff : 0; dis_rd_rs_rt("pceqh"); break;
				case 7: /*pminh*/ for(int i = 0; i < 8; i++) rdi(s16,i) = rsi(s16,i) < rti(s16,i) ? rsi(s16,i) : rti(s16,i); dis_rd_rs_rt("pminh"); break;
				case 8: /*illegal*/ illegal(); break;
				case 9: /*illegal*/ illegal(); break;
				case 10: /*pceqb*/ for(int i = 0; i < 16; i++) rdi(s8,i) = rsi(s8,i) == rti(s8,i) ? 0xff : 0; dis_rd_rs_rt("pceqb"); break;
				case 11: /*illegal*/ illegal(); break;
				case 12: /*illegal*/ illegal(); break;
				case 13: /*illegal*/ illegal(); break;
				case 14: /*illegal*/ illegal(); break;
				case 15: /*illegal*/ illegal(); break;
				case 16: /*padduw*/ for(int i = 0; i< 4; i++)rdi(u32,i) = satadd(rsi(u32,i),rti(u32,i)); dis_rd_rs_rt("padduw"); break;
				case 17: /*psubuw*/ for(int i = 0; i< 4; i++)rdi(u32,i) = satsub(rsi(u32,i),rti(u32,i)); dis_rd_rs_rt("psubuw"); break;
				case 18: /*pextuw*/ rdi(s32,0) = rti(s32,2); rdi(s32,1) = rsi(s32,2); rdi(s32,2) = rti(s32,3); rdi(s32,3) = rsi(s32,3); dis_rd_rs_rt("pextuw");  break;
				case 19: /*illegal*/ illegal(); break;
				case 20: /*padduh*/ for(int i = 0; i< 8; i++)rdi(u16,i) = satadd(rsi(u16,i),rti(u16,i)); dis_rd_rs_rt("padduh"); break;
				case 21: /*psubuh*/ for(int i = 0; i< 8; i++)rdi(u16,i) = satsub(rsi(u16,i),rti(u16,i)); dis_rd_rs_rt("psubuh"); break;
				case 22: /*pextuh*/ for(int i = 0; i < 8; i+=2){rdi(s16,i) = rti(s16,i/2+4); rdi(s16,i+1) = rsi(s16,i/2+4);} dis_rd_rs_rt("pextuh"); break;
				case 23: /*illegal*/ illegal(); break;
				case 24: /*paddub*/ for(int i = 0; i< 16; i++)rdi(u8,i) = satadd(rsi(u8,i),rti(u8,i)); dis_rd_rs_rt("paddub"); break;
				case 25: /*psubub*/ for(int i = 0; i< 16; i++)rdi(u8,i) = satsub(rsi(u8,i),rti(u8,i)); dis_rd_rs_rt("psubub"); break;
				case 26: /*pextub*/ for(int i = 0; i < 16; i+=2){rdi(s8,i) = rti(s8,i/2+8); rdi(s16,i+1) = rsi(s8,i/2+8);} dis_rd_rs_rt("pextub"); break;
				case 27: /*qfsrv*/ dis_("qfsrv"); break;
				case 28: /*illegal*/ illegal(); break;
				case 29: /*illegal*/ illegal(); break;
				case 30: /*illegal*/ illegal(); break;
				case 31: /*illegal*/ illegal(); break;
				default: illegal(); break;
			} break;
			case 41: switch(sa) {
				/*mmi3*/
				case 0: /*pmadduw*/ dis_rd_rs_rt("pmadduw"); break;
				case 1: /*illegal*/ illegal(); break;
				case 2: /*illegal*/ illegal(); break;
				case 3: /*psravw*/ dis_rd_rs_rt("psravw"); break;
				case 4: /*illegal*/ illegal(); break;
				case 5: /*illegal*/ illegal(); break;
				case 6: /*illegal*/ illegal(); break;
				case 7: /*illegal*/ illegal(); break;
				case 8: /*pmthi*/ hi128s = rs128s; dis_rs("pmthi"); break;
				case 9: /*pmtlo*/ lo128s = rs128s; dis_rs("pmtlo"); break;
				case 10: /*pinteh*/ for(int i = 0; i < 8; i+=2){rdi(s16,i) = rti(s16,i); rdi(s16,i+1) = rsi(s16,i);} dis_rd_rs_rt("pinteh"); break;
				case 11: /*illegal*/ illegal(); break;
				case 12: /*pmultuw*/ rdi(u64,0) = (u64)rsi(u32,0) * (u64)rdi(u32,0); loi(s64,0) = rdi(s32,0); hii(s64,0) = rdi(s32,1); rdi(u64,1) = (u64)rsi(s32,2) * (u64)rdi(s32,2); loi(s64,1) = rdi(s32,2); hii(s64,1) = rdi(s32,3); dis_rd_rs_rt("pmultuw"); break;
				case 13: /*pdivuw*/ loi(s64,0) = (s32)(rsi(u32,0)/rti(u32,0)); hii(s64,0) = (s32)(rsi(u32,0)%rti(u32,0)); loi(s64,1) = (s32)(rsi(u32,2)/rti(u32,2)); hii(s64,1) = (s32)(rsi(u32,2)%rti(u32,2)); dis_rd_rs_rt("pdivuw"); break;
				case 14: /*pcpyud*/ rdi(s64,0) = rsi(s64,1); rdi(s64,1) = rti(s64,1); dis_rd_rs_rt("pcpyud"); break;
				case 15: /*illegal*/ illegal(); break;
				case 16: /*illegal*/ illegal(); break;
				case 17: /*illegal*/ illegal(); break;
				case 18: /*por*/ rd128s = rs128s | rt128s; dis_rd_rs_rt("por"); break;
				case 19: /*pnor*/ rd128s = ~(rs128s | rt128s); dis_rd_rs_rt("pnor"); break;
				case 20: /*illegal*/ illegal(); break;
				case 21: /*illegal*/ illegal(); break;
				case 22: /*illegal*/ illegal(); break;
				case 23: /*illegal*/ illegal(); break;
				case 24: /*illegal*/ illegal(); break;
				case 25: /*illegal*/ illegal(); break;
				case 26: /*pexch*/ rdi(s16,0) = rti(s16,0); rdi(s16,1) = rti(s16,2); rdi(s16,2) = rti(s16,1); rdi(s16,3) = rti(s16,3); rdi(s16,4) = rti(s16,4); rdi(s16,5) = rti(s16,6); rdi(s16,6) = rti(s16,5); rdi(s16,7) = rti(s16,7); dis_rd_rs_rt("pexch");   break;
				case 27: /*pcpyh*/ for(int i = 0; i < 4; i++){rdi(s16,i) = rti(s16,0); rdi(s16,i+4) = rti(s16,4); } dis_rd_rs_rt("pcpyh"); break;
				case 28: /*illegal*/ illegal(); break;
				case 29: /*illegal*/ illegal(); break;
				case 30: /*pexcw*/ rdi(s32,0) = rti(s32,0); rdi(s32,1) = rti(s32,2); rdi(s32,2) = rti(s32,1); rdi(s32,3) = rti(s32,3); dis_rd_rs_rt("pexcw"); break;
				case 31: /*illegal*/ illegal(); break;
				default: illegal(); break;
			} break;
			case 48: /*pmfhl*/ dis_rd_rs_rt("pmfhl"); break;
			case 49: /*pmthl*/ dis_rd_rs_rt("pmthl"); break;
			case 52: /*psllh*/ for(int i = 0; i < 8; i++) rdi(u16,i) = rti(u16,i) << (sa&0xf);dis_rd_rt_sa("psllh"); break;
			case 54: /*psrlh*/ for(int i = 0; i < 8; i++) rdi(u16,i) = rti(u16,i) >> (sa&0xf);dis_rd_rt_sa("psrlh");break;
			case 55: /*psrah*/ for(int i = 0; i < 8; i++) rdi(s16,i) = rti(s16,i) >> (sa&0xf);dis_rd_rt_sa("psrah");break;
			case 60: /*psllw*/ for(int i = 0; i < 4; i++) rdi(u32,i) = rti(u32,i) << sa; dis_rd_rt_sa("psllw");break;
			case 62: /*psrlw*/ for(int i = 0; i < 4; i++) rdi(u32,i) = rti(u32,i) >> sa; dis_rd_rt_sa("psrlw");break;
			case 63: /*psraw*/ for(int i = 0; i < 4; i++) rdi(s32,i) = rti(s32,i) >> sa; dis_rd_rt_sa("psraw");break;
			default: /*illegal*/ illegal(); break;
		} break;
		case 29: /*illegal*/ illegal(); break;
		case 30: /*lq*/ rt128s = memread128(ps2, (base32s + simm16)&~0xf); dis_rt_offset_base("lq"); break;
		case 31: /*sq*/ memwrite128(ps2, (base32s + simm16)&~0xf, rt128s); dis_rt_offset_base("sq"); break;
		case 32: /*lb*/ rt64s = (s8)memread8(ps2, base32s + simm16); dis_rt_offset_base("lb"); break;
		case 33: /*lh*/ rt64s = (s16)memread16(ps2, base32s + simm16); dis_rt_offset_base("lh"); break;
		case 34: /*lwl*/ dis_rt_offset_base("lwl"); break;
		case 35: /*lw*/ rt64s = (s32)memread16(ps2, base32s + simm16); dis_rt_offset_base("lw"); break;
		case 36: /*lbu*/ rt64u = memread8(ps2, base32s + simm16); dis_rt_offset_base("lbu"); break;
		case 37: /*lhu*/ rt64u = memread16(ps2, base32s + simm16); dis_rt_offset_base("lhu"); break;
		case 38: /*lwr*/ dis_rt_offset_base("lwr"); break;
		case 39: /*lwu*/ rt64u = memread32(ps2, base32s + simm16); dis_rt_offset_base("lwu"); break;
		case 40: /*sb*/ 	memwrite8(ps2, base32s + simm16, rt32s); dis_rt_offset_base("sb"); break;
		case 41: /*sh*/ 	memwrite16(ps2, base32s + simm16, rt32u); dis_rt_offset_base("sh"); break;
		case 42: /*swl*/ dis_rt_offset_base("swl"); break;
		case 43: /*sw*/ 	memwrite32(ps2, base32s + simm16,  rt32u); dis_rt_offset_base("sw"); break;
		case 44: /*sdl*/ dis_rt_offset_base("sdl"); break;
		case 45: /*sdr*/ dis_rt_offset_base("sdr"); break;
		case 46: /*swr*/ dis_rt_offset_base("sdr"); break;
		case 47: /*cache*/ dis_("cache"); break;
		case 48: /*illegal*/ illegal(); break;
		case 49: /*lwc1*/ dis_("lwc1"); break;
		case 50: /*illegal*/ illegal(); break;
		case 51: /*pref*/ dis_("pref"); break;
		case 52: /*illegal*/ illegal(); break;
		case 53: /*illegal*/ illegal(); break;
		case 54: /*lqc2*/ dis_("lqc2"); break;
		case 55: /*ld*/ rt64u = memread64(ps2, base32s + simm16); dis_rt_offset_base("ld"); break;
		case 56: /*illegal*/ illegal(); break;
		case 57: /*swc1*/ dis_("swc1"); break;
		case 58: /*illegal*/ illegal(); break;
		case 59: /*illegal*/ illegal(); break;
		case 60: /*illegal*/ illegal(); break;
		case 61: /*illegal*/ illegal(); break;
		case 62: /*sqc2*/ dis_("sqc2"); break;
		case 63: /*sd*/ memwrite64(ps2, base32s + simm16,  rt64u); dis_rt_offset_base("sd"); break;
		default: illegal(); break;
	}
}

void vudecode(struct ps2* ps2, u32 upperopcode, u32 loweropcode, bool interpret, bool disasm) {
	auto bc = upperopcode & 0x3;
	auto fd = upperopcode >> 6 & 0x1f;
	auto fs = upperopcode >> 11 & 0x1f;
	auto ft = upperopcode >> 16 & 0x1f;
	auto dest =  upperopcode >> 20 & 0xf;
	switch(upperopcode >> 2 & 0xf) {
		case 0x0: /*addbc*/ break;
		case 0x1: /*subbc*/break;
		case 0x2: /*maddbc*/break;
		case 0x3: /*msubbc*/break;
		case 0x4: /*maxbc*/ break;
		case 0x5: /*minibc*/break;
		case 0x6: /*mulbc*/break;
		case 0x7: switch(bc) {
			case 0: /*mulq*/break;
			case 1: /*maxi*/break;
			case 2: /*muli*/break;
			case 3: /*minii*/break;
		}break;
		case 0x8: switch(bc) {
			case 0: /*addq*/ break;
			case 1: /*madd*/ break;
			case 2: /*addi*/ break;
			case 3: /*maddi*/break;
		}break;
		case 0x9: switch(bc) {
			case 0: /*subq*/break;
			case 1: /*msubq*/break;
			case 2: /*subi*/break;
			case 3: /*msubi*/break;
		}break;
		case 0xa: switch(bc) {
			case 0: /*add*/break;
			case 1: /*madd*/break;
			case 2: /*mul*/break;
			case 3: /*max*/break;
		}break;
		case 0xb: switch(bc) {
			case 0: /*sub*/ break;
			case 1: /*msub*/break;
			case 2: /*opmsub*/break;
			case 3: /*mini*/break;
		}break;
		case 0xc: break;
		case 0xd: break;
		case 0xe: break;
		case 0xf: switch(upperopcode >> 6 & 0x1f) {
			case 0x0: /*addabc*/ break;
			case 0x1: /*subabc*/break;
			case 0x2: /*maddabc*/ break;
			case 0x3: /*msubabc*/break;
			case 0x4: switch(bc) {
				case 0: /*itof0*/ break;
				case 1: /*itof4*/ break;
				case 2: /*itof12*/ break;
				case 3: /*itof15*/ break;
			}break;
			case 0x5: switch(bc) {
				case 0: /*ftoi0*/break;
				case 1: /*ftoi4*/break;
				case 2: /*ftoi12*/break;
				case 3: /*ftoi15*/break;
			}break;
			case 0x6: /*mulabc*/break;
			case 0x7: switch(bc) {
				case 0: /*mulaq*/break;
				case 1: /*abs*/break;
				case 2: /*mulai*/break;
				case 3: /*clip*/break;
			}
			break;
			case 0x8: switch(bc) {
				case 0: /*addaq*/break;
				case 1: /*maddaq*/break;
				case 2: /*addai*/ break;
				case 3: /*maddai*/ break;
			}break;
			case 0x9: switch(bc) {
				case 0: /*subaq*/break;
				case 1: /*msubaq*/ break;
				case 2: /*subai*/break;
				case 3: /*msubai*/ break;
			}break;
			case 0xa: switch(bc) {
				case 0: /*adda*/ break;
				case 1: /*madda*/ break;
				case 2: /*mula*/break;
				case 3: break;
			}break;
			case 0xb: switch(bc) {
				case 0: /*suba*/break;
				case 1: /*msuba*/ break;
				case 2: /*opmula*/ break; /*todo: dest == 0xe ? */
				case 3: /*nop*/break;
			}break;
			case 0xc: break;
			case 0xd: break;
			case 0xe: break;
			case 0xf: break;
			case 0x10: break;
			case 0x11: break;
			case 0x12: break;
			case 0x13: break;
			case 0x14: break;
			case 0x15: break;
			case 0x16: break;
			case 0x17: break;
			case 0x18: break;
			case 0x19: break;
			case 0x1a: break;
			case 0x1b: break;
			case 0x1c: break;
			case 0x1d: break;
			case 0x1e: break;
			case 0x1f: break;
		}break;
	}
	switch(loweropcode >> 25) {
		case 0x0: /*lq*/ break;
		case 0x1: /**/ break;
		case 0x2: /**/ break;
		case 0x3: /**/ break;
		case 0x4: /*ilw*/ break;
		case 0x5: /*isw*/ break;
		case 0x6: /**/ break;
		case 0x7: /**/ break;
		case 0x8: /*iaddiu*/ break;
		case 0x9: /*isubiu*/ break;
		case 0xa: /**/ break;
		case 0xb: /**/ break;
		case 0xc: /**/ break;
		case 0xd: /**/ break;
		case 0xe: /**/ break;
		case 0xf: /**/ break;
		case 0x10: /*fceq*/ break;
		case 0x11: /*fcset*/ break;
		case 0x12: /*fcand*/ break;
		case 0x13: /*fcor*/ break;
		case 0x14: /*fseq*/ break;
		case 0x15: /*fsset*/ break;
		case 0x16: /*fsand*/ break;
		case 0x17: /*fsor*/ break;
		case 0x18: /*fmeq*/ break;
		case 0x19: /**/ break;
		case 0x1a: /*fmand*/ break;
		case 0x1b: /*fmor*/ break;
		case 0x1c: /*fcget*/ break;
		case 0x1d: /**/ break;
		case 0x1e: /**/ break;
		case 0x1f: /**/ break;
		case 0x20: /*B*/ break;
		case 0x21: /*BAL*/ break;
		case 0x22: /**/ break;
		case 0x23: /**/ break;
		case 0x24: /*jr*/ break;
		case 0x25: /*jalr*/ break;
		case 0x26: /**/ break;
		case 0x27: /**/ break;
		case 0x28: /*ibeq*/ break;
		case 0x29: /*ibne*/ break;
		case 0x2a: /**/ break;
		case 0x2b: /**/ break;
		case 0x2c: /*ibltz*/ break;
		case 0x2d: /*ibgtz*/ break;
		case 0x2e: /*iblez*/ break;
		case 0x2f: /*ibgez*/ break;
		case 0x30: /**/ break;
		case 0x31: /**/ break;
		case 0x32: /**/ break;
		case 0x33: /**/ break;
		case 0x34: /**/ break;
		case 0x35: /**/ break;
		case 0x36: /**/ break;
		case 0x37: /**/ break;
		case 0x38: /**/ break;
		case 0x39: /**/ break;
		case 0x3a: /**/ break;
		case 0x3b: /**/ break;
		case 0x3c: /**/ break;
		case 0x3d: /**/ break;
		case 0x3e: /**/ break;
		case 0x3f: /**/ break;
		case 0x40: switch(loweropcode >> 2 & 0xf) {
			case 0x0: /**/ break;
			case 0x1: /**/ break;
			case 0x2: /**/ break;
			case 0x3: /**/ break;
			case 0x4: /**/ break;
			case 0x5: /**/ break;
			case 0x6: /**/ break;
			case 0x7: /**/ break;
			case 0x8: /**/ break;
			case 0x9: /**/ break;
			case 0xa: /**/ break;
			case 0xb: /**/ break;
			case 0xc: switch(bc) {
				case 0: /*iadd*/ break;
				case 1: /**/ break;
				case 2: /*iaddi*/ break;
				case 3: /**/ break;
			} break;
			case 0xd: /**/ break;
			case 0xe: /**/ break;
			case 0xf: switch(fd) {
				case 0x0: /**/ break;
				case 0x1: /**/ break;
				case 0x2: /**/ break;
				case 0x3: /**/ break;
				case 0x4: /**/ break;
				case 0x5: /**/ break;
				case 0x6: /**/ break;
				case 0x7: /**/ break;
				case 0x8: /**/ break;
				case 0x9: /**/ break;
				case 0xa: /**/ break;
				case 0xb: /**/ break;
				case 0xc: switch(bc) {
					case 0: break;
					case 1: /*isub*/ break;
					case 2: break;
					case 3: break;
				} break;
				case 0xd: switch(bc) {
					case 0: /*iand*/ break;
					case 1: /*ior*/ break;
					case 2: /*lqd*/ break;
					case 3: /**/ break;
				} break;
				case 0xe: switch(bc) {
					case 0: /*div*/break;
					case 1: break;
					case 2: break;
					case 3: break;
				}break;
				case 0xf: switch(bc) {
					case 0: break;
					case 1: break;
					case 2: /*ilwr*/break;
					case 3: /*iswr*/break;
				} break;
				case 0x10: /**/ break;
				case 0x11: /**/ break;
				case 0x12: /**/ break;
				case 0x13: /**/ break;
				case 0x14: /**/ break;
				case 0x15: /**/ break;
				case 0x16: /**/ break;
				case 0x17: /**/ break;
				case 0x18: /**/ break;
				case 0x19: /**/ break;
				case 0x1a: /**/ break;
				case 0x1b: /**/ break;
				case 0x1c: switch(bc) {
					case 0: /*esadd*/ break;
					case 1: /*ersadd*/break;
					case 2: /*eleng*/break;
					case 3: /*erleng*/ break;
				} break;
				case 0x1d: switch(bc) {
					case 0: /*eatanxy*/ break;
					case 1: /*eatanxz*/ break;
					case 2: /*sum*/ break;
					case 3: /**/ break;
				} break;
				case 0x1e: switch(bc) {
					case 0: /*esqrt*/ break;
					case 1: /*ersqrt*/ break;
					case 2: /*ercpr*/ break;
					case 3: /**/ break;
				}break;
				case 0x1f: switch(bc) {
					case 0: /*esin*/break;
					case 1: /*eatan*/ break;
					case 2: /*eexp*/break;
					case 3: break;
				}break;
			}break;
		}
	}
}


void init(struct ps2* ps2) {
	ps2->ee.ram = aligned_alloc(4096, EE_RAM_SIZE);
	ps2->ee.scratch = aligned_alloc(4096, EE_SCRATCHPAD_SIZE);
	ps2->ee.bios = aligned_alloc(4096, BIOS_SIZE);
	ps2->ee.regs = aligned_alloc(4096, EE_REGS_SIZE);

	ps2->vu0.code = NULL;
	ps2->vu0.data = NULL;
	ps2->vu1.code = NULL;
	ps2->vu1.data = NULL;

	ps2->iop.ram = aligned_alloc(4096, IOP_IOP_RAM_SIZE);
	ps2->iop.scratch = aligned_alloc(4096, 4096); //TODO: how big is this?
	ps2->iop.bios = ps2->ee.bios;
	ps2->iop.regs = aligned_alloc(4096, IOP_REGS_SIZE);
	ps2->gs.regs = aligned_alloc(4096, GS_REGS_SIZE);

	//values from pcsx2
	ps2->ee.pc._u32[0] = 0xbfc00000;
	ps2->ee.cop0.config._u32[0] = 0x440;
	ps2->ee.cop0.status._u32[0] = 0x70400004; // COP0 enabled | BEV = 1 | TS = 1
	ps2->ee.cop0.prid._u32[0] = 0x00002e20; // PRevID = Revision ID, same as R5900
	ps2->ee.cop1.fcr[0]._u32[0] = 0x00002e00; // fpu Revision..
	ps2->ee.cop1.fcr[31]._u32[0] = 0x01000001; // fpu Status/Control
}

void fini(struct ps2* ps2) {
	free(ps2->gs.regs);
	free(ps2->iop.regs);
	//no free for ps2.iop.bios because it is also ps2.ee.bios
	free(ps2->iop.scratch);
	free(ps2->iop.ram);
	free(ps2->ee.regs);
	free(ps2->ee.bios);
	free(ps2->ee.scratch);
	free(ps2->ee.ram);
}

void loadbios(struct ps2* ps2, char *path) {
	FILE *fp;
	char *abspath = realpath(path, NULL);
	fp = fopen(abspath, "r");
	dieif(fp == nullptr, "could not open %s", abspath);
	fseek(fp, 0L, SEEK_END);
	size_t size = (size_t) ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	assert(size == 1024 * 1024 * 4);
	fread(ps2->ee.bios, size, 1, fp);
	ps2->ee.pc._u32[0] = 0xbfc00000;
	free(abspath);
	fclose(fp);
}



void loadelf(struct ps2* ps2, char *path) {
	FILE *fp;
	char *abspath = realpath(path, NULL);
	fp = fopen(abspath, "r");
	dieif(fp == NULL, "could not open %s", abspath);
	fseek(fp, 0L, SEEK_END);
	size_t size = (size_t) ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	u8 *buffer = malloc(size);

	fread(buffer, size, 1, fp);

	Elf32_Ehdr *elf_header = (Elf32_Ehdr *) buffer;
	Elf32_Phdr *phdr = (Elf32_Phdr *) ((uint8_t *) buffer + elf_header->e_phoff);
	printf("entry %x\n", elf_header->e_entry);
	printf("num pheaders %x\n", elf_header->e_phnum);
	printf("phoff %x\n", elf_header->e_phoff);
	printf("prog offset %x\n", phdr->p_offset);
	printf("paddr %x\n", phdr->p_paddr);
	printf("vaddr %x\n", phdr->p_vaddr);
	printf("filesz %x\n", phdr->p_filesz);
	printf("memsz %x\n", phdr->p_memsz);

	memset((u8 *) ps2->ee.ram + phdr->p_vaddr, 0, phdr->p_memsz);
	memcpy((u8 *) ps2->ee.ram + phdr->p_vaddr, (uint8_t *) buffer + phdr->p_offset, phdr->p_filesz);

	ps2->ee.pc._u32[0] = elf_header->e_entry;
	free(buffer);
	free(abspath);
	fclose(fp);
}


void step(struct ps2* ps2, int instrs){
	for(int i = instrs; i > 0; i--) {
		reg32 op, pc;
		bool delay;
		u32 instr = memread32(ps2, ps2->ee.pc._u32[0]);
		char dis[1024];

		printf("%08x %s\n", ps2->ee.pc._u32[0], dis);


		delay = ps2->ee._in_branch_delay;
		interpret(ps2, instr, true, true);
		if(delay){
			//we just executed the delay slot so set new pc
			ps2->ee.pc = ps2->ee._pc_latch;
			ps2->ee._in_branch_delay = false;
		} else {
			ps2->ee.pc._u32[0]+=4;
		}
	}
}


int main(int argc, char **argv) {
	struct ps2 ps2;
	init(&ps2);
	//loadbios("../bios/ps2.bin");
	loadelf(&ps2, "../thirdparty/ps2autotests/tests/cpu/ee/alu.elf");
	//loadelf("../elfs/ee/helloworld");
	char dis[1024];

	step(&ps2, 50000);
	fini(&ps2);
}
