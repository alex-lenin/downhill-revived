/* fiber.c's switch for AAPCS64 (Android, Linux, macOS): x19-x28, the frame pointer x29, the link
   register x30 (where ret goes), d8-d15 (the low halves of v8-v15) and FPCR are callee-saved. */
#include <stdint.h>
#include <string.h>
#include "fiber_switch.h"

/* The symbols: ELF's, or Mach-O's (a leading underscore, no .type) */
#ifdef __APPLE__
#define ASM_FUNCTION(name) ".globl _" #name "\n.p2align 2\n_" #name ":\n"
#define ASM_SYMBOL(name) "_" #name
#else
#define ASM_FUNCTION(name) ".globl " #name "\n.type " #name ",@function\n.p2align 2\n" #name ":\n"
#define ASM_SYMBOL(name) #name
#endif
/* x0=&save, x1=resume. One 176-byte frame:
     [sp+0]   x19 x20   [sp+16]  x21 x22   [sp+32]  x23 x24   [sp+48]  x25 x26
     [sp+64]  x27 x28   [sp+80]  x29 x30   [sp+96]  d8 d9     [sp+112] d10 d11
     [sp+128] d12 d13   [sp+144] d14 d15   [sp+160] FPCR
   A new fiber's frame returns (x30) into fiber_start with x19 holding fiber_entry's argument. */
__asm__(
".text\n"
ASM_FUNCTION(fiber_switch_asm)
"  sub sp, sp, #176\n"
"  stp x19, x20, [sp,#0]\n"
"  stp x21, x22, [sp,#16]\n"
"  stp x23, x24, [sp,#32]\n"
"  stp x25, x26, [sp,#48]\n"
"  stp x27, x28, [sp,#64]\n"
"  stp x29, x30, [sp,#80]\n"
"  stp d8, d9, [sp,#96]\n"
"  stp d10, d11, [sp,#112]\n"
"  stp d12, d13, [sp,#128]\n"
"  stp d14, d15, [sp,#144]\n"
"  mrs x9, fpcr\n"
"  str x9, [sp,#160]\n"
"  mov x2, sp\n"
"  str x2, [x0]\n"
"  mov sp, x1\n"
"  ldp x19, x20, [sp,#0]\n"
"  ldp x21, x22, [sp,#16]\n"
"  ldp x23, x24, [sp,#32]\n"
"  ldp x25, x26, [sp,#48]\n"
"  ldp x27, x28, [sp,#64]\n"
"  ldp x29, x30, [sp,#80]\n"
"  ldp d8, d9, [sp,#96]\n"
"  ldp d10, d11, [sp,#112]\n"
"  ldp d12, d13, [sp,#128]\n"
"  ldp d14, d15, [sp,#144]\n"
"  ldr x9, [sp,#160]\n"
"  msr fpcr, x9\n"
"  isb\n"
"  add sp, sp, #176\n"
"  ret\n"
ASM_FUNCTION(fiber_start)
"  mov x0, x19\n"
"  bl " ASM_SYMBOL(fiber_entry) "\n"
"  brk #0\n"
);
void fiber_start(void);

void *fiber_frame(uintptr_t lo,uintptr_t hi,void *arg) {
    (void)lo;
    /* x29 = 0 ends the frame chain; sp stays 16-aligned before and after the frame. */
    uint64_t *frame=(uint64_t *)(((hi-16)&~(uintptr_t)15)-176), fpcr;
    __asm__ volatile("mrs %0, fpcr" : "=r"(fpcr));
    memset(frame,0,176);
    frame[0]=(uintptr_t)arg; frame[11]=(uintptr_t)fiber_start; frame[20]=fpcr;
    return frame;
}
