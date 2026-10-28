/* The one part of the EE fibers (fiber.c) that differs per CPU and ABI: the register switch and
   a new fiber's first frame. build_host64.py's target table links one adapter: fiber_win64.c
   (Windows x64), fiber_sysv64.c (x86-64 System V: Linux), fiber_arm64.c (AAPCS64: Android,
   Linux, macOS). */
#ifndef DHD_HOST64_FIBER_SWITCH_H
#define DHD_HOST64_FIBER_SWITCH_H
#include <stdint.h>
/* Saves the ABI's callee-saved registers and the FP control register on the running native
   stack, stores that stack pointer in *save and resumes the fiber saved at resume. */
void fiber_switch_asm(void **save, void *resume);
/* Lays out a new fiber's first frame at the top of its native stack [lo, hi) and returns it for
   fiber_switch_asm: resuming it calls fiber_entry(arg) with the creator's FP control register. */
void *fiber_frame(uintptr_t lo, uintptr_t hi, void *arg);
/* fiber.c: runs a new fiber's function; never returns. */
void fiber_entry(void *arg);
#endif
