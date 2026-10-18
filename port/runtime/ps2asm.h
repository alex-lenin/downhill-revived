/* PS2 inline asm runtime: the emulated EE/VU0 register state that the game's inline asm
   (rewritten into ps2_asm() calls by port/tools/transform.py) works on. */
#ifndef PS2ASM_H
#define PS2ASM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef union {
    uint8_t b[16];
    uint16_t h[8];
    int16_t sh[8];
    uint32_t w[4];
    int32_t sw[4];
    uint64_t d[2];
    float f[4];
} ps2_q128;

/* Registers 0..31 are the real ones; 32..63 stand for asm operands %0..%31 bound by constraint. */
typedef struct {
    ps2_q128 gpr[64];
    uint64_t lo, hi;
    uint64_t lo1, hi1; /* upper halves of the 128-bit LO/HI (MMI) */
    float fpr[64];
    float fpu_acc;
    ps2_q128 vf[32]; /* vf0 always reads (0, 0, 0, 1) */
    float vacc[4];
    uint16_t vi[16];
    float q, i;
    uint32_t r; /* VU0 random register */
    uint8_t *vu0_code; /* VU0 micro memory, 4 KB: at its PS2 address 0x11000000 in the game */
    ps2_q128 *vu0_mem; /* VU0 data memory, 4 KB: at its PS2 address 0x11004000 in the game
                          (port/runtime/memory.c), where the game also writes it */
    uint32_t cop0[32]; /* system control registers: Status (12) for the interrupt switches */
    uint32_t vu0_ctrl[16]; /* VU0 control registers vi16..vi31 (cfc2/ctc2), apart from R, I, Q */
    float p; /* VU1 elementary-function result */
} ps2_cpu;

extern ps2_cpu ps2;
extern void (*ps2_mmio_write)(uintptr_t address, uint64_t value, unsigned size);
extern void (*ps2_graphics_sync)(void);
/* Fence mapped VU1/VIF1/GS state before the EE reads or changes it. */
static inline uintptr_t ps2_memory_address(uintptr_t address)
{
    if (ps2_graphics_sync && ((address >= 0x11008000 && address < 0x11010000) ||
         (address >= 0x10003C00 && address < 0x10004000) ||
         (address >= 0x10005000 && address < 0x10005010) ||
         (address >= 0x10003000 && address < 0x10003030) ||
         (address >= 0x12001000 && address < 0x12001100)))
        ps2_graphics_sync();
    return address;
}
/* INTC_STAT (0x1000F000) holds the EE's pending interrupt causes. The game reads it only to wait
   for VBLANK_START (func_0025AB70, func_0025AC00: sceGsSyncV), so a read lets the kernel run
   time up to the vblank first. */
#define PS2_INTC_STAT 0x1000F000u
extern void (*ps2_intc_read)(void);
static inline uintptr_t ps2_load_address(uintptr_t address)
{
    if (address == PS2_INTC_STAT && ps2_intc_read)
        ps2_intc_read();
    return ps2_memory_address(address);
}
void ps2_store32(uintptr_t address, uint32_t value);
void ps2_store64(uintptr_t address, uint64_t value);
void ps2_store128(uintptr_t address, const ps2_q128 *value);

/* A VU0 microprogram started with vcallms; addr is the byte address in micro memory. */
typedef void (*ps2_vu0_micro_fn)(ps2_cpu *cpu);
void ps2_vu0_register_micro(unsigned addr, ps2_vu0_micro_fn fn);

/* Registers the native microprograms of port/runtime/vu0_micro.c; call once at startup. */
void ps2_vu0_init(void);

/* Runs the microcode in VU0 micro memory from addr (bytes) to its end (vu0_interp.c), for a
   microprogram with no native version. Returns 0, or -1 on an instruction VU0 doesn't have. */
int ps2_vu0_run(ps2_cpu *cpu, unsigned addr);

/* The translated VU blocks and native kernels (vu0_interp.c, PS2_VU_AOT) run only where micro
   memory holds the microcode they were translated from. Their copy of it is taken here from the
   game's .vutext (vutext: PS2 address 0x263BC0, read from the player's disc), not kept in
   dhd.exe. Once, before any VU code runs; nothing without PS2_VU_AOT. */
void ps2_vu_aot_load(const void *vutext);

/* VU1's registers, which VU0 micro code reads at data address 0x4000 (set by the VIF1 HLE),
   the fence that makes them current, and that 1 KB view as VF0..31 then VI0..31 per quadword. */
extern ps2_cpu *ps2_vu1_regs;
extern uint64_t ps2_vu1_code_version;  /* bumped when VIF1 MPG changes VU1 micro memory */
extern int ps2_vu1_native_mode;  /* -1: DHD_VU1_NATIVE decides; 0 or 1 forces the native kernels off or on (tests) */
extern int ps2_vu0_native_mode;  /* same override for DHD_VU0_NATIVE */
extern unsigned long long ps2_vu0_native_runs;
extern void (*ps2_vu1_regs_sync)(void);
void ps2_vu0_vu1_file(ps2_q128 file[64]);

/* VU1 has its own register file; vu0_mem/code point to its 16 KB memories.
   TPC (vu0_ctrl[10]) records the continuation byte address for MSCNT. */
typedef int (*ps2_vu_kick_fn)(void *opaque, unsigned qword);
/* Optional diagnostic hook reports visible CLIP at the XGKICK instruction. */
typedef void (*ps2_vu_kick_trace_fn)(unsigned pc, unsigned clip, unsigned qword);
void ps2_vu1_set_kick_trace(ps2_vu_kick_trace_fn trace);
int ps2_vu1_run(ps2_cpu *cpu, unsigned addr, unsigned top, unsigned itop,
                ps2_vu_kick_fn kick, void *opaque);
/* Native-loop coverage, indexed by the dispatch kind's bit number. */
enum { PS2_VU1_NATIVE_KINDS = 12 };
extern unsigned long long ps2_vu1_native_runs[PS2_VU1_NATIVE_KINDS];


/* One instruction of statically recompiled MIPS code (port/tools/recomp.py), e.g.
   "vmulx.xyz $vf1, $vf2, $vf3x". Unlike ps2_asm it may read the argument registers. */
void ps2_exec(const char *insn);
int ps2_exec_check(const char *insn); /* parse only; returns the number of problems */

/* Count of templates that failed to parse or ran something unsupported (bring-up diagnostics). */
int ps2_asm_problems(void);

/* Parse a template without running it; returns the number of problems found. */
int ps2_asm_check(const char *tmpl, const char *constraints, int n);

#ifdef __cplusplus
}
#endif

#endif
