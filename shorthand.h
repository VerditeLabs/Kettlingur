#pragma once


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