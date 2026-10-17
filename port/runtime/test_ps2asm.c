/* Tests for the inline asm runtime: every template the game uses must parse (generated list from
   port/tools/test_runtime.py), and the common shapes must compute what the PS2 computes. */
#include "ps2asm.h"
#include "ps2float.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xmmintrin.h>

typedef struct {
    void *p;
    unsigned int size;
} ps2_op;
void ps2_asm(const char *tmpl, const char *constraints, int n, ps2_op *ops);

#define OP(x) {(void *)&(x), sizeof(x)}

static int failures;
static unsigned sync_calls;
static void graphics_sync(void) { sync_calls++; }
static unsigned fifo_calls;
static void fifo_write(uintptr_t address, uint64_t value, unsigned size)
{
    failures += address != 0x10005000 || size != 16 || !sync_calls;
    failures += value != ps2.gpr[2].d[0] || memcmp((void *)address, &ps2.gpr[2], 16) != 0;
    fifo_calls++;
}

static void expect(const char *what, double got, double want)
{
    if (fabs(got - want) > 1e-5 * (fabs(want) > 1 ? fabs(want) : 1)) {
        printf("FAIL %s: got %.9g, want %.9g\n", what, got, want);
        failures++;
    }
}

static void expect_bits(const char *what, float got, uint32_t want)
{
    uint32_t bits;
    memcpy(&bits, &got, 4);
    if (bits != want) {
        printf("FAIL %s: %08X != %08X\n", what, bits, want);
        failures++;
    }
}

static void exceptional_macro(void)
{
    /* Execute the real EE/VU macro interpreter with host FP faults unmasked. */
    unsigned control = _mm_getcsr();
    ps2_cpu saved = ps2;
    memset(&ps2, 0, sizeof ps2);
    ps2.vf[1].w[0] = 0x7f7fffff; ps2.vf[2].f[0] = 2;
    ps2.vf[1].w[1] = 0x7fc00000; ps2.vf[2].f[1] = 0;
    ps2.vf[1].w[2] = 0x80800000; ps2.vf[2].f[2] = 0.5f;
    _mm_setcsr(control & ~(0x680u | 63u));
    ps2_asm("vmul.xyz $vf3,$vf1,$vf2", "", 0, NULL);
    expect_bits("VU mul overflow", ps2.vf[3].f[0], 0x7f7fffff);
    expect_bits("VU raw exp255 times zero", ps2.vf[3].f[1], 0);
    expect_bits("VU negative underflow", ps2.vf[3].f[2], 0x80000000);
    ps2.vacc[0] = -ps2.vf[1].f[0];
    ps2_asm("vmadd.x $vf3,$vf1,$vf2", "", 0, NULL);
    expect_bits("VU MADD overflowing product", ps2.vf[3].f[0], 0x7f7fffff);
    ps2.vf[2].w[0] = 0x80000001;
    ps2_asm("vdiv Q,$vf1x,$vf2x", "", 0, NULL);
    expect_bits("VU denormal signed divisor", ps2.q, 0xff7fffff);
    ps2.vf[1].w[0] = 0;
    ps2_asm("vrsqrt Q,$vf1x,$vf2x", "", 0, NULL);
    expect_bits("VU RSQRT signed zero/zero", ps2.q, 0x80000000);
    ps2.vf[2].f[0] = -4;
    ps2_asm("vsqrt Q,$vf2x", "", 0, NULL);
    expect_bits("VU negative SQRT", ps2.q, 0x40000000);
    ps2.fpr[1] = 0; ps2.fpr[2] = 0;
    ps2_asm("div.s $f3,$f1,$f2", "", 0, NULL);
    expect_bits("EE zero/zero", ps2.fpr[3], 0x7f7fffff);
    ps2.fpr[1] = ps2.vf[3].f[0]; ps2.fpr[2] = 2;
    ps2_asm("mul.s $f3,$f1,$f2", "", 0, NULL);
    expect_bits("EE mul overflow", ps2.fpr[3], 0x7f7fffff);
    ps2.vf[1].w[0] = 0xc4ab4401; ps2.vf[2].w[0] = 0xffffffff;
    ps2.vf[1].w[1] = 0; ps2.vf[2].w[1] = 0x80000000;
    ps2.vf[1].w[2] = 0x00000001; ps2.vf[2].w[2] = 0;
    ps2.vf[1].w[3] = 0x7fa12345; ps2.vf[2].w[3] = 0x3f800000;
    ps2_asm("vmini.xyzw $vf3,$vf1,$vf2\n\tvmax.xyzw $vf4,$vf1,$vf2", "", 0, NULL);
    expect_bits("VU MINI raw negative exp255", ps2.vf[3].f[0], 0xffffffff);
    expect_bits("VU MAX negative exp255", ps2.vf[4].f[0], 0xc4ab4401);
    expect_bits("VU MINI signed zero", ps2.vf[3].f[1], 0x80000000);
    expect_bits("VU MAX signed zero", ps2.vf[4].f[1], 0);
    expect_bits("VU MAX retains denormal", ps2.vf[4].f[2], 1);
    expect_bits("VU MAX raw positive exp255", ps2.vf[4].f[3], 0x7fa12345);
    memcpy(&ps2.fpr[1], &ps2.vf[1].w[0], 4);
    memcpy(&ps2.fpr[2], &ps2.vf[2].w[0], 4);
    ps2_asm("min.s $f3,$f1,$f2\n\tmax.s $f4,$f1,$f2", "", 0, NULL);
    expect_bits("EE MIN raw negative exp255", ps2.fpr[3], 0xffffffff);
    expect_bits("EE MAX raw negative exp255", ps2.fpr[4], 0xc4ab4401);
    {   /* Same pairs as test_vu1.c: VU0 macro MIN/MAX equals the VU1 lane ordering. */
        static const uint32_t order[] = {0x7f800000, 0xff800000, 0x7fffffff, 0xffffffff, 0x7f7fffff,
                                         0xff7fffff, 0x3f800000, 0xbf800000, 0, 0x80000000, 1, 0x80000001};
        for (unsigned i = 0; i < 12; i++)
            for (unsigned j = 0; j < 12; j += 4) {
                for (unsigned c = 0; c < 4; c++) { ps2.vf[1].w[c] = order[i]; ps2.vf[2].w[c] = order[j + c]; }
                ps2_asm("vmax.xyzw $vf3,$vf1,$vf2\n\tvmini.xyzw $vf4,$vf1,$vf2", "", 0, NULL);
                for (unsigned c = 0; c < 4; c++) {
                    float hi = ps2_float_minmax(ps2.vf[1].f[c], ps2.vf[2].f[c], 1);
                    float lo = ps2_float_minmax(ps2.vf[1].f[c], ps2.vf[2].f[c], 0);
                    uint32_t h, l; memcpy(&h, &hi, 4); memcpy(&l, &lo, 4);
                    expect_bits("VU0 macro MAX order", ps2.vf[3].f[c], h);
                    expect_bits("VU0 macro MINI order", ps2.vf[4].f[c], l);
                }
            }
        /* Exponent 255 is the largest magnitude, never a NaN. */
        ps2.vf[1].w[0] = 0x7f800000; ps2.vf[2].w[0] = 0x7f7fffff;
        ps2.vf[1].w[1] = 0xff800000; ps2.vf[2].w[1] = 0xff7fffff;
        ps2.vf[1].w[2] = 0x7fffffff; ps2.vf[2].w[2] = 0x7f800000;
        ps2.vf[1].w[3] = 0xffffffff; ps2.vf[2].w[3] = 0x3f800000;
        ps2_asm("vmax.xyzw $vf3,$vf1,$vf2\n\tvmini.xyzw $vf4,$vf1,$vf2", "", 0, NULL);
        expect_bits("VU MAX 7f800000", ps2.vf[3].f[0], 0x7f800000);
        expect_bits("VU MINI ff800000", ps2.vf[4].f[1], 0xff800000);
        expect_bits("VU MAX 7fffffff", ps2.vf[3].f[2], 0x7fffffff);
        expect_bits("VU MINI ffffffff", ps2.vf[4].f[3], 0xffffffff);
    }
    _mm_setcsr(control);
    ps2 = saved;
}

typedef struct {
    const char *tmpl, *cons;
    int n;
} Site;

static const Site sites[] = {
#include "all_templates.inc"
};

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) Vec;

/* The VU0 microcode package of .vutext (finding 019: the DMA chain at 0x26CF80, a cnt tag then VIF
   MPG packets) into micro memory; DHD_VUTEXT names assets/vutext.bin. 0 if it isn't there. */
static int load_vu0_microcode(void)
{
    static unsigned char data[0x10000];
    const char *path = getenv("DHD_VUTEXT");
    FILE *f = path ? fopen(path, "rb") : NULL;
    size_t n, p;
    unsigned qwc;
    if (!f)
        return 0;
    n = fread(data, 1, sizeof data, f);
    fclose(f);
    p = 0x26CF80 - 0x263BC0;
    if (p + 16 > n)
        return 0;
    qwc = data[p] | data[p + 1] << 8;
    for (p += 8; p + 4 <= 0x26CF80 - 0x263BC0 + 16 + qwc * 16u && p + 4 <= n;) {
        unsigned code = data[p] | data[p + 1] << 8 | data[p + 2] << 16 | (unsigned)data[p + 3] << 24;
        unsigned num = (code >> 16 & 0xFF) ? (code >> 16 & 0xFF) : 256;
        p += 4;
        if ((code >> 24 & 0x7F) == 0x4A) {  /* MPG num instructions to micro address imm * 8 */
            memcpy(ps2.vu0_code + (code & 0xFFFF) * 8, data + p, num * 8);
            p += num * 8;
        }
    }
    return 1;
}

static uint32_t rng = 12345;
static float random_float(float range)
{
    rng = rng * 1103515245u + 12345u;
    return ((float)(rng >> 8) / 16777216.0f * 2 - 1) * range;
}

/* Native microprograms against the interpreter running the retail microcode: bit for bit on
   the registers the EE reads back. */
static void compare_interpreter(void)
{
    static const unsigned entries[] = {0x978, 0x830, 0x0c0};
    ps2_cpu before, native;
    int e, k, r, bad = 0;
    if (!load_vu0_microcode()) {
        printf("skip interpreter comparison: set DHD_VUTEXT to assets/vutext.bin\n");
        return;
    }
    for (e = 0; e < 3; e++)
        for (k = 0; k < 2000; k++) {
            for (r = 1; r < 32; r++) {
                float range = k < 1000 ? 4.0f : 400.0f;
                ps2.vf[r].f[0] = random_float(range);
                ps2.vf[r].f[1] = random_float(range);
                ps2.vf[r].f[2] = random_float(range);
                ps2.vf[r].f[3] = random_float(range);
            }
            memset(ps2.vi, 0, sizeof ps2.vi);
            before = ps2;
            /* distinct literals: ps2_asm keeps a parsed template by its address */
            ps2_asm(e == 0 ? "vcallms 0x978" : e == 1 ? "vcallms 0x830" : "vcallms 0xc0", "", 0, NULL);
            native = ps2;
            ps2 = before;
            if (ps2_vu0_run(&ps2, entries[e])) {
                printf("FAIL interpreter: 0x%x stopped\n", entries[e]);
                failures++;
                return;
            }
            if (entries[e] == 0x978)
                r = memcmp(&ps2.vf[14], &native.vf[14], 32);
            else if (entries[e] == 0x830)
                r = memcmp(&ps2.vf[10], &native.vf[10], 12) || memcmp(&ps2.vf[11], &native.vf[11], 12)
                    || memcmp(&ps2.vf[12], &native.vf[12], 12);
            else
                r = ps2.vi[2] != native.vi[2] || ps2.vi[5] != native.vi[5]
                    || memcmp(&ps2.vf[10].f[0], &native.vf[10].f[0], 4);
            if (r && bad++ < 5)
                printf("FAIL interpreter 0x%03x case %d: vf10 %08X/%08X vf14 %08X/%08X vi2 %X/%X vi5 %X/%X\n",
                       entries[e], k, ps2.vf[10].w[0], native.vf[10].w[0], ps2.vf[14].w[0], native.vf[14].w[0],
                       ps2.vi[2], native.vi[2], ps2.vi[5], native.vi[5]);
            ps2 = native;
        }
    if (bad) {
        printf("FAIL interpreter: %d of 6000 differ\n", bad);
        failures++;
    } else
        printf("interpreter matches the native microprograms bit for bit (6000 cases)\n");
}

static void exceptional_micro(ps2_cpu *cpu)
{
    volatile float one = 1.0f, zero = 0.0f;
    cpu->vf[3].f[0] = one / zero;
}

static int intc_reads;
static void count_intc_read(void) { intc_reads++; }

int main(void)
{
    size_t i;
    int unparsed = 0;
    ps2_vu0_init();
    {
        /* Recompiled code stores vf0 from memory (port/tools/recomp.py: sqc2 $vf0 as the last
           row of a matrix), so ps2.vf[0] must hold (0, 0, 0, 1) and survive writes to $vf0. */
        static float row[4] __attribute__((aligned(16)));
        ps2_exec("vadd.xyzw $vf0, $vf0, $vf0");
        ps2_exec("vmove.xyzw $vf0, $vf1");
        ps2_store128((uintptr_t)row, &ps2.vf[0]);
        expect("vf0 x", row[0], 0); expect("vf0 y", row[1], 0); expect("vf0 z", row[2], 0);
        expect("vf0 w", row[3], 1);
    }
    {
        /* Loads of INTC_STAT (sceGsSyncV's vblank wait) let the kernel run time; no other load does. */
        intc_reads = 0;
        ps2_intc_read = count_intc_read;
        (void)ps2_load_address(PS2_INTC_STAT);
        (void)ps2_load_address(PS2_INTC_STAT + 0x10);  /* INTC_MASK */
        (void)ps2_load_address((uintptr_t)&intc_reads);
        ps2_intc_read = NULL;
        expect("INTC_STAT loads reaching the kernel", intc_reads, 1);
    }
    {
        unsigned original = _mm_getcsr(), diagnostic = (original & ~0x6BFu);
        ps2_vu0_register_micro(0xFF8, exceptional_micro);
        _mm_setcsr(diagnostic);
        ps2_asm("vcallms 0xff8", "", 0, NULL);
        unsigned restored = _mm_getcsr();
        _mm_setcsr(original);
        if (restored != diagnostic || ps2.vf[3].w[0] != 0x7F800000u) return 1;
        /* A synthetic MUL overflow in the interpreted VU0 path. It rounds toward zero, so the
           overflow gives the largest float, as on the VU and in PCSX2 (VU0FPCR chop). */
        uint32_t code[4] = {0x8000033Cu,
            (1u << 30) | (15u << 21) | (2u << 16) | (1u << 11) | (3u << 6) | 0x2Au,
            0x8000033Cu, 0x000002FFu};
        ps2_cpu cpu = {0}; ps2_q128 data[256] = {0};
        cpu.vu0_code = (uint8_t *)code; cpu.vu0_mem = data;
        for (unsigned lane=0; lane<4; lane++) cpu.vf[1].w[lane] = cpu.vf[2].w[lane] = 0x7F7FFFFFu;
        _mm_setcsr(diagnostic);
        int result = ps2_vu0_run(&cpu, 0); restored = _mm_getcsr();
        _mm_setcsr(original);
        if (result || restored != diagnostic || cpu.vf[3].w[0] != 0x7F7FFFFFu) return 1;
    }
    {
        uintptr_t fenced[] = {0x11008000, 0x1100FFFF, 0x10003C00, 0x10005000, 0x10003020, 0x12001000};
        uintptr_t plain[] = {0x11004000, 0x11010000, 0x10004000, 0x10005010, 0x10003030, 0x12001100, 0x1234};
        ps2_graphics_sync = graphics_sync;
        for (i = 0; i < sizeof fenced / sizeof *fenced; i++)
            expect("fenced address", ps2_memory_address(fenced[i]), fenced[i]);
        expect("memory fences", sync_calls, 6);
        for (i = 0; i < sizeof plain / sizeof *plain; i++)
            expect("plain address", ps2_memory_address(plain[i]), plain[i]);
        expect("ordinary RAM does not fence", sync_calls, 6);
        ps2_exec("cfc2 $2,$vi29");
        expect("VPU status fence", sync_calls, 7);
        expect("VPU idle after fence", ps2.gpr[2].w[0], 0);
        ps2_exec("ctc2 $0,$vi28");
        expect("FBRST fence", sync_calls, 8);
        void *hw = VirtualAlloc((void *)0x10000000, 65536, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        if (hw != (void *)0x10000000) return 1;
        ps2_mmio_write = fifo_write;
        ps2.gpr[2].w[0] = 0x12345678; ps2.gpr[2].w[1] = 0x89abcdef;
        ps2.gpr[2].w[2] = 0x99887766; ps2.gpr[2].w[3] = 0x55443322;
        ps2.gpr[4].d[0] = 0x10005000;
        ps2_exec("sq $2,0($4)");
        expect("quadword FIFO notification", fifo_calls, 1);
        ps2.vf[1] = ps2.gpr[2];
        ps2_exec("sqc2 $vf1,0($4)");
        expect("VF quadword FIFO notification", fifo_calls, 2);
        ps2_mmio_write = NULL;
        VirtualFree(hw, 0, MEM_RELEASE);
        ps2_graphics_sync = NULL;
    }
    for (i = 0; i < sizeof sites / sizeof *sites; i++)
        if (ps2_asm_check(sites[i].tmpl, sites[i].cons, sites[i].n))
            unparsed++;
    printf("%d of %d game templates flagged (see messages above)\n", unparsed, (int)(sizeof sites / sizeof *sites));

    { /* dot product on the FPU accumulator */
        float d, ax = 1, ay = 2, az = 3, bx = 4, by = 5, bz = 6;
        ps2_op o[] = {OP(d), OP(ax), OP(bx), OP(ay), OP(by), OP(az), OP(bz)};
        ps2_asm("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6", "=f|f|f|f|f|f|f", 7, o);
        expect("dot", d, 32);
    }
    { /* 128-bit copy through a GPR */
        Vec src = {1, 2, 3, 4}, dst = {0, 0, 0, 0};
        Vec *ps = &src;
        ps2_op o[] = {OP(dst), OP(ps)};
        ps2_asm("lq $2, 0x0(%1)\n\tsq $2, %0", "=m|r", 2, o);
        expect("copy.x", dst.x, 1);
        expect("copy.w", dst.w, 4);
    }
    { /* quadword addresses ignore their low 4 bits, as on the EE and in recompiled code */
        Vec q[2] = {{1, 2, 3, 4}, {0, 0, 0, 0}};
        char *inside = (char *)&q[0] + 4;
        ps2_op o[] = {OP(inside)};
        ps2_asm("lqc2 $vf11, 0x0(%0)\n\tsqc2 $vf11, 0x10(%0)", "r", 1, o);
        expect("masked lqc2/sqc2.x", q[1].x, 1);
        expect("masked lqc2/sqc2.w", q[1].w, 4);
    }
    { /* vector subtract keeps w of the destination register's load */
        Vec a = {5, 7, 9, 1}, b = {1, 2, 3, 8}, out;
        Vec *pa = &a, *pb = &b;
        ps2_op o[] = {OP(out), OP(pa), OP(pb)};
        ps2_asm("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0",
                "=m|r|r", 3, o);
        expect("sub.x", out.x, 4);
        expect("sub.z", out.z, 6);
        expect("sub.w", out.w, 1);
    }
    { /* scale by a float through mfc1/qmtc2 and a broadcast */
        Vec a = {1, 2, 3, 9}, out;
        Vec *pa = &a;
        float s = 2.5f;
        ps2_op o[] = {OP(out), OP(pa), OP(s)};
        ps2_asm("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0",
                "=m|r|f", 3, o);
        expect("scale.y", out.y, 5);
        expect("scale.w", out.w, 9);
    }
    { /* cross product */
        Vec a = {1, 0, 0, 0}, b = {0, 1, 0, 0}, out;
        Vec *pa = &a, *pb = &b;
        ps2_op o[] = {OP(out), OP(pa), OP(pb)};
        ps2_asm("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0",
                "=m|r|r", 3, o);
        expect("cross.x", out.x, 0);
        expect("cross.z", out.z, 1);
    }
    { /* 1 / length with a scratch GPR operand */
        float inv, len, x = 3, y = 4, z = 0;
        int tmp;
        ps2_op o[] = {OP(inv), OP(len), OP(tmp), OP(x), OP(y), OP(z)};
        ps2_asm("mula.s %3,%3\n\tmadda.s %4,%4\n\tmadd.s %1,%5,%5\n\tsqrt.s %1,%1\n\tlui %2,0x3f80\n\tmtc1 %2,%0\n\tnop\n\tdiv.s %0,%0,%1",
                "=&f|=&f|=r|f|f|f", 6, o);
        expect("len", len, 5);
        expect("inv", inv, 0.2);
    }
    { /* rsqrt.s, min.s, max.s, abs.s, and the EE's divide by zero */
        float r, a = 6, b = 9, z = 0;
        ps2_op o[] = {OP(r), OP(a), OP(b)};
        ps2_op oz[] = {OP(r), OP(a), OP(z)};
        ps2_asm("rsqrt.s %0,%1,%2", "=f|f|f", 3, o);
        expect("rsqrt", r, 2);
        ps2_asm("min.s %0,%1,%2", "=f|f|f", 3, o);
        expect("min", r, 6);
        ps2_asm("max.s %0,%1,%2", "=f|f|f", 3, o);
        expect("max", r, 9);
        ps2_asm("div.s %0,%1,%2", "=f|f|f", 3, oz);
        expect("div0", r, 3.4028235e38);
    }
    { /* tied operand: "0" shares %0's register */
        float r, x = 2;
        ps2_op o[] = {OP(r), OP(x)};
        ps2_asm("abs.s %0,%0", "=f|0", 2, o);
        expect("tied", r, 2);
    }
    { /* libgcc umul_ppmm */
        unsigned int lo, hi, u = 0xFFFFFFFFu, v = 3;
        ps2_op o[] = {OP(lo), OP(hi), OP(u), OP(v)};
        ps2_asm("multu %2,%3", "=l|=h|d|d", 4, o);
        expect("multu.lo", lo, 0xFFFFFFFDu);
        expect("multu.hi", hi, 2);
    }
    { /* matrix * vector with ACC chains */
        Vec m[4] = {{1, 0, 0, 0}, {0, 2, 0, 0}, {0, 0, 3, 0}, {10, 20, 30, 1}};
        Vec v = {1, 1, 1, 1}, out;
        Vec *pm = m, *pv = &v;
        ps2_op o[] = {OP(out), OP(pv), OP(pm)};
        ps2_asm("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tlqc2 $vf13,16(%2)\n\tlqc2 $vf14,32(%2)\n\tlqc2 $vf15,48(%2)\n\t"
                "vmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddaz.xyz ACC,$vf14,$vf11z\n\t"
                "vmaddw.xyz $vf11,$vf15,$vf11w\n\tsqc2 $vf11,%0",
                "=m|r|r", 3, o);
        expect("mat.x", out.x, 11);
        expect("mat.y", out.y, 22);
        expect("mat.z", out.z, 33);
    }
    { /* VU0 random generator: values in [1, 2), deterministic from the seed */
        Vec seed = {0, 1.2345f, 0, 0}, r1, r2;
        Vec *ps = &seed;
        ps2_op o1[] = {OP(ps)};
        ps2_op o2[] = {OP(r1)};
        ps2_op o3[] = {OP(r2)};
        ps2_asm("lqc2 $vf28, 0(%0)\n\tvrinit R, $vf28y", "r", 1, o1);
        ps2_asm("vrnext.x $vf27, R\n\tsqc2 $vf27, %0", "=m", 1, o2);
        ps2_asm("vrnext.x $vf27, R\n\tsqc2 $vf27, %0", "=m", 1, o3);
        if (!(r1.x >= 1 && r1.x < 2 && r2.x >= 1 && r2.x < 2 && r1.x != r2.x)) {
            printf("FAIL rnext: %g %g\n", r1.x, r2.x);
            failures++;
        }
        ps2_asm("lqc2 $vf28, 0(%0)\n\tvrinit R, $vf28y", "r", 1, o1);
        ps2_asm("vrnext.x $vf27, R\n\tsqc2 $vf27, %0", "=m", 1, o3);
        expect("rnext.repeat", r2.x, r1.x);
    }
    { /* fixed point conversions */
        Vec a = {1.5f, -2.25f, 3, 0}, out;
        Vec *pa = &a;
        ps2_op o[] = {OP(out), OP(pa)};
        int bits[4];
        ps2_asm("lqc2 $vf11,0(%1)\n\tvftoi4.xyzw $vf11,$vf11\n\tsqc2 $vf11,%0", "=m|r", 2, o);
        memcpy(bits, &out, 16);
        expect("ftoi4.x", bits[0], 24);
        expect("ftoi4.y", bits[1], -36);
    }
    { /* asm reading function arguments is refused, not run */
        int before = ps2_asm_problems();
        ps2_asm("daddu $t1, $a0, $zero\n\tlq $v1, 0x0($t1)", "", 0, NULL);
        if (ps2_asm_problems() == before) {
            printf("FAIL abi: whole-function asm not flagged\n");
            failures++;
        }
    }
    { /* sin/cos microprogram: angles in vf14, sin back in vf14, cos in vf15 */
        static const float angles[] = {0, 0.5f, 1.5707964f, 2.5f, 3.1415927f, 4, 5.5f, 6.2f, -0.3f,
                                       -2, -4.5f, -7, 7, 13.4f, -100.25f, 1000};
        float worst = 0;
        for (i = 0; i < sizeof angles / sizeof *angles; i++) {
            Vec a = {angles[i], -angles[i], angles[i] * 0.5f, angles[i] + 1}, s, c;
            Vec *pa = &a;
            ps2_op o[] = {OP(s), OP(c), OP(pa)};
            int k;
            ps2_asm("lqc2 $vf14,0(%2)\n\tvcallms 0x978\n\tsqc2 $vf14,%0\n\tsqc2 $vf15,%1", "=m|=m|r", 3, o);
            for (k = 0; k < 4; k++) {
                /* reduction by a float 2pi: the error grows with the number of whole turns */
                float tol = 2e-5f + fabsf((&a.x)[k]) * 4e-8f;
                float e1 = fabsf((&s.x)[k] - (float)sin((&a.x)[k])) / tol;
                float e2 = fabsf((&c.x)[k] - (float)cos((&a.x)[k])) / tol;
                worst = e1 > worst ? e1 : worst;
                worst = e2 > worst ? e2 : worst;
            }
        }
        if (worst > 1) {
            printf("FAIL sincos: error %g x tolerance\n", worst);
            failures++;
        }
    }
    { /* rotation microprogram: rows vf10..vf12 must be orthonormal */
        Vec a = {0.3f, -1.2f, 2.1f, 0}, out[3];
        Vec *pa = &a, *po = out;
        ps2_op o[] = {OP(pa), OP(po)};
        int r, k;
        float worst = 0;
        memset(out, 0, sizeof out);
        ps2_asm("lqc2 $vf14,0(%0)\n\tvcallms 0x830\n\tsqc2 $vf10,0(%1)\n\tsqc2 $vf11,16(%1)\n\tsqc2 $vf12,32(%1)",
                "r|r", 2, o);
        for (r = 0; r < 3; r++)
            for (k = 0; k < 3; k++) {
                float d = (&out[r].x)[0] * (&out[k].x)[0] + (&out[r].x)[1] * (&out[k].x)[1] +
                          (&out[r].x)[2] * (&out[k].x)[2];
                float e = fabsf(d - (r == k));
                worst = e > worst ? e : worst;
            }
        if (worst > 1e-4f) {
            printf("FAIL rotation: orthonormality error %g\n", worst);
            failures++;
        }
    }
    { /* box microprogram: identity boxes, point (0.5, -2, 1) + (0, 0, 0), radius 1 */
        Vec m[3] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}}, p = {0.5f, -2, 1, 1}, d = {0, 0, 0, 0};
        Vec lim = {-3, 0, 0, 0};
        Vec *pm = m, *pd = &d, *pl = &lim;
        int vi2, vi5;
        ps2_op o[] = {OP(pl), OP(pm), OP(pm), OP(p), OP(pd), OP(vi2), OP(vi5)};
        ps2_asm("lqc2 $vf1,0(%1)\n\tlqc2 $vf2,16(%1)\n\tlqc2 $vf3,32(%1)\n\tlqc2 $vf7,0(%2)\n\tlqc2 $vf8,16(%2)\n\t"
                "lqc2 $vf9,32(%2)\n\tlqc2 $vf5,%3\n\tlqc2 $vf4,0(%4)\n\tlqc2 $vf10,0(%0)\n\tvcallms 0xc0\n\t"
                "cfc2 %5,$vi2\n\tcfc2 %6,$vi5", "r|r|r|m|r|=r|=r", 7, o);
        /* +r: (1.5, -1, 2): y negative -> 0x40, any -> 2; vf10.x = -3 + 2 < 0 -> 0x80.
           -r: (-0.5, -3, 0): x, y negative -> 0xC0 | 2 */
        expect("box.vi2", vi2, 0xC2);
        expect("box.vi5", vi5, 0xC2);
    }
    exceptional_macro();
    compare_interpreter();
    printf(failures ? "%d FAILED\n" : "all semantic tests passed\n", failures);
    return failures != 0;
}
