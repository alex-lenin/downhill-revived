#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#include <windows.h>
#endif
#if defined(__x86_64__) || defined(_M_X64)
#include <xmmintrin.h>
#endif
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"
#include "wasm-rt-impl.h"
#include "core_host.h"
#include "memory_access.h"
#include "memory_layout.h"

#define CHECK(x) do { if (!(x)) { fprintf(stderr,"FAIL line %d: %s\n",__LINE__,#x); exit(1); } } while (0)
struct w2c_env { w2c_core *core; };
static w2c_core core;
static HostFiber root, threads[3];
static unsigned probes[3], switches;
static uint32_t minimum[3];
#if defined(__x86_64__) || defined(_M_X64)
static unsigned fp_bits(void) { return _mm_getcsr() & ~63u; }
#define FP_EE_EXPECTED 0xFFC0u
#define FP_IEEE_EXPECTED 0x1F80u
#elif defined(__aarch64__)
static unsigned fp_bits(void) {
    uint64_t f; __asm__ volatile("mrs %0, fpcr" : "=r"(f)); return (unsigned)f;
}
static uint64_t fp_raw(void) {
    uint64_t f; __asm__ volatile("mrs %0, fpcr" : "=r"(f)); return f;
}
/* FPCR.RMode=RZ, FZ=1 (fp_mode.c); IEEE entry clears the register. */
#define FP_EE_EXPECTED 0x01C00000u
#define FP_IEEE_EXPECTED 0u
#else
#error Implement the FP checks for this architecture
#endif
void w2c_env_host_fp_mode(struct w2c_env *env,u32 mode) { (void)env; host_fp_mode(mode); }
void w2c_env_yield(struct w2c_env *env,u32 id) {
    (void)env; CHECK(id<=3);
    uint32_t sp=*w2c_core_0x5F_stack_pointer(&core), depth=wasm_rt_call_stack_depth;
    unsigned mode=fp_bits();
    ++switches; host_fiber_switch(id==3?root:threads[id]);
    CHECK(*w2c_core_0x5F_stack_pointer(&core)==sp);
    CHECK(wasm_rt_call_stack_depth==depth);
    CHECK(fp_bits()==mode);
}
void w2c_env_probe(struct w2c_env *env,u32 id,u32 address,u32 depth) {
    (void)env; CHECK(id<3 && depth<=180);
    uint32_t top=host64_fiber_stack_top(threads[id]);
    CHECK(host_fiber_current()==threads[id]);
    CHECK(address>=top-0x40000+HOST64_PAGE && address+128<=top); /* above the guard page */
    CHECK(wasm_rt_call_stack_depth==182-depth);
    CHECK(fp_bits()==(id==1?FP_IEEE_EXPECTED:FP_EE_EXPECTED));
    if (address<minimum[id]) minimum[id]=address;
    probes[id]++;
}
static void entry(void *arg) { w2c_core_thread(&core,(u32)(uintptr_t)arg); }
static void noop(void *arg) { (void)arg; }
__attribute__((noinline)) static uint32_t native_float(unsigned mode,unsigned op,uint32_t a,uint32_t b) {
    union { uint32_t u; float f; } x={a},y={b},z;
    host_fp_mode(mode); z.f=op?x.f*y.f:x.f/y.f; return z.u;
}
static const uint64_t windows[][2]={{0x0a000000,0x1000},{0x2a000000,0x1000},{0x3a000000,0x1000},{0x37fff000,0x1000},
        {0x70000000,0x4000},{0x10000000,0x10000},{0x11000000,0x10000},{0x12000000,0x2000}};
static void commit_windows(void) {
    for (unsigned i=0;i<sizeof windows/sizeof windows[0];i++)
        CHECK(host64_memory_commit(windows[i][0],windows[i][1]));
}
#ifdef _WIN32
static void check_protection(uint64_t offset,int committed) {
    MEMORY_BASIC_INFORMATION info;
    CHECK(VirtualQuery((void *)(guest_base+offset),&info,sizeof info)==sizeof info);
    CHECK(info.State==(committed?MEM_COMMIT:MEM_RESERVE));
    if (committed) CHECK(info.Protect==PAGE_READWRITE);
}
#else
/* Independent check against the kernel's view: reserved pages read "---",
   committed private pages "rw-", RAM aliases "rw-s". */
static void check_protection(uint64_t offset,int committed) {
    FILE *maps=fopen("/proc/self/maps","r"); CHECK(maps);
    char line[512]; unsigned long long start,end; char perms[5];
    int found=0;
    while (fgets(line,sizeof line,maps)) {
        if (sscanf(line,"%llx-%llx %4s",&start,&end,perms)==3 &&
            start<=guest_base+offset && guest_base+offset<end) {
            found=1;
            if (committed) CHECK(perms[0]=='r' && perms[1]=='w');
            else CHECK(perms[0]=='-' && perms[1]=='-');
            break;
        }
    }
    fclose(maps); CHECK(found);
}
#endif
static void bounds(uint32_t address,unsigned store) {
    /* No wasm exception module is linked; trap() longjmps to this buffer. */
    WASM_RT_SAVE_STACK_DEPTH();
    wasm_rt_trap_t trap=WASM_RT_SETJMP(g_wasm_rt_jmp_buf);
    if (trap==WASM_RT_TRAP_NONE) {
        (void)w2c_core_out_of_bounds(&core,address,store);
        CHECK(0);
    }
    CHECK(trap==WASM_RT_TRAP_OOB && wasm_rt_call_stack_depth==0);
}
static void floats(void) {
    static const struct {unsigned op,a,b,rn,rz;} cases[]={
        {0,0x3f800000,0x40400000,0x3eaaaaab,0x3eaaaaaa},
        {0,0xbf800000,0x40400000,0xbeaaaaab,0xbeaaaaaa},
        {1,0x00800000,0x3f000000,0x00400000,0}, /* FTZ output */
        {1,0x00000001,0x4b000000,0x00800000,0}, /* DAZ input */
        {1,0x80800000,0x3f000000,0x80400000,0x80000000},
    };
    for (unsigned i=0;i<sizeof cases/sizeof cases[0];i++) for (unsigned mode=0;mode<2;mode++) {
        uint32_t expected=native_float(mode,cases[i].op,cases[i].a,cases[i].b);
        CHECK(expected==(mode?cases[i].rz:cases[i].rn));
        uint32_t got=w2c_core_float_test(&core,mode,cases[i].op,cases[i].a,cases[i].b);
        CHECK(got==expected);
        printf("float case %u mode %u: %08x\n",i,mode,got);
    }
    host_fp_mode(0); CHECK(fp_bits()==FP_IEEE_EXPECTED);
#if defined(__x86_64__) || defined(_M_X64)
    _mm_setcsr(0x1fa1); host_fp_mode(1); CHECK(_mm_getcsr()==0xffe1);
#else
    { uint64_t f=0x1fa1; __asm__ volatile("msr fpcr, %0\n\tisb" :: "r"(f) : "memory"); }
    host_fp_mode(1); CHECK(fp_raw()==0x01C00000ull);
#endif
    host_fp_mode(0);
}
int main(int argc,char **argv) {
#ifdef _WIN32
    SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);
#endif
    if (argc==3 && !strcmp(argv[1],"--guard")) {
        /* Isolate deliberate access violations; no WABT trap handler is installed. */
        wasm_rt_memory_t memory={0};
        wasm_rt_allocate_memory(&memory,32768,32770,false,65536);
        volatile unsigned char byte=*(volatile unsigned char *)(guest_base+strtoull(argv[2],NULL,0));
        (void)byte;
        return 1;
    }
    struct w2c_env env={&core};
    wasm_rt_init(); wasm2c_core_instantiate(&core,&env);
    wasm_rt_memory_t *m=w2c_core_memory(&core);
    CHECK((uintptr_t)m->data==guest_base && m->pages==32768);
    /* Exhaustive window edges plus unrelated hardware/module/stack ranges. */
    static const uint64_t unchanged[]={0,0x10000,0x06000000,0x06ffffff,
        0x08000000,0x09ffffff,0x0a000000,0x10000000,0x11000000,0x12000000,
        0x27ffffff,0x2a000000,0x37ffffff,0x3a000000,0x70000000,
        0xffffffffull,0x128000000ull};
    for (unsigned i=0;i<sizeof unchanged/sizeof unchanged[0];i++)
        CHECK(host64_memory_address(m->data,unchanged[i],1)==m->data+unchanged[i]);
    for (unsigned mirror=0x20000000;mirror<=0x30000000;mirror+=0x10000000)
        for (unsigned offset=0;offset<0x2000000;offset+=4096)
            CHECK(host64_memory_address(m->data,0x08000000ull+mirror+offset,4)==
                  m->data+0x08000000+offset);
    CHECK(host64_memory_address(m->data,0x29ffffff,2)==m->data+0x29ffffff);
    CHECK(host64_memory_address(m->data,0x39fffff0,32)==m->data+0x39fffff0);
    /* The access adapter (MEM_ADDR_MEMOP, sizes 1..16) gives the full adapter's address around
       every 4-MiB edge up to 8 GiB: the window edges, the low-view edges and the mod-64 window
       wrap beyond 2 GiB. */
    for (uint64_t edge=0;edge<=0x200000000ull;edge+=0x400000)
        for (uint64_t a=edge<32?0:edge-32;a<=edge+32;a++)
            for (uint64_t size=1;size<=16;size*=2)
                CHECK(host64_access_address(m->data,a,size)==host64_memory_address(m->data,a,size));
    /* The call-site split: host64_base_ok(base, offset) only where the plain address is the
       adapter's, for every offset shape (small, at the 32-MiB limit, larger, above the views);
       the cold arm always gives the adapter's address. */
    static const uint64_t offsets[]={0,4,0x1ffff0,0x1fffff0,0x1fffff1,0x2000000,0x10000000,
        0x39fffff0,0x3a000000,0x70100000,0xffffffffull};
    for (uint64_t edge=0;edge<=0x100000000ull;edge+=0x400000)
        for (uint64_t v=edge<32?0:edge-32;v<=edge+32 && v<=0xffffffffull;v++)
            for (unsigned k=0;k<sizeof offsets/sizeof offsets[0];k++)
                for (uint64_t size=1;size<=16;size*=2) {
                    uint64_t a=v+offsets[k];
                    if (host64_base_ok((uint32_t)v,offsets[k]))
                        CHECK(host64_memory_address(m->data,a,size)==m->data+a);
                    CHECK(host64_cold_address(m->data,a,size)==host64_memory_address(m->data,a,size));
                }
    /* Committed: only the module's own memory, its --stack-first shadow stack
       above the null page, and the RAM views. */
    CHECK(*w2c_core_0x5F_stack_pointer(&core)==0x100000);
    CHECK(host64_memory_commit_stack(*w2c_core_0x5F_stack_pointer(&core)));
    check_protection(HOST64_NULL_GUARD,1); check_protection(0xff000,1);
    static const uint64_t reserved[]={0,HOST64_NULL_GUARD-1,0x100000,0x003ff000,0x02000000,0x06000000,0x06ffffff,
        0x07000000,0x0a000000,0x10000000,0x11000000,0x12000000,0x13000000,0x3a000000,
        0x70000000,0x70004000,0x700ff000};
    for (unsigned i=0;i<sizeof reserved/sizeof reserved[0];i++) check_protection(reserved[i],0);
    check_protection(0x08000000,1); check_protection(0x39fff000,1); check_protection(0x70100000,1);
    /* The windows this synthetic module touches, as a module commits them (host_mem_commit). */
    commit_windows();
    CHECK(!host64_memory_commit(0x7ffff000,0x2000)); /* beyond the memory's size */
    CHECK(w2c_core_memory_test(&core)==0);
    CHECK(*(uint32_t *)(guest_base+0x08000100)==33);
    CHECK(w2c_core_table_call(&core,0,9)==26);
    CHECK(w2c_core_table_call(&core,1,9)==27);
    for (unsigned i=0;i<7;i++) {
        static const uint32_t addresses[]={0x0a000000,0x3a000000,0x10000000,0x11000000,0x12000000,0x70000000,0x7fffffff};
        check_protection(addresses[i],1);
    }
    check_protection(0x80000000ull,0); check_protection(0x100000000ull,0);
    check_protection(0x1ffff0000ull,0);
    CHECK(wasm_rt_grow_memory(m,0)==32768);
    CHECK(wasm_rt_grow_memory(m,1)==32768);
    check_protection(0x80000000ull,1);
    CHECK(*(uint32_t *)(guest_base+0x80000000ull)==0);
    *(uint32_t *)(guest_base+0x80000000ull)=123;
    CHECK(wasm_rt_grow_memory(m,2)==UINT32_MAX && m->pages==32769);
    CHECK(wasm_rt_grow_memory(m,1)==32769);
    check_protection(0x80020000ull,0);
    bounds(0x80020000,0); bounds(0x80020000,1);
    bounds(0xffffffff,0); bounds(0xffffffff,1);
    puts("PASS: 4 generated-module OOB reads/writes trap after size inference");
    floats();
    host64_fiber_bind(w2c_core_0x5F_stack_pointer(&core)); root=host_fiber_main();
    uint32_t root_sp=*w2c_core_0x5F_stack_pointer(&core);
    for (unsigned i=0;i<3;i++) {
        threads[i]=host_fiber_create(entry,(void *)(uintptr_t)i); CHECK(threads[i]==i+1);
        CHECK(host64_fiber_stack_top(threads[i])==0x06040000+i*0x40000);
        check_protection(0x06000000+i*0x40000+HOST64_PAGE-1,0); check_protection(0x06000000+i*0x40000+HOST64_PAGE,1);
        minimum[i]=UINT32_MAX;
    }
    host_fiber_switch(threads[0]);
    CHECK(wasm_rt_call_stack_depth==0 && *w2c_core_0x5F_stack_pointer(&core)==root_sp);
    CHECK(fp_bits()==FP_IEEE_EXPECTED);
    for (unsigned id=0;id<3;id++) {
        uint32_t expected=0;
        for (unsigned d=0;d<=180;d++) for (unsigned i=0;i<32;i++) expected+=id*100000+d*32+i;
        expected*=20;
        CHECK(w2c_core_result(&core,id)==expected && probes[id]==20*181);
        printf("fiber %u: checksum %08x, probes %u, lowest local %08x\n",id,expected,probes[id],minimum[id]);
    }
    CHECK(switches==63);
    uintptr_t (*ranges)[2]; CHECK(host_stack_ranges(&ranges)==3);
    CHECK(ranges[0][0]==0x06000000 && ranges[2][1]==0x060c0000);
    host_fiber_delete(threads[1]);
    CHECK(host_fiber_create(noop,NULL)==threads[1]);
    for (unsigned i=0;i<3;i++) host_fiber_delete(threads[i]);
    HostFiber slots[64];
    for (unsigned i=0;i<64;i++) {
        slots[i]=host_fiber_create(noop,NULL); CHECK(slots[i]==i+1);
        CHECK(host64_fiber_stack_top(slots[i])==0x06040000+i*0x40000);
    }
    CHECK(host_fiber_create(noop,NULL)==HOST_FIBER_NONE);
    CHECK(host_stack_ranges(&ranges)==64 && ranges[63][1]==0x07000000);
    host_fiber_delete(slots[0]); CHECK(host_fiber_create(noop,NULL)==slots[0]);
    host64_fiber_shutdown();
    wasm2c_core_free(&core); CHECK(guest_base==0);
    /* Reinstantiation must release every reservation, including the guard. */
    wasm2c_core_instantiate(&core,&env); check_protection(0x10000000,0);
    CHECK(host64_memory_commit_stack(*w2c_core_0x5F_stack_pointer(&core)));
    commit_windows(); CHECK(w2c_core_memory_test(&core)==0);
    wasm2c_core_free(&core); wasm_rt_free();
    puts("PASS: mirrors, hardware memory, growth, guards, table, float bits, 3 fibers, cleanup");
    return 0;
}
