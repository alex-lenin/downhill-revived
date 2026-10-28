/* fiber.c's switch for the Windows x64 ABI: rbx, rbp, rdi, rsi, r12-r15, xmm6-xmm15 and MXCSR
   are callee-saved. The thread's stack bounds in its TEB (StackBase gs:0x08, StackLimit gs:0x10,
   DeallocationStack gs:0x1478, stable since the first x64 Windows; Boost.Context's Windows switch
   swaps the same fields) follow the running stack: __chkstk probes from StackLimit down, and the
   exception dispatcher and RtlCaptureStackBackTrace accept only frames inside the bounds. This is
   all SwitchToFiber does that the game needs; its fiber-local storage and fiber data are unused. */
#include <stdint.h>
#include <string.h>
#include <xmmintrin.h>
#include "fiber_switch.h"

/* rcx=&save, rdx=resume. One 264-byte frame below the return address (16-aligned for movaps):
     [0x00] xmm6-xmm15   [0xa0] rbx rbp rdi rsi r12 r13 r14 r15
     [0xe0] StackBase StackLimit DeallocationStack   [0xf8] MXCSR
   A new fiber's frame returns into fiber_start with rbx holding fiber_entry's argument; its unwind
   data ends every stack walk at the zero return address above it. */
__asm__(
".att_syntax prefix\n"  /* clang-cl reads file-scope asm as Intel syntax */
".text\n"
".globl fiber_switch_asm\n.def fiber_switch_asm; .scl 2; .type 32; .endef\n"
"fiber_switch_asm:\n"
".seh_proc fiber_switch_asm\n"
"  sub $0x108,%rsp\n"
".seh_stackalloc 0x108\n"
".seh_endprologue\n"
"  movaps %xmm6,0x00(%rsp)\n"
"  movaps %xmm7,0x10(%rsp)\n"
"  movaps %xmm8,0x20(%rsp)\n"
"  movaps %xmm9,0x30(%rsp)\n"
"  movaps %xmm10,0x40(%rsp)\n"
"  movaps %xmm11,0x50(%rsp)\n"
"  movaps %xmm12,0x60(%rsp)\n"
"  movaps %xmm13,0x70(%rsp)\n"
"  movaps %xmm14,0x80(%rsp)\n"
"  movaps %xmm15,0x90(%rsp)\n"
"  mov %rbx,0xa0(%rsp)\n"
"  mov %rbp,0xa8(%rsp)\n"
"  mov %rdi,0xb0(%rsp)\n"
"  mov %rsi,0xb8(%rsp)\n"
"  mov %r12,0xc0(%rsp)\n"
"  mov %r13,0xc8(%rsp)\n"
"  mov %r14,0xd0(%rsp)\n"
"  mov %r15,0xd8(%rsp)\n"
"  mov %gs:0x08,%rax\n"
"  mov %rax,0xe0(%rsp)\n"
"  mov %gs:0x10,%rax\n"
"  mov %rax,0xe8(%rsp)\n"
"  mov %gs:0x1478,%rax\n"
"  mov %rax,0xf0(%rsp)\n"
"  stmxcsr 0xf8(%rsp)\n"
"  mov %rsp,(%rcx)\n"
"  mov %rdx,%rsp\n"
"  movaps 0x00(%rsp),%xmm6\n"
"  movaps 0x10(%rsp),%xmm7\n"
"  movaps 0x20(%rsp),%xmm8\n"
"  movaps 0x30(%rsp),%xmm9\n"
"  movaps 0x40(%rsp),%xmm10\n"
"  movaps 0x50(%rsp),%xmm11\n"
"  movaps 0x60(%rsp),%xmm12\n"
"  movaps 0x70(%rsp),%xmm13\n"
"  movaps 0x80(%rsp),%xmm14\n"
"  movaps 0x90(%rsp),%xmm15\n"
"  mov 0xa0(%rsp),%rbx\n"
"  mov 0xa8(%rsp),%rbp\n"
"  mov 0xb0(%rsp),%rdi\n"
"  mov 0xb8(%rsp),%rsi\n"
"  mov 0xc0(%rsp),%r12\n"
"  mov 0xc8(%rsp),%r13\n"
"  mov 0xd0(%rsp),%r14\n"
"  mov 0xd8(%rsp),%r15\n"
"  mov 0xe0(%rsp),%rax\n"
"  mov %rax,%gs:0x08\n"
"  mov 0xe8(%rsp),%rax\n"
"  mov %rax,%gs:0x10\n"
"  mov 0xf0(%rsp),%rax\n"
"  mov %rax,%gs:0x1478\n"
"  ldmxcsr 0xf8(%rsp)\n"
"  add $0x108,%rsp\n"
"  ret\n"
".seh_endproc\n"
".globl fiber_start\n.def fiber_start; .scl 2; .type 32; .endef\n"
"fiber_start:\n"
".seh_proc fiber_start\n"
"  sub $32,%rsp\n"
".seh_stackalloc 32\n"
".seh_endprologue\n"
"  mov %rbx,%rcx\n"
"  call fiber_entry\n"
"  ud2\n"
".seh_endproc\n"
);
void fiber_start(void);

void *fiber_frame(uintptr_t lo,uintptr_t hi,void *arg) {
    /* fiber_start runs at a 16-byte aligned rsp (32 bytes of home space below it, then its call
       keeps the ABI) with a zero return address at [rsp]. The TEB bounds are the usable stack:
       below lo is the guard page. */
    uintptr_t start=(hi-16)&~(uintptr_t)15;
    uint64_t *frame=(uint64_t *)(start-0x110);
    memset(frame,0,0x110+16);
    frame[0xa0/8]=(uintptr_t)arg;
    frame[0xe0/8]=hi; frame[0xe8/8]=lo; frame[0xf0/8]=lo;
    frame[0xf8/8]=_mm_getcsr(); frame[0x108/8]=(uintptr_t)fiber_start;
    return frame;
}
