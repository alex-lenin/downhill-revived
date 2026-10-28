/* The EE fibers (fiber.c and the CPU's switch, fiber_switch.h) on their own, on every target:
   build_host64.py --target T --fiber-test builds this with T's compiler and adapter and runs it.
   No module: a plain __stack_pointer over guest_memory.c's guest memory. Checks that switches keep
   each fiber's __stack_pointer, call depth, FP control register and callee-saved registers
   (integer and floating-point locals live across the switch); a fiber made from a fiber; a
   returning fiber resumes the root; a large frame (Windows: __chkstk against the TEB's bounds); a
   backtrace from a fiber ends; delete and reuse, the 64 slots, the stack ranges. Child runs:
   --guest-guard writes a slot's guard page, --native-guard writes down a native stack; both must
   fault. */
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include "../host/backtrace_posix.h"  /* <execinfo.h>, or Android's own (API < 33) */
#endif
#if defined(__x86_64__) || defined(_M_X64)
#include <xmmintrin.h>
#endif
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wasm-rt.h"
#include "core_host.h"
#include "memory_layout.h"

#define CHECK(x) do { if (!(x)) { fprintf(stderr,"FAIL line %d: %s\n",__LINE__,#x); exit(1); } } while (0)
#define NOINLINE __attribute__((noinline))
WASM_RT_THREAD_LOCAL uint32_t wasm_rt_call_stack_depth;  /* wasm-rt-impl.c's, not linked here */
void host_log_drain(void) {}  /* host.c's (the POSIX fault reporter's weak reference; Mach-O wants it) */
enum { FIBERS=3, ROUNDS=50, NATIVE_STACK=8*1024*1024 };
static uint32_t stack_pointer=0x100000;
static HostFiber root, fibers[FIBERS+1];
static unsigned switches, finished, children;
static int mode;  /* 1: --guest-guard, 2: --native-guard */

static uint64_t fp_control(void) {
#if defined(__x86_64__) || defined(_M_X64)
    return _mm_getcsr() & ~63u;  /* without the sticky exception flags */
#elif defined(__aarch64__)
    uint64_t f; __asm__ volatile("mrs %0, fpcr" : "=r"(f)); return f;
#else
#error Implement fp_control for this architecture
#endif
}
/* Ten integers and ten doubles live across the switch, each from its own volatile load so none
   can be recomputed after it: the compiler keeps them in callee-saved registers (rbx, rsi, rdi,
   r12-r15 and xmm6-15 on Windows; x19-x28 and d8-d15 on AArch64) or spills them. */
static volatile uint64_t ints[FIBERS+1][10];  /* different in every fiber (main) */
static volatile double reals[FIBERS+1][10];
static NOINLINE void registers(unsigned id,HostFiber to) {
    volatile uint64_t *n=ints[id]; volatile double *f=reals[id];
    uint64_t a=n[0], b=n[1], c=n[2], d=n[3], e=n[4], g=n[5], h=n[6], i=n[7], j=n[8], k=n[9];
    double q=f[0], r=f[1], s=f[2], t=f[3], u=f[4], v=f[5], w=f[6], x=f[7], y=f[8], z=f[9];
    host_fiber_switch(to);
    CHECK(a==n[0] && b==n[1] && c==n[2] && d==n[3] && e==n[4] && g==n[5] && h==n[6] && i==n[7] &&
          j==n[8] && k==n[9]);
    CHECK(q==f[0] && r==f[1] && s==f[2] && t==f[3] && u==f[4] && v==f[5] && w==f[6] && x==f[7] &&
          y==f[8] && z==f[9]);
}
#ifdef _WIN32
/* The TEB's stack bounds (fiber_win64.c) are the running stack's. */
static void stack_bounds(size_t size) {
    NT_TIB *tib=(NT_TIB *)NtCurrentTeb(); ULONG_PTR low,high; uintptr_t here=(uintptr_t)&tib;
    GetCurrentThreadStackLimits(&low,&high);
    CHECK((uintptr_t)tib->StackLimit<here && here<(uintptr_t)tib->StackBase && high==(uintptr_t)tib->StackBase);
    CHECK(!size || ((uintptr_t)tib->StackBase-(uintptr_t)tib->StackLimit==size && low==(uintptr_t)tib->StackLimit));
}
#else
static void stack_bounds(size_t size) { (void)size; }
#endif
static NOINLINE void large_frame(void) {
    volatile char big[256*1024];
    big[0]=1; big[sizeof big-1]=2; CHECK(big[0]+big[sizeof big-1]==3);
}
static NOINLINE void backtrace_ends(void) {
    void *frames[64];
#ifdef _WIN32
    int n=RtlCaptureStackBackTrace(0,64,frames,NULL);
#else
    int n=backtrace(frames,64);
#endif
    CHECK(n>=2 && n<64);
}
static void child(void *arg) { ++children; host_fiber_switch((HostFiber)(uintptr_t)arg); abort(); }
static void worker(void *arg) {
    unsigned id=(unsigned)(uintptr_t)arg;
    HostFiber self=host_fiber_current(), next=id==FIBERS?root:fibers[id+1];
    CHECK(self==fibers[id] && stack_pointer==host64_fiber_stack_top(self) && wasm_rt_call_stack_depth==0);
    if (mode==1) *(volatile char *)(guest_base+stack_pointer-0x40000)=1;
    if (mode==2) {
        /* Down the native stack page by page into the guard below it. */
        volatile char here=0; uintptr_t p=(uintptr_t)&here;  /* integers: not a pointer past here */
        for (size_t below=0;below<=NATIVE_STACK+65536;below+=256) {
            if (below==NATIVE_STACK-65536) fprintf(stderr,"native stack: 8 MiB less 64 KiB written\n");
            *(volatile char *)(p-below)=1;
        }
        fprintf(stderr,"FAIL: no guard below the native stack\n"); exit(1);
    }
    host_fp_mode(id==2?0:HOST_FP_EE);  /* 0: IEEE */
    stack_bounds(NATIVE_STACK); large_frame(); backtrace_ends();
    if (id==1) {  /* a fiber made and deleted by a fiber, as drain_pending's posted jobs */
        HostFiber made=host_fiber_create(child,(void *)(uintptr_t)self);
        CHECK(made==FIBERS+1); host_fiber_switch(made); host_fiber_delete(made);
        CHECK(children==1 && host_fiber_current()==self);
    }
    for (unsigned round=0;round<ROUNDS;round++) {
        stack_pointer-=16*id; wasm_rt_call_stack_depth=id*1000+round;
        uint32_t sp=stack_pointer; uint64_t fp=fp_control();
        ++switches; registers(id,next);
        CHECK(stack_pointer==sp && wasm_rt_call_stack_depth==id*1000+round && fp_control()==fp);
        CHECK(host_fiber_current()==self); stack_bounds(NATIVE_STACK);
    }
    ++finished;  /* returns: fiber_entry resumes the root */
}
static void noop(void *arg) { (void)arg; }
int main(int argc,char **argv) {
#ifdef _WIN32
    SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);
#endif
    if (argc==2) mode=!strcmp(argv[1],"--guest-guard") ? 1 : !strcmp(argv[1],"--native-guard") ? 2 : -1;
    CHECK(mode>=0);
    for (unsigned id=0;id<=FIBERS;id++) {
        for (unsigned i=0;i<10;i++) ints[id][i]=0x1000*id+i+1;
        for (unsigned i=0;i<10;i++) reals[id][i]=100.0*id+i+0.5;
    }
    wasm_rt_memory_t memory={0};
    wasm_rt_allocate_memory(&memory,32768,32770,false,65536);
    host64_fiber_bind(&stack_pointer); root=host_fiber_main(); CHECK(root==65);
    for (unsigned id=1;id<=FIBERS;id++) {
        fibers[id]=host_fiber_create(worker,(void *)(uintptr_t)id); CHECK(fibers[id]==id);
        CHECK(host64_fiber_stack_top(fibers[id])==0x06000000+id*0x40000);
    }
    if (mode) { host_fiber_switch(fibers[1]); return 1; }
    host_fp_mode(0);
    uint64_t fp=fp_control();
    for (unsigned round=0;round<=ROUNDS;round++) {
        wasm_rt_call_stack_depth=7; stack_pointer=0x100000-round;
        registers(0,fibers[1]);
        CHECK(stack_pointer==0x100000-round && wasm_rt_call_stack_depth==7 && fp_control()==fp);
        CHECK(host_fiber_current()==root); stack_bounds(0);
    }
    CHECK(finished==1 && switches==FIBERS*ROUNDS);
    for (unsigned id=2;id<=FIBERS;id++) { registers(0,fibers[id]); CHECK(finished==id); }
    uintptr_t (*ranges)[2]; CHECK(host_stack_ranges(&ranges)==FIBERS);
    CHECK(ranges[0][0]==0x06000000 && ranges[FIBERS-1][1]==0x06000000+FIBERS*0x40000);
    host_fiber_delete(fibers[2]); CHECK(host_stack_ranges(&ranges)==FIBERS-1);
    CHECK(host_fiber_create(noop,NULL)==fibers[2]);
    for (unsigned id=1;id<=FIBERS;id++) host_fiber_delete(fibers[id]);
    HostFiber slots[64];
    for (unsigned i=0;i<64;i++) { slots[i]=host_fiber_create(noop,NULL); CHECK(slots[i]==i+1); }
    CHECK(host_fiber_create(noop,NULL)==HOST_FIBER_NONE);
    CHECK(host_stack_ranges(&ranges)==64 && ranges[63][1]==0x07000000);
    host_fiber_delete(slots[5]); CHECK(host_fiber_create(noop,NULL)==slots[5]);
    host64_fiber_shutdown(); CHECK(!host64_fiber_id());
    wasm_rt_free_memory(&memory);
    printf("PASS: fibers: %u switches keep registers, FP control, __stack_pointer and depth; a fiber "
           "from a fiber; a large frame; a backtrace; 64 slots, reuse, ranges\n",switches);
    return 0;
}
