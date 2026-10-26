/* Windows fibers preserve the native C frames; wasm locals in linear memory
   also need __stack_pointer swapped. POSIX hosts use fiber_posix.c (a small
   asm context switch, same slot rule and wasm globals). */
#define _WIN32_WINNT 0x0A00
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include "wasm-rt.h"
#include "core_host.h"
#if !WASM_RT_STACK_DEPTH_COUNT
#error Build all wasm2c runtime units with WASM_RT_STACK_DEPTH_COUNT=1
#endif
/* Each slot's lowest page stays reserved: an overflow faults (guest_memory.c's
   report) instead of running into the slot below. */
enum { SLOTS=64, ROOT=65, STACK_BASE=0x06000000, STACK_BYTES=0x40000, GUARD=0x1000 };
typedef struct {
    void *native; void (*fn)(void *); void *arg;
    uint32_t sp, depth;
} Fiber;
static Fiber fibers[ROOT+1];
static uint32_t *module_sp;
static int current, converted;
static DWORD owner_thread;
static void check_thread(void) { if (owner_thread!=GetCurrentThreadId()) abort(); }
void host64_fiber_bind(uint32_t *sp) {
    if (current || !sp) abort();
    module_sp=sp; owner_thread=GetCurrentThreadId();
}
HostFiber host_fiber_main(void) {
    check_thread(); if (!module_sp) abort(); if (current) return current;
    converted=!IsThreadAFiber();
    fibers[ROOT].native=converted?ConvertThreadToFiberEx(NULL,FIBER_FLAG_FLOAT_SWITCH):GetCurrentFiber();
    if (!fibers[ROOT].native) abort();
    current=ROOT; return ROOT;
}
static VOID CALLBACK start(void *p) {
    Fiber *f=p; f->fn(f->arg);
    /* EE wrappers normally never return. Returning here resumes the root. */
    host_fiber_switch(ROOT); abort();
}
HostFiber host_fiber_create(void (*fn)(void *),void *arg) {
    check_thread(); if (!current || !fn) return 0;
    for (int i=1;i<=SLOTS;i++) if (!fibers[i].native) {
        Fiber *f=&fibers[i]; f->fn=fn; f->arg=arg;
        f->sp=STACK_BASE+i*STACK_BYTES; f->depth=0;
        if (!host64_memory_commit(f->sp-STACK_BYTES+GUARD,STACK_BYTES-GUARD)) return 0;
        memset((void *)(guest_base+f->sp-STACK_BYTES+GUARD),0,STACK_BYTES-GUARD);
        f->native=CreateFiberEx(65536,4*1024*1024,FIBER_FLAG_FLOAT_SWITCH,start,f);
        return f->native?i:0;
    }
    return 0;
}
void host_fiber_switch(HostFiber to) {
    check_thread(); if (!current || to<1 || to>ROOT || !fibers[to].native) abort();
    if (current==to) return;
    fibers[current].sp=*module_sp; fibers[current].depth=wasm_rt_call_stack_depth;
    *module_sp=fibers[to].sp; wasm_rt_call_stack_depth=fibers[to].depth;
    current=to; SwitchToFiber(fibers[to].native);
}
void host_fiber_delete(HostFiber id) {
    check_thread(); if (id<1 || id>SLOTS || id==current) abort();
    if (fibers[id].native) DeleteFiber(fibers[id].native);
    memset(&fibers[id],0,sizeof fibers[id]);
}
HostFiber host_fiber_current(void) { check_thread(); return current; }
int host64_fiber_id(void) { return current; }  /* diagnostics: any thread */
uint32_t host64_fiber_stack_top(HostFiber id) {
    return id>=1 && id<=SLOTS ? STACK_BASE+id*STACK_BYTES : 0;
}
int host_stack_ranges(uintptr_t (**ranges)[2]) {
    static uintptr_t result[SLOTS][2]; int n=0; check_thread();
    for (int i=1;i<=SLOTS;i++) if (fibers[i].native) {
        result[n][0]=STACK_BASE+(i-1)*STACK_BYTES;
        result[n++][1]=STACK_BASE+i*STACK_BYTES;
    }
    *ranges=result; return n;
}
void host64_fiber_shutdown(void) {
    check_thread(); if (current!=ROOT) abort();
    for (int i=1;i<=SLOTS;i++) host_fiber_delete(i);
    if (converted && !ConvertFiberToThread()) abort();
    memset(fibers,0,sizeof fibers); current=0; module_sp=NULL; owner_thread=0;
}
