/* fiber.c's switch for x86-64 System V (Linux): rbx, rbp, r12-r15 and MXCSR are callee-saved. */
#include <stdint.h>
#include <string.h>
#include <xmmintrin.h>
#include "fiber_switch.h"

/* rdi=&save, rsi=resume. One 56-byte frame below the return address:
     [0] rbx  [8] rbp  [16] r12  [24] r13  [32] r14  [40] r15  [48] MXCSR
   A new fiber's frame returns into fiber_start with rbx holding fiber_entry's argument. */
__asm__(
".text\n"
".globl fiber_switch_asm\n.type fiber_switch_asm,@function\nfiber_switch_asm:\n"
"  sub $56,%rsp\n"
"  mov %rbx,0(%rsp)\n"
"  mov %rbp,8(%rsp)\n"
"  mov %r12,16(%rsp)\n"
"  mov %r13,24(%rsp)\n"
"  mov %r14,32(%rsp)\n"
"  mov %r15,40(%rsp)\n"
"  stmxcsr 48(%rsp)\n"
"  mov %rsp,(%rdi)\n"
"  mov %rsi,%rsp\n"
"  mov 0(%rsp),%rbx\n"
"  mov 8(%rsp),%rbp\n"
"  mov 16(%rsp),%r12\n"
"  mov 24(%rsp),%r13\n"
"  mov 32(%rsp),%r14\n"
"  mov 40(%rsp),%r15\n"
"  ldmxcsr 48(%rsp)\n"
"  add $56,%rsp\n"
"  ret\n"
".globl fiber_start\n.type fiber_start,@function\nfiber_start:\n"
"  mov %rbx,%rdi\n"
"  call fiber_entry\n"
"  ud2\n"
);
void fiber_start(void);

void *fiber_frame(uintptr_t lo,uintptr_t hi,void *arg) {
    (void)lo;
    /* fiber_start runs at a 16-byte aligned rsp, so its call keeps the ABI; above it a zero
       return address (and rbp 0) ends the frame chain for unwinders. */
    uintptr_t start=(hi-16)&~(uintptr_t)15;
    uint64_t *frame=(uint64_t *)(start-64);
    memset(frame,0,64+16);
    frame[0]=(uintptr_t)arg; frame[6]=_mm_getcsr(); frame[7]=(uintptr_t)fiber_start;
    return frame;
}
