/* Support for MIPS functions recompiled to C by port/tools/recomp.py: the game's functions that
   have no C version yet run instruction by instruction on the emulated register file of
   ps2asm.h. Addresses are host addresses: on the 32-bit PC a PS2 pointer and a host pointer are
   the same value, so loads and stores go straight to memory. */
#ifndef RECOMP_H
#define RECOMP_H

#include "ps2asm.h" /* no C library headers: recompiled code calls game functions named strcmp etc. */
#include "ps2float.h"

#define GPR(n) ps2.gpr[n].d[0]
#define UW(n) ((uint32_t)ps2.gpr[n].d[0])
#define SW(n) ((int32_t)ps2.gpr[n].d[0])
#define S32(x) ((uint64_t)(int64_t)(int32_t)(x)) /* 32-bit results are sign-extended */
#define FPR(n) ps2.fpr[n]
#define FPRW(n) (*(int32_t *)&ps2.fpr[n])        /* the bits of an FPU register */

#define ADDR(base, off) ((uintptr_t)(uint32_t)(UW(base) + (int32_t)(off)))
/* ps2_load_address with one test on the way to RAM: INTC_STAT and every fenced range lie in
   [0x10000000, 0x12001100), the only addresses where it does more than ps2_ram_canonical. */
static inline uintptr_t rc_load_address(uintptr_t address)
{
    if (__builtin_expect((uint32_t)address - 0x10000000u < 0x02001100u, 0))
        return ps2_load_address(address);
    return ps2_ram_canonical(address);
}
#define M8(a) (*(int8_t *)rc_load_address(a))
#define MU8(a) (*(uint8_t *)rc_load_address(a))
#define M16(a) (*(int16_t *)rc_load_address(a))
#define MU16(a) (*(uint16_t *)rc_load_address(a))
#define MF32(a) (*(float *)rc_load_address(a))
#define M32(a) (*(int32_t *)rc_load_address(a))
#define MU32(a) (*(uint32_t *)rc_load_address(a))
#define M64(a) (*(uint64_t *)rc_load_address(a))
#define M128(a) (*(ps2_q128 *)ps2_memory_address((a) & ~(uintptr_t)15)) /* lq/sq ignore the low 4 bits */

/* A ps2_exec template that recomp.py turned into C (its COP2 register moves and vcallms, finding
   090 SP11). ps2asm_aot.py still reads the template, so its generated program keeps its slot in
   the function table: the game's RAM holds table indices, and SP1 saw the state trace differ
   when templates were dropped. The empty asm keeps the literal in .rodata as well. It costs no
   instruction. */
#define ps2_exec_kept(tmpl) __asm__ volatile("" : : "r"(tmpl))

/* ---- The wasm build's recompiled bodies (recomp.py WASM_EMIT, finding 090 SP15) ----
   Guest registers live in C locals as well as in ps2: a write goes to both (so ps2 is always
   current for calls, ps2_exec, interrupts and the state trace), a read takes the local, and the
   locals are read back from ps2 after anything that may change ps2 (a call, an instruction run
   through its generated code). The compiler then keeps registers in wasm locals across guest
   stores, which it otherwise has to assume may write ps2. */
typedef union { float f; int32_t w; } rc_fpr; /* an FPU register: value and bits */
#define RADDR(base, off) ((uintptr_t)(uint32_t)((uint32_t)(base) + (int32_t)(off)))
/* An integer moved into the FPU (mtc1) is opaque to the optimizer: the EE rounds toward zero at
   run time, and an operation the compiler folded on two constants would round to nearest. */
static inline int32_t rc_opaque32(int32_t x) { __asm__("" : "+r"(x)); return x; }
/* ps2_store32/64/128 without the call where they are only the store: outside
   [0x10000000, 0x13000000) nothing is fenced and nothing is MMIO, and ps2_memory_address is
   ps2_ram_canonical (SP14's fold of the low view and the 0x38 mirror). */
static inline void rc_store32(uintptr_t address, uint32_t value)
{
    if (__builtin_expect((uint32_t)address - 0x10000000u < 0x03000000u, 0))
        ps2_store32(address, value);
    else
        *(volatile uint32_t *)ps2_ram_canonical(address) = value;
}
static inline void rc_store64(uintptr_t address, uint64_t value)
{
    if (__builtin_expect((uint32_t)address - 0x10000000u < 0x03000000u, 0))
        ps2_store64(address, value);
    else
        *(volatile uint64_t *)ps2_ram_canonical(address) = value;
}
static inline void rc_store128(uintptr_t address, const ps2_q128 *value)
{
    if (__builtin_expect((uint32_t)address - 0x10000000u < 0x03000000u, 0))
        ps2_store128(address, value);
    else
        __builtin_memcpy((void *)ps2_ram_canonical(address), value, 16);
}
/* EE MUL/ADD/SUB in single precision, the same bits as ps2float.h's double forms on clamped
   (finite, normal or zero) operands, in any rounding mode, with or without FTZ/DAZ:
   - the double product of two floats is exact, so (float) of it is one rounding of the exact
     product, as the float product is;
   - after ps2_ee_align the double sum is exact too (the operands' bits lie within 26 bits), so
     the same holds for the sum;
   - an overflow gives the largest magnitude (toward zero) or infinity (to nearest), which
     ps2_float_input turns into the largest magnitude of its sign, as ps2_float_result does;
     a denormal result is flushed by FTZ or by ps2_float_input, as after (float).
   No promote/demote pair per operation (wasm2c's NaN checks; the browser's helper calls). */
static inline float rc_float_mul(float a, float b)
{ return ps2_float_input(ps2_float_input(a) * ps2_float_input(b)); }
static inline float rc_ee_add(float a, float b)
{ ps2_ee_align(&a, &b); return ps2_float_input(ps2_float_input(a) + ps2_float_input(b)); }
static inline float rc_ee_sub(float a, float b)
{ ps2_ee_align(&a, &b); return ps2_float_input(ps2_float_input(a) - ps2_float_input(b)); }

/* The call stack of recompiled code (sp = $29): set up on the first entry from C. */
void ps2_recomp_enter(void);
/* Hot path of every recompiled wrapper: once $sp stands, entering is a load
   and a test, not a call. The slow path keeps the exact original semantics. */
static inline void ps2_recomp_enter_inline(void)
{
    if (!GPR(29))
        ps2_recomp_enter();
}
/* A VU0/MMI instruction of the wasm build: fn is its generated code in ps2asm_aot.inc
   (ps2asm_aot.py defines it from this text; the text's first run goes through ps2_exec). */
#define PS2X(fn, text) do { void fn(void); fn(); } while (0)
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
