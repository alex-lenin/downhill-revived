/* Isolated unclipped loop feasibility test. Contains no game instruction data. */
#define PS2_VU_GEOMETRY_BENCH
#include "vu0_interp.c"
#include "bench_vu1_geometry_kernel.h"
#include <windows.h>

typedef struct { unsigned header[4]; ps2_cpu cpu; uint32_t code[4096]; ps2_q128 data[1024]; } GeometryCapture;
static Vu initial;
static ps2_cpu initial_cpu, work_cpu;
static ps2_q128 initial_memory[1024], memory[1024], exact_memory[1024];
static int captured;
int vu_geometry_capture(void *state, unsigned pc)
{
    if (pc != 0x170 / 8) return 0;
    initial = *(Vu *)state;
    initial_cpu = *initial.c;
    memcpy(initial_memory, initial.c->vu0_mem, sizeof initial_memory);
    captured = 1;
    return 1;
}
static void reset(Vu *u)
{
    *u = initial; work_cpu = initial_cpu;
    memcpy(memory, initial_memory, sizeof memory);
    u->c = &work_cpu; work_cpu.vu0_mem = memory;
}
static unsigned aot_loop(Vu *u)
{
    unsigned pc = 0x170 / 8, pairs = 0;
    while (pc != 0x260 / 8 && pairs < 4096) {
        if (pc < 0x170/8 || pc >= 0x260/8) {
            fprintf(stderr, "unexpected path %04x\n", pc * 8); return 0;
        }
        const uint32_t *words = (const uint32_t *)(u->c->vu0_code + pc * 8);
        const AotBlock *b = aot_find(pc, words, 4096 - pairs, 0);
        if (!b) { fprintf(stderr, "missing AOT block %04x\n", pc * 8); return 0; }
        int bad = 0, next = b->run(u, pc, &bad);
        if (bad || b->fused == 2) return 0;
        pairs += b->count; u->cycle++;
        pc = next >= 0 ? next : pc + b->count;
    }
    drain_flags(u); update_q(u);
    return pairs;
}
static int compare(const char *label, const void *a, const void *b, size_t n)
{
    const unsigned char *x=a, *y=b;
    for (size_t i=0;i<n;i++) if(x[i]!=y[i]) {
        printf("%s DIFF byte %u: %02x != %02x\n",label,(unsigned)i,x[i],y[i]);return 1;
    }
    printf("%s MATCH\n",label);return 0;
}
int main(int argc, char **argv)
{
    if (argc != 3) return 2;
    unsigned reps = strtoul(argv[2],NULL,10); if(!reps) return 2;
    GeometryCapture cap;
    FILE *f=fopen(argv[1],"rb"); if(!f) return 2;
    int valid=fread(&cap,sizeof cap,1,f)==1 && fgetc(f)==EOF && cap.header[3]==sizeof(ps2_cpu);
    fclose(f); if(!valid || cap.header[0]!=0x1040 || cap.cpu.vi[13]!=0x11) return 2;
    void *mapped=VirtualAlloc((void*)0x11000000,0x10000,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
    if(mapped!=(void*)0x11000000) return 2;
    uint8_t *code=(uint8_t*)mapped+0x8000;
    memcpy(code,cap.code,sizeof cap.code);ps2_vu1_code_version++;
    work_cpu=cap.cpu;memcpy(memory,cap.data,sizeof memory);
    work_cpu.vu0_code=code;work_cpu.vu0_mem=memory;
    if(ps2_vu1_run(&work_cpu,cap.header[0],cap.header[1],cap.header[2],NULL,NULL)||!captured) return 2;
    /* Widen the clip volume only. No microcode or AOT block is changed. */
    initial_cpu.vf[13].f[3]=FLT_MAX;
    unsigned vertices=((initial_cpu.vi[10]-initial_cpu.vi[8])&65535)/3;
    if(!vertices) return 2;
    printf("unclipped fixture: %u vertices, input VI8=%u VI10=%u\n",vertices,initial_cpu.vi[8],initial_cpu.vi[10]);
    unsigned csr=_mm_getcsr();_mm_setcsr((csr&~0x603f)|0xffc0);
    Vu u; reset(&u);unsigned expected=aot_loop(&u);
    if(!expected) return 1;
    ps2_cpu exact=work_cpu;memcpy(exact_memory,memory,sizeof memory);
    unsigned exact_flags[3]={u.mac,u.status,u.clip};
    reset(&u); unsigned actual=native_geometry_loop(&u);
    if(!actual) { fprintf(stderr,"native did not complete\n");return 1; }
    unsigned native_flags[3]={u.mac,u.status,u.clip};
    int differences=compare("CPU",&exact,&work_cpu,sizeof exact);
    differences+=compare("memory",exact_memory,memory,sizeof memory);
    differences+=compare("flags",exact_flags,native_flags,sizeof exact_flags);
    printf("pairs AOT=%u native=%u; native coverage=100%% (%u/%u vertices), state sections differing=%d\n",expected,actual,vertices,vertices,differences);
    LARGE_INTEGER freq;QueryPerformanceFrequency(&freq);
    for(unsigned trial=0;trial<3;trial++) for(unsigned mode=0;mode<2;mode++) {
        uint64_t ticks=0;unsigned complete=0;
        for(unsigned i=0;i<reps;i++) {
            reset(&u);LARGE_INTEGER a,b;QueryPerformanceCounter(&a);
            unsigned n=mode?native_geometry_loop(&u):aot_loop(&u);
            QueryPerformanceCounter(&b);ticks+=b.QuadPart-a.QuadPart;complete+=n!=0;
        }
        printf("trial=%u mode=%s calls=%u completed=%u ns_per_vertex=%.3f\n",trial,mode?"native":"aot",reps,complete,1e9*(double)ticks/freq.QuadPart/reps/vertices);
        if(complete!=reps)return 1;
    }
    _mm_setcsr(csr);
    return 0; /* Feasibility result, not an exactness certification. */
}
