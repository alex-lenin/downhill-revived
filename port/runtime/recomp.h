/* Support for MIPS functions recompiled to C by port/tools/recomp.py: the game's functions that
   have no C version yet run instruction by instruction on the emulated register file of
   ps2asm.h. Addresses are host addresses: on the 32-bit PC a PS2 pointer and a host pointer are
   the same value, so loads and stores go straight to memory. */
#ifndef RECOMP_H
#define RECOMP_H

#include "ps2asm.h" /* no C library headers: recompiled code calls game functions named strcmp etc. */
#include "ps2float.h"
#include "fp_trap.h"

#define GPR(n) ps2.gpr[n].d[0]
#define UW(n) ((uint32_t)ps2.gpr[n].d[0])
#define SW(n) ((int32_t)ps2.gpr[n].d[0])
#define S32(x) ((uint64_t)(int64_t)(int32_t)(x)) /* 32-bit results are sign-extended */
#define FPR(n) ps2.fpr[n]
#define FPRW(n) (*(int32_t *)&ps2.fpr[n])        /* the bits of an FPU register */

#define ADDR(base, off) ((uintptr_t)(uint32_t)(UW(base) + (int32_t)(off)))
#define M8(a) (*(int8_t *)ps2_load_address(a))
#define MU8(a) (*(uint8_t *)ps2_load_address(a))
#define M16(a) (*(int16_t *)ps2_load_address(a))
#define MU16(a) (*(uint16_t *)ps2_load_address(a))
#define MF32(a) (*(float *)ps2_load_address(a))
#define M32(a) (*(int32_t *)ps2_load_address(a))
#define MU32(a) (*(uint32_t *)ps2_load_address(a))
#define M64(a) (*(uint64_t *)ps2_load_address(a))
#define M128(a) (*(ps2_q128 *)ps2_memory_address((a) & ~(uintptr_t)15)) /* lq/sq ignore the low 4 bits */

/* The call stack of recompiled code (sp = $29): set up on the first entry from C. */
void ps2_recomp_enter(void);
/* Unknown target of an indirect jump: reports and stops. */
void ps2_recomp_bad_jump(const char *func, uint32_t target);

/* EE FPU: division by zero gives +-max instead of infinity; sqrt takes |x|. */
float ps2_fdiv(float a, float b);
float ps2_fmul(float a, float b);
float ps2_fadd(float a, float b);
float ps2_fsub(float a, float b);
float ps2_fsqrt(float a);
int32_t ps2_cvt_w_s(float a); /* truncates toward zero, saturating */

/* Typed dispatch of known native scalar callbacks; zero means an integer-only
 * or external callback which can use the legacy GPR-only fallback. */
int ps2_recomp_scalar_call(uintptr_t target);

/* Software-float doubles live in a GPR as their IEEE bits. */
static inline uint64_t ps2_d2u(double d) { uint64_t u; __builtin_memcpy(&u, &d, 8); return u; }
static inline double ps2_u2d(uint64_t u) { double d; __builtin_memcpy(&d, &u, 8); return d; }

/* EE integer division: no trap, defined results for a zero divisor. */
void ps2_div(int32_t a, int32_t b);
void ps2_divu(uint32_t a, uint32_t b);

/* Unaligned loads (little-endian ldl/ldr, lwl/lwr). */
uint64_t ps2_ldl(uint64_t rt, uintptr_t addr);
uint64_t ps2_ldr(uint64_t rt, uintptr_t addr);
uint64_t ps2_lwl(uint64_t rt, uintptr_t addr);
uint64_t ps2_lwr(uint64_t rt, uintptr_t addr);

/* Unaligned stores (sdl/sdr, swl/swr). */
void ps2_sdl(uint64_t rt, uintptr_t addr);
void ps2_sdr(uint64_t rt, uintptr_t addr);
void ps2_swl(uint64_t rt, uintptr_t addr);
void ps2_swr(uint64_t rt, uintptr_t addr);

/* A COP0 register: Count (9) runs at the EE's 294.912 MHz from host time. */
uint32_t ps2_mfc0(int reg);

#endif
