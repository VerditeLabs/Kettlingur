#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include <thirdparty/elf.h>

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

overload s8 satadd(s8 a, s8 b){
	s16 ret = ((s16)a) + (s16)b;
	if(ret > 0x7f){
		return 0x7f;
	} else if(ret < -0x80){
		return (s8)0x80;
	}
	return (s8)ret;
}

overload s16 satadd(s16 a, s16 b){
	s32 ret = ((s32)a) + (s32)b;
	if(ret > 0x7fff){
		return 0x7fff;
	} else if(ret < -0x8000){
		return (s16)0x8000;
	}
	return (s16)ret;
}

overload s32 satadd(s32 a, s32 b){
	s64 ret = ((s64)a) + (s64)b;
	if(ret > 0x7fffffff){
		return 0x7fffffff;
	} else if(ret < -0x80000000ll){
		return (s32)0x80000000;
	}
	return (s32)ret;
}


overload s8 satsub(s8 a, s8 b){
	s16 ret = ((s16)a) - (s16)b;
	if(ret > 0x7f){
		return 0x7f;
	} else if(ret < -0x80){
		return (s8)0x80;
	}
	return (s8)ret;
}

overload s16 satsub(s16 a, s16 b){
	s32 ret = ((s32)a) - (s32)b;
	if(ret > 0x7fff){
		return 0x7fff;
	} else if(ret < -0x8000){
		return (s16)0x8000;
	}
	return (s16)ret;
}

overload s32 satsub(s32 a, s32 b){
	s64 ret = ((s64)a) - (s64)b;
	if(ret > 0x7fffffff){
		return 0x7fffffff;
	} else if(ret < -0x80000000ll){
		return (s32)0x80000000;
	}
	return (s32)ret;
}

