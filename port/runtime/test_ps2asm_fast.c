/* Full-state oracle for the fixed asm templates (ps2asm_fast.h); never linked into the game.
   Each case runs the same asm statement through the generic interpreter and the fast
   template and compares the whole CPU state, memory and MXCSR. "negative N" corrupts
   case N's fast result and must fail. */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "ps2asm.c"

int ps2_vu0_run(ps2_cpu *c, unsigned a) { (void)c; (void)a; abort(); return -1; }
ps2_cpu *ps2_vu1_regs;
void (*ps2_vu1_regs_sync)(void);

static const char matrix_direction_text[] =
    "lqc2 $vf11, 0x0(%1)\n lqc2 $vf12, 0x0(%0)\n lqc2 $vf13, 0x10(%0)\n lqc2 $vf14, 0x20(%0)\n"
    "vaddw.xyz $vf15, $vf0, $vf0w\n vmul.xyz $vf12, $vf12, $vf11\n vmul.xyz $vf13, $vf13, $vf11\n"
    "vmul.xyz $vf14, $vf14, $vf11\n vadday.x ACC, $vf12, $vf12y\n vmaddz.x $vf11, $vf15, $vf12z\n"
    "vaddaz.y ACC, $vf13, $vf13z\n vmaddx.y $vf11, $vf15, $vf13x\n vaddax.z ACC, $vf14, $vf14x\n"
    "vmaddy.z $vf11, $vf15, $vf14y\n sqc2 $vf11, 0x0(%2)";
/* binding: how invoke() hands the three test pointers to the statement's operands. */
typedef struct { const char *name, *text, *constraints; int operands, binding, expected; } TestCase;
static const TestCase cases[] = {
    {"matrix direction", matrix_direction_text, "r|r|r", 3, 0, FAST_MATRIX_DIRECTION},
    {"bounds", "lqc2 $vf11,0(%0)\nvmini.xyzw $vf30,$vf30,$vf11\nvmax.xyzw $vf31,$vf31,$vf11",
     "r", 1, 1, FAST_BOUNDS},
    {"vector add in place", "lqc2 $vf11,0(%0)\nlqc2 $vf12,0(%1)\nvadd.xyz $vf11,$vf11,$vf12\nsqc2 $vf11,0(%0)",
     "r|r", 2, 2, FAST_VECTOR_ADD},
    {"vector subtract memory", "lqc2 $vf11,0(%1)\nlqc2 $vf12,0(%2)\nvsub.xyz $vf11,$vf11,$vf12\nsqc2 $vf11,%0",
     "=m|r|r", 3, 3, FAST_VECTOR_SUB},
    {"vector add memory", "lqc2 $vf11,0(%1)\nlqc2 $vf12,0(%2)\nvadd.xyz $vf11,$vf11,$vf12\nsqc2 $vf11,%0",
     "=m|r|r", 3, 3, FAST_VECTOR_ADD},
    {"vector subtract pointer", "lqc2 $vf11,0(%0)\nlqc2 $vf12,0(%1)\nvsub.xyz $vf11,$vf11,$vf12\nsqc2 $vf11,0(%2)",
     "r|r|r", 3, 0, FAST_VECTOR_SUB},
    {"quad copy", "lq $2,0(%1)\nsq $2,%0", "=m|r", 2, 4, FAST_QUAD_COPY},
};
enum { CASES = sizeof cases / sizeof *cases };
static unsigned active;

static void invoke(void *m, void *v, void *o)
{
    const TestCase *c = &cases[active];
    ps2_op ops[] = {{&m, 4}, {&v, 4}, {&o, 4}};
    if (c->binding == 3) {
        ops[0] = (ps2_op){o, 16};
        ops[1] = (ps2_op){&m, 4};
        ops[2] = (ps2_op){&v, 4};
    }
    if (c->binding == 4) {
        ops[0] = (ps2_op){o, 16};
        ops[1] = (ps2_op){&m, 4};
    }
    ps2_asm(c->text, c->constraints, c->operands, ops);
}
__declspec(noinline) static void original(void *m, void *v, void *o) { fast_enabled = 0; invoke(m, v, o); }
__declspec(noinline) static void specialized(void *m, void *v, void *o) { fast_enabled = 1; invoke(m, v, o); }

static unsigned seed = 12345;
static unsigned random32(void)
{
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    return seed;
}

/* Graphics fences and MMIO writes must happen in the same order with the same $zero. */
static uint64_t callback_trace;
static void fence_probe(void)
{
    callback_trace = callback_trace * 131 + ps2.gpr[0].w[0] + 17;
    ps2.gpr[0].w[0] = 0xabcdef;
}
static void write_probe(uintptr_t address, uint64_t value, unsigned size)
{
    callback_trace = callback_trace * 131 + address + value + size;
    ps2.gpr[0].w[0] = 0xfedcba;
}
static int mapped_cases(void)
{
    unsigned char *page = VirtualAlloc((void *)0x12000000, 65536, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (page != (void *)0x12000000) return 2;
    unsigned char before[256], want[256];
    ps2_cpu initial, expected;
    for (unsigned i = 0; i < 256; i++) before[i] = (unsigned char)random32();
    ps2_graphics_sync = fence_probe;
    ps2_mmio_write = write_probe;
    for (unsigned t = 0; t < 64; t++) {
        unsigned *state = (unsigned *)&initial;
        for (unsigned i = 0; i < sizeof initial / 4; i++) state[i] = random32();
        void *m = page + 0x1000 + (t % 4), *v = page + 0x1040 + (t % 4), *o = page + 0x1000 + (t % 3) * 64 + (t % 4);
        memcpy(page + 0x1000, before, 256);
        ps2 = initial;
        callback_trace = 0;
        _mm_setcsr(0x1f80);
        original(m, v, o);
        expected = ps2;
        uint64_t trace = callback_trace;
        unsigned mxcsr = _mm_getcsr();
        memcpy(want, page + 0x1000, 256);
        memcpy(page + 0x1000, before, 256);
        ps2 = initial;
        callback_trace = 0;
        _mm_setcsr(0x1f80);
        specialized(m, v, o);
        if (memcmp(&ps2, &expected, sizeof ps2) || memcmp(page + 0x1000, want, 256) ||
            trace != callback_trace || mxcsr != _mm_getcsr())
            return 1;
    }
    ps2_graphics_sync = NULL;
    ps2_mmio_write = NULL;
    VirtualFree(page, 0, MEM_RELEASE);
    puts("MATCH 64 mapped-memory/fence/MMIO trace cases");
    return 0;
}

static int run_case(int negative)
{
    __declspec(align(16)) unsigned arena[32], initial[32], want[32];
    ps2_cpu start, expected;
    unsigned csr = _mm_getcsr();
    const TestCase *c = &cases[active];
    printf("case=%u %s\n", active, c->name);
    Prog *program = lookup(c->text, c->constraints, c->operands);
    if (program->fast_kind != c->expected) {
        puts("template not specialized");
        return 2;
    }
    if (active == 0) {
        /* Any changed bit of the parsed matrix program must fall back to the interpreter. */
        Ins changed[15];
        Prog probe = *program;
        probe.ins = changed;
        for (unsigned i = 0; i < sizeof changed; i++) {
            memcpy(changed, program->ins, sizeof changed);
            ((unsigned char *)changed)[i] ^= 1;
            if (select_fast_program(&probe)) {
                puts("changed template accepted");
                return 2;
            }
        }
    }
    static const unsigned edge[] = {0, 0x80000000, 0x7f800000, 0xff800000, 0x7fffffff,
                                    0x00000001, 0x007fffff, 0x00800000, 0x7f7fffff};
    for (unsigned t = 0; t < 200000; t++) {
        unsigned *p = (unsigned *)&start;
        for (unsigned i = 0; i < sizeof start / 4; i++) p[i] = random32();
        for (int i = 0; i < 32; i++) initial[i] = random32();
        if (t % 2 == 0)
            for (int i = 0; i < 32; i++) initial[i] = edge[random32() % 9];
        if (t % 4 == 0)
            for (int i = 0; i < 4; i++) {
                start.vf[30].w[i] = edge[(t + i) % 9];
                start.vf[31].w[i] = edge[(t + i + 3) % 9];
            }
        /* Unaligned operands, and operands aliasing the emulated registers themselves. */
        void *m = arena + (t % 4), *v = arena + 16 + (t % 4), *o = arena + (t % 6) * 4 + (t % 4);
        if (t % 12 == 0) m = &ps2.gpr[0];
        if (t % 12 == 1) v = &ps2.vf[11];
        if (t % 12 == 2) m = &ps2.vf[12];
        if (t % 10 == 0) o = &ps2.gpr[0];
        if (t % 10 == 1) o = &ps2.vf[11];
        if (t % 10 == 2) o = ps2.vacc;
        unsigned mode = 0x1f80 | ((t % 4) << 13) | ((t % 8 < 4) ? 0x8040 : 0);
        memcpy(arena, initial, sizeof arena);
        ps2 = start;
        _mm_setcsr(mode);
        original(m, v, o);
        expected = ps2;
        memcpy(want, arena, sizeof arena);
        unsigned want_csr = _mm_getcsr();
        memcpy(arena, initial, sizeof arena);
        ps2 = start;
        _mm_setcsr(mode);
        specialized(m, v, o);
        if (negative) ps2.vf[11].w[0] ^= 1;
        if (memcmp(&ps2, &expected, sizeof ps2) || memcmp(arena, want, sizeof arena) || _mm_getcsr() != want_csr) {
            printf("DIFF trial=%u csr=%x/%x\n", t, _mm_getcsr(), want_csr);
            for (unsigned z = 0; z < sizeof ps2 / 4; z++)
                if (((unsigned *)&ps2)[z] != ((unsigned *)&expected)[z])
                    printf("cpu[%u]=%x want=%x\n", z, ((unsigned *)&ps2)[z], ((unsigned *)&expected)[z]);
            return 1;
        }
    }
    _mm_setcsr(csr);
    printf("MATCH 200000 complete CPU/memory/MXCSR cases (alias + unaligned + raw bits)\n");
    return mapped_cases() ? 3 : 0;
}

int main(int argc, char **argv)
{
    if (argc > 2 && !strcmp(argv[1], "negative")) {
        active = (unsigned)atoi(argv[2]);
        return active < CASES ? run_case(1) : 2;
    }
    for (active = 0; active < CASES; active++) {
        int result = run_case(0);
        if (result) return result;
    }
    if (lookup("nop", "", 0)->fast_kind) {
        puts("unsupported template accelerated");
        return 2;
    }
    puts("ALL FAST TEMPLATE CASES MATCH; generic fallback retained");
    return 0;
}
