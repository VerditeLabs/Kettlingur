#pragma once


#define rd() ((opcode._u32[0] >> 11) & 0x1f)
#define rs() ((opcode._u32[0] >> 21) & 0x1f)
#define rt() ((opcode._u32[0] >> 16) & 0x1f)
#define sa() ((opcode._u32[0] >> 6) & 0x1f)
#define base() rs()


#define rs32s ps2.ee.gpr.raw[((opcode._u32[0] >> 21) & 0x1f)]._s32[0]
#define rt32s ps2.ee.gpr.raw[((opcode._u32[0] >> 16) & 0x1f)]._s32[0]
#define rd32s ps2.ee.gpr.raw[((opcode._u32[0] >> 11) & 0x1f)]._s32[0]
#define rs32u ps2.ee.gpr.raw[((opcode._u32[0] >> 21) & 0x1f)]._u32[0]
#define rt32u ps2.ee.gpr.raw[((opcode._u32[0] >> 16) & 0x1f)]._u32[0]
#define rd32u ps2.ee.gpr.raw[((opcode._u32[0] >> 11) & 0x1f)]._u32[0]

#define rs64s ps2.ee.gpr.raw[((opcode._u32[0] >> 21) & 0x1f)]._s64[0]
#define rt64s ps2.ee.gpr.raw[((opcode._u32[0] >> 16) & 0x1f)]._s64[0]
#define rd64s ps2.ee.gpr.raw[((opcode._u32[0] >> 11) & 0x1f)]._s64[0]
#define rs64u ps2.ee.gpr.raw[((opcode._u32[0] >> 21) & 0x1f)]._u64[0]
#define rt64u ps2.ee.gpr.raw[((opcode._u32[0] >> 16) & 0x1f)]._u64[0]
#define rd64u ps2.ee.gpr.raw[((opcode._u32[0] >> 11) & 0x1f)]._u64[0]

#define rsi(dt, i) ps2.ee.gpr.raw[((opcode._u32[0] >> 21) & 0x1f)]._##dt[i]
#define rti(dt, i) ps2.ee.gpr.raw[((opcode._u32[0] >> 16) & 0x1f)]._##dt[i]
#define rdi(dt, i) ps2.ee.gpr.raw[((opcode._u32[0] >> 11) & 0x1f)]._##dt[i]

#define base32s ps2.ee.gpr.raw[((opcode._u32[0] >> 21) & 0x1f)]._s32[0]

#define lo32s ps2.ee.lo._s32[0]
#define hi32s ps2.ee.hi._s32[0]
#define lo32u ps2.ee.lo._u32[0]
#define hi32u ps2.ee.hi._u32[0]
#define lo64s ps2.ee.lo._s64[0]
#define hi64s ps2.ee.hi._s64[0]
#define lo64u ps2.ee.lo._u64[0]
#define hi64u ps2.ee.hi._u64[0]


#define pc32s ps2.ee.pc._s32[0]
#define pc32u ps2.ee.pc._u32[0]


#define ra32s ps2.ee.gpr.ra._s32[0]
#define ra32u ps2.ee.gpr.ra._u32[0]
#define ra64s ps2.ee.gpr.ra._s64[0]
#define ra64u ps2.ee.gpr.ra._u64[0]

#define simm16 (opcode._s16[0])
#define uimm16 (opcode._u16[0])
#define offset (((s32)(opcode._s16[0])) << 2)
