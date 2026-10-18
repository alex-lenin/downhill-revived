/* PS2 inline asm runtime. port/tools/transform.py turns each inline asm statement of the game into
   ps2_asm(template, constraints, n, operands); this parses the template once per call site and
   runs it on an emulated EE/VU0 register file (struct ps2_cpu).

   Covered: the EE FPU (incl. ACC forms), VU0 macro mode (broadcast, dest masks, ACC, Q, R),
   128-bit GPR moves and the few MMI/integer ops the game uses. vcallms runs a native routine
   registered with ps2_vu0_register_micro (vu0_micro.c), else the uploaded microcode
   (vu0_interp.c). Not covered: asm that reads function arguments straight from $a0..$a3
   (whole-function asm; needs a C replacement), reported once per call site on stderr. */
#include "ps2asm.h"
#include "ps2float.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xmmintrin.h>

typedef struct {
    void *p;
    unsigned int size;
} ps2_op; /* as in port/include/port.h */

static ps2_q128 vu0_mem[256];
static uint8_t vu0_code[4096];
/* vf0 reads (0, 0, 0, 1) on the VU and never changes. The interpreter's readers special-case
   register 0, but recompiled code stores it straight from memory (sqc2 $vf0: the identity row
   of a matrix), so it must hold that value; every writer skips register 0. */
ps2_cpu ps2 = {.vf[0].f[3] = 1.0f, .vu0_code = vu0_code, .vu0_mem = vu0_mem};

void (*ps2_mmio_write)(uintptr_t address, uint64_t value, unsigned size);
void (*ps2_graphics_sync)(void);
void (*ps2_intc_read)(void);

void ps2_store32(uintptr_t address, uint32_t value)
{
    address = ps2_memory_address(address);
    *(volatile uint32_t *)address = value;
    if ((address >> 24 == 0x10 || address >> 24 == 0x12) && ps2_mmio_write)
        ps2_mmio_write(address, value, 4);
}

void ps2_store64(uintptr_t address, uint64_t value)
{
    address = ps2_memory_address(address);
    *(volatile uint64_t *)address = value;
    if ((address >> 24 == 0x10 || address >> 24 == 0x12) && ps2_mmio_write)
        ps2_mmio_write(address, value, 8);
}

void ps2_store128(uintptr_t address, const ps2_q128 *value)
{
    address = ps2_memory_address(address);
    memcpy((void *)address, value, 16);
    if ((address >> 24 == 0x10 || address >> 24 == 0x12) && ps2_mmio_write)
        ps2_mmio_write(address, value->d[0], 16);
}

enum { K_NONE, K_GPR, K_FPR, K_VF, K_VI, K_ACC, K_Q, K_R, K_I, K_IMM, K_MEM, K_OPMEM, K_VIMEM };

typedef struct {
    uint8_t kind;
    uint8_t reg;  /* register number; for K_MEM the base GPR, for K_OPMEM the operand index */
    uint8_t comp; /* vf component x..w = 0..3, NO_COMP if none */
    int8_t step;  /* K_VIMEM: +1 post-increment, -1 pre-decrement */
    int32_t imm;  /* immediate or memory offset */
} Opd;
#define NO_COMP 0xFF

enum {
    NOP, LQ, SQ, LQC2, SQC2, LWC1, SWC1, MFC1, MTC1, QMFC2, QMTC2, CFC2, CTC2, LUI, LI, DADDU, ADDU, MULTU,
    PEXTLW, PEXTUW, PCPYLD, PCPYUD, PROT3W, PADDW, PSUBH, PCGTH, PEXTLH, PPACH, PSRAW, PMULTH, PMFHI, PCPYH,
    PAND, PNOR, PXOR, PSUBB, PSUBW, PADDH, PMINH, PMAXH, PPACB,
    PMFLO,
    F_ADD, F_SUB, F_MUL, F_DIV, F_SQRT, F_RSQRT, F_ABS, F_NEG, F_MOV, F_MAX, F_MIN,
    F_ADDA, F_SUBA, F_MULA, F_MADDA, F_MSUBA, F_MADD, F_MSUB,
    V_ADD, V_SUB, V_MUL, V_MAX, V_MINI, V_MADD, V_MSUB, V_ADDA, V_SUBA, V_MULA, V_MADDA, V_MSUBA,
    V_OPMULA, V_OPMSUB, V_FTOI, V_ITOF, V_MOVE, V_MR32, V_ABS, V_DIV, V_SQRT, V_RSQRT,
    V_RINIT, V_RNEXT, V_RGET, V_IADDI, V_SQI, V_LQI, V_CALLMS,
};

typedef struct {
    uint8_t op;
    uint8_t dest; /* VU dest mask: bit 0 = x .. bit 3 = w */
    uint8_t bc;   /* VU broadcast: 0..3 = x..w of ft, 4 = Q, 5 = I, NO_COMP = none */
    uint8_t n;
    int32_t arg;  /* fixed-point bits of vftoi/vitof */
    Opd o[3];
} Ins;

enum { OPK_GPR = 1, OPK_FPR, OPK_MEM, OPK_LO, OPK_HI };

typedef struct {
    const char *tmpl, *cons;
    int n_ops, n_ins;
    uint8_t kind[32];    /* OPK_* of each operand */
    int8_t alias[32];    /* tied input: index of the output it shares a register with, else -1 */
    uint8_t in[32], out[32];
    Ins *ins;
    int reported;
    int bad; /* unparsable or not runnable here: skipped */
    int quad_reported;
    int fast_kind; /* selected once from the complete parsed instruction sequence */
} Prog;

static int select_fast_program(const Prog *p);

static int problems;
int ps2_asm_problems(void) { return problems; }

static void report(Prog *p, const char *what, const char *detail)
{
    problems++;
    if (p->reported++ < 3)
        fprintf(stderr, "ps2_asm: %s '%s' in \"%s\"\n", what, detail, p->tmpl);
}

/* lq/sq/lqc2/sqc2 ignore the low 4 address bits, as on the EE and in recompiled code (recomp.h
   M128), so both sides agree on which quadword an address names. The port build gives local
   aggregates the EE's 16-byte alignment (build.py align_quadword_objects); DHD_QUAD_CHECK=1
   reports any access still unaligned, with the calling function's return address. */
static const void *asm_caller; /* return address of the current ps2_asm call */

static uint8_t *quad(Prog *p, uint8_t *at)
{
    static int check = -1;
    if (check < 0) { const char *e = getenv("DHD_QUAD_CHECK"); check = e && *e == '1'; }
    if (check && ((uintptr_t)at & 15) && !p->quad_reported++)
        fprintf(stderr, "ps2_asm: unaligned quadword %p from %p in \"%s\"\n", (void *)at, asm_caller,
                p->tmpl);
    return (uint8_t *)((uintptr_t)at & ~(uintptr_t)15);
}

/* ---- parsing ---- */

static const char *const GPR_NAMES[32] = {
    "zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
    "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"};

static int gpr_number(const char *s)
{
    int i;
    if (s[0] >= '0' && s[0] <= '9')
        return atoi(s) & 31;
    if (!strcmp(s, "s8"))
        return 30;
    for (i = 0; i < 32; i++)
        if (!strcmp(s, GPR_NAMES[i]))
            return i;
    return -1;
}

static int comp_of(char c)
{
    switch (c) {
    case 'x': return 0;
    case 'y': return 1;
    case 'z': return 2;
    case 'w': return 3;
    }
    return -1;
}

/* One operand of an instruction ("$vf12x", "0x10(%1)", "%0", "ACC", "($vi7++)", ...). */
static int parse_operand(Prog *p, char *t, Opd *o)
{
    char *paren;
    memset(o, 0, sizeof *o);
    o->comp = NO_COMP;
    while (*t == ' ' || *t == '\t')
        t++;
    {
        size_t len = strlen(t);
        while (len && (t[len - 1] == ' ' || t[len - 1] == '\t'))
            t[--len] = 0;
    }
    if (!strcmp(t, "ACC") || !strcmp(t, "$ACC")) { o->kind = K_ACC; return 1; }
    if (!strcmp(t, "Q")) { o->kind = K_Q; return 1; }
    if (!strcmp(t, "R")) { o->kind = K_R; return 1; }
    if (!strcmp(t, "I")) { o->kind = K_I; return 1; }
    paren = strchr(t, '(');
    if (paren) {
        char *base = paren + 1, *end = strchr(base, ')');
        if (!end)
            return 0;
        *end = 0;
        if (strstr(base, "$vi")) { /* ($vi7++), (--$vi7), ($vi7) */
            o->kind = K_VIMEM;
            o->step = strstr(base, "++") ? 1 : strstr(base, "--") ? -1 : 0;
            o->reg = (uint8_t)(atoi(strstr(base, "$vi") + 3) & 15);
            return 1;
        }
        *paren = 0;
        o->imm = *t ? (int32_t)strtol(t, NULL, 0) : 0;
        o->kind = K_MEM;
        if (base[0] == '%') {
            int k = atoi(base + 1);
            if (k >= p->n_ops || p->kind[k] != OPK_GPR)
                return 0;
            o->reg = (uint8_t)(32 + (p->alias[k] >= 0 ? p->alias[k] : k));
            return 1;
        }
        if (base[0] != '$' || gpr_number(base + 1) < 0)
            return 0;
        o->reg = (uint8_t)gpr_number(base + 1);
        return 1;
    }
    if (t[0] == '%') {
        int k = atoi(t + 1), r;
        if (k >= p->n_ops)
            return 0;
        r = p->alias[k] >= 0 ? p->alias[k] : k;
        switch (p->kind[r]) {
        case OPK_GPR: o->kind = K_GPR; o->reg = (uint8_t)(32 + r); return 1;
        case OPK_FPR: o->kind = K_FPR; o->reg = (uint8_t)(32 + r); return 1;
        case OPK_MEM: o->kind = K_OPMEM; o->reg = (uint8_t)r; return 1;
        }
        return 0;
    }
    if (!strncmp(t, "$vf", 3)) {
        char *e;
        o->kind = K_VF;
        o->reg = (uint8_t)(strtol(t + 3, &e, 10) & 31);
        if (*e) {
            int c = comp_of(*e);
            if (c < 0 || e[1])
                return 0;
            o->comp = (uint8_t)c;
        }
        return 1;
    }
    if (!strncmp(t, "$vi", 3)) {
        o->kind = K_VI;
        o->reg = (uint8_t)(atoi(t + 3) & 31); /* 16..31: control registers (cfc2, ctc2) */
        return 1;
    }
    if (t[0] == '$' && t[1] == 'f' && t[2] >= '0' && t[2] <= '9') {
        o->kind = K_FPR;
        o->reg = (uint8_t)(atoi(t + 2) & 31);
        return 1;
    }
    if (t[0] == '$') {
        int r = gpr_number(t + 1);
        if (r < 0)
            return 0;
        o->kind = K_GPR;
        o->reg = (uint8_t)r;
        return 1;
    }
    if ((t[0] >= '0' && t[0] <= '9') || t[0] == '-') {
        o->kind = K_IMM;
        o->imm = (int32_t)strtol(t, NULL, 0);
        return 1;
    }
    return 0;
}

typedef struct {
    const char *name;
    uint8_t op;
} Name;

static const Name SIMPLE[] = {
    {"nop", NOP}, {"sync", NOP}, {"ei", NOP}, {"vnop", NOP}, {"vwaitq", NOP},
    {"lq", LQ}, {"sq", SQ}, {"lqc2", LQC2}, {"sqc2", SQC2}, {"lwc1", LWC1}, {"swc1", SWC1},
    {"mfc1", MFC1}, {"mtc1", MTC1}, {"qmfc2", QMFC2}, {"qmtc2", QMTC2}, {"cfc2", CFC2}, {"ctc2", CTC2},
    {"lui", LUI}, {"li", LI}, {"daddu", DADDU}, {"addu", ADDU}, {"multu", MULTU},
    {"pextlw", PEXTLW}, {"pextuw", PEXTUW}, {"pcpyld", PCPYLD}, {"pcpyud", PCPYUD}, {"prot3w", PROT3W},
    {"paddw", PADDW}, {"psubh", PSUBH}, {"pcgth", PCGTH}, {"pextlh", PEXTLH}, {"ppach", PPACH},
    {"psraw", PSRAW}, {"pmulth", PMULTH}, {"pmfhi", PMFHI}, {"pmflo", PMFLO}, {"pcpyh", PCPYH},
    {"pand", PAND}, {"pnor", PNOR}, {"pxor", PXOR}, {"psubb", PSUBB}, {"psubw", PSUBW}, {"paddh", PADDH},
    {"pminh", PMINH}, {"pmaxh", PMAXH}, {"ppacb", PPACB},
    {"add", F_ADD}, {"sub", F_SUB}, {"mul", F_MUL}, {"div", F_DIV}, {"sqrt", F_SQRT}, {"rsqrt", F_RSQRT},
    {"abs", F_ABS}, {"neg", F_NEG}, {"mov", F_MOV}, {"max", F_MAX}, {"min", F_MIN},
    {"adda", F_ADDA}, {"suba", F_SUBA}, {"mula", F_MULA}, {"madda", F_MADDA}, {"msuba", F_MSUBA},
    {"madd", F_MADD}, {"msub", F_MSUB},
    {"vopmula", V_OPMULA}, {"vopmsub", V_OPMSUB}, {"vmove", V_MOVE}, {"vmr32", V_MR32}, {"vabs", V_ABS},
    {"vdiv", V_DIV}, {"vsqrt", V_SQRT}, {"vrsqrt", V_RSQRT}, {"vrinit", V_RINIT}, {"vrnext", V_RNEXT},
    {"vrget", V_RGET}, {"viaddi", V_IADDI}, {"vsqi", V_SQI}, {"vlqi", V_LQI}, {"vcallms", V_CALLMS},
};

/* VU arithmetic families, longest first so "madda" wins over "madd". */
static const Name VFAMILY[] = {
    {"madda", V_MADDA}, {"msuba", V_MSUBA}, {"mula", V_MULA}, {"adda", V_ADDA}, {"suba", V_SUBA},
    {"madd", V_MADD}, {"msub", V_MSUB}, {"mini", V_MINI}, {"max", V_MAX}, {"add", V_ADD},
    {"sub", V_SUB}, {"mul", V_MUL},
};

static int parse_line(Prog *p, char *line, Ins *in)
{
    char *mn = line, *args, *dot, *a;
    size_t i;
    int found = 0;
    memset(in, 0, sizeof *in);
    in->bc = NO_COMP;
    while (*mn == ' ' || *mn == '\t')
        mn++;
    for (args = mn; *args && *args != ' ' && *args != '\t'; args++)
        ;
    if (*args)
        *args++ = 0;
    if (!*mn || mn[strlen(mn) - 1] == ':' || mn[0] == '.')
        return 0; /* empty, label or directive */
    dot = strchr(mn, '.');
    if (dot)
        *dot++ = 0;
    for (i = 0; i < sizeof SIMPLE / sizeof *SIMPLE; i++)
        if (!strcmp(mn, SIMPLE[i].name)) {
            in->op = SIMPLE[i].op;
            found = 1;
            break;
        }
    if (!found && mn[0] == 'v') {
        if (!strncmp(mn, "vftoi", 5) || !strncmp(mn, "vitof", 5)) {
            in->op = mn[1] == 'f' ? V_FTOI : V_ITOF;
            in->arg = atoi(mn + 5);
            found = 1;
        }
        for (i = 0; !found && i < sizeof VFAMILY / sizeof *VFAMILY; i++) {
            size_t len = strlen(VFAMILY[i].name);
            const char *rest = mn + 1 + len;
            if (strncmp(mn + 1, VFAMILY[i].name, len))
                continue;
            if (*rest && rest[1])
                continue;
            if (*rest == 'q')
                in->bc = 4;
            else if (*rest == 'i')
                in->bc = 5;
            else if (*rest && comp_of(*rest) >= 0)
                in->bc = (uint8_t)comp_of(*rest);
            else if (*rest)
                continue;
            in->op = VFAMILY[i].op;
            found = 1;
        }
    }
    if (!found) {
        report(p, "unsupported instruction", mn);
        return -1;
    }
    if (dot && mn[0] == 'v')
        for (a = dot; *a; a++) {
            int c = comp_of(*a);
            if (c >= 0)
                in->dest |= (uint8_t)(1 << c);
        }
    /* operands: split at commas outside parentheses */
    while (args && *args && in->n < 3) {
        char *end = args;
        int depth = 0;
        while (*end && (depth || *end != ',')) {
            if (*end == '(') depth++;
            if (*end == ')') depth--;
            end++;
        }
        a = args;
        args = *end ? end + 1 : NULL;
        *end = 0;
        if (!parse_operand(p, a, &in->o[in->n])) {
            report(p, "bad operand", a);
            return -1;
        }
        in->n++;
    }
    return 1;
}

static void parse_constraints(Prog *p, int n)
{
    const char *c = p->cons;
    int k;
    p->n_ops = n;
    for (k = 0; k < n && k < 32; k++) {
        int out = 0, inout = 0;
        p->alias[k] = -1;
        for (; *c && *c != '|'; c++) {
            switch (*c) {
            case '=': out = 1; break;
            case '+': inout = 1; break;
            case 'r': case 'd': p->kind[k] = OPK_GPR; break;
            case 'f': p->kind[k] = OPK_FPR; break;
            case 'm': p->kind[k] = OPK_MEM; break;
            case 'l': p->kind[k] = OPK_LO; break;
            case 'h': p->kind[k] = OPK_HI; break;
            default:
                if (*c >= '0' && *c <= '9') {
                    p->alias[k] = (int8_t)(*c - '0');
                    p->kind[k] = p->kind[*c - '0'];
                }
            }
        }
        if (*c == '|')
            c++;
        p->out[k] = (uint8_t)(out || inout);
        p->in[k] = (uint8_t)(!out || inout);
    }
}

/* Asm that reads $a0..$a3, $gp, $sp or $ra before writing them is a whole function body relying
   on the PS2 calling convention; on the PC those registers hold nothing, so it can't run here. */
static void check_abi_reads(Prog *p)
{
    const unsigned ABI = 0xB00000F0u; /* a0..a3, gp, sp, ra */
    unsigned written = 0;
    int k, i;
    for (k = 0; k < p->n_ins; k++) {
        const Ins *in = &p->ins[k];
        /* these take their first operand as a source, the rest write it */
        int first_is_dest = in->op != SQ && in->op != SQC2 && in->op != SWC1 && in->op != MTC1 &&
                            in->op != QMTC2 && in->op != MULTU && in->op != V_SQI;
        for (i = first_is_dest && in->o[0].kind == K_GPR ? 1 : 0; i < in->n; i++) {
            const Opd *o = &in->o[i];
            if ((o->kind == K_GPR || o->kind == K_MEM) && o->reg < 32 && (ABI >> o->reg & 1) &&
                !(written >> o->reg & 1)) {
                report(p, "reads a calling-convention register (whole-function asm needs a C version)",
                       GPR_NAMES[o->reg]);
                p->bad = 1;
                return;
            }
        }
        if (first_is_dest && in->n && in->o[0].kind == K_GPR && in->o[0].reg < 32)
            written |= 1u << in->o[0].reg;
    }
}

/* Constraints of ps2_exec: recompiled code runs on the real registers, argument registers too. */
static const char RECOMPILED[] = "";

static Prog *compile(const char *tmpl, const char *cons, int n)
{
    Prog *p = calloc(1, sizeof *p);
    char *text = malloc(strlen(tmpl) + 1), *line, *next;
    int cap = 8;
    p->tmpl = tmpl;
    p->cons = cons;
    parse_constraints(p, n);
    p->ins = malloc(cap * sizeof *p->ins);
    strcpy(text, tmpl);
    for (line = text; line; line = next) {
        next = line + strcspn(line, "\n;");
        next = *next ? (*next = 0, next + 1) : NULL;
        if (p->n_ins == cap)
            p->ins = realloc(p->ins, (cap *= 2) * sizeof *p->ins);
        switch (parse_line(p, line, &p->ins[p->n_ins])) {
        case 1: p->n_ins++; break;
        case -1: p->bad = 1; break;
        }
    }
    free(text);
    if (cons != RECOMPILED)
        check_abi_reads(p);
    return p;
}

/* Programs by call site: the (template, constraints) literal pointers are unique per site. */
#define TABLE_SIZE 8192
static struct {
    const char *tmpl, *cons;
    Prog *prog;
} table[TABLE_SIZE];

static Prog *lookup(const char *tmpl, const char *cons, int n)
{
    unsigned h = (unsigned)(((uintptr_t)tmpl >> 2) * 2654435761u ^ ((uintptr_t)cons >> 2)) % TABLE_SIZE;
    while (table[h].tmpl) {
        if (table[h].tmpl == tmpl && table[h].cons == cons)
            return table[h].prog;
        h = (h + 1) % TABLE_SIZE;
    }
    table[h].tmpl = tmpl;
    table[h].cons = cons;
    Prog *p = table[h].prog = compile(tmpl, cons, n);
    if (!p->bad) p->fast_kind = select_fast_program(p);
    return p;
}

/* ---- execution ---- */

static ps2_vu0_micro_fn micro[0x1000 / 8];

void ps2_vu0_register_micro(unsigned addr, ps2_vu0_micro_fn fn)
{
    micro[(addr / 8) % (sizeof micro / sizeof *micro)] = fn;
}

static float ee_div(float a, float b)
{
    return ps2_ee_div(a, b);
}

static float ee_sqrt(float a) { return ps2_ee_sqrt(a); }

static uint8_t *address(const Opd *o, ps2_op *ops)
{
    if (o->kind == K_OPMEM)
        return (uint8_t *)ps2_memory_address((uintptr_t)ops[o->reg].p);
    return (uint8_t *)ps2_memory_address((uintptr_t)(ps2.gpr[o->reg].d[0] + (int64_t)o->imm));
}

static float vcomp(int reg, int c)
{
    return reg ? ps2.vf[reg].f[c] : (c == 3 ? 1.0f : 0.0f);
}

/* Raw 128 bits of a vf register (bit-exact: also used for integer data). */
static ps2_q128 vraw(int reg)
{
    ps2_q128 r;
    if (reg)
        return ps2.vf[reg];
    r.f[0] = r.f[1] = r.f[2] = 0.0f;
    r.f[3] = 1.0f;
    return r;
}

/* VPU_STAT reads idle only after any queued VU1 work finishes. */
enum { VI_R = 20, VI_I = 21, VI_Q = 22, VI_FBRST = 28, VI_VPU_STAT = 29 };

static uint32_t vu0_control(int reg)
{
    uint32_t bits;
    switch (reg) {
    case VI_R: return ps2.r;
    case VI_I: memcpy(&bits, &ps2.i, 4); return bits;
    case VI_Q: memcpy(&bits, &ps2.q, 4); return bits;
    case VI_FBRST: case VI_VPU_STAT:
        if (ps2_graphics_sync) ps2_graphics_sync();
        return 0;
    default: return reg < 16 ? ps2.vi[reg] : ps2.vu0_ctrl[reg - 16];
    }
}

static void set_vu0_control(int reg, uint32_t value)
{
    if (reg == VI_FBRST && ps2_graphics_sync) ps2_graphics_sync();
    switch (reg) {
    case 0: break;
    case VI_R: ps2.r = value; break;
    case VI_I: memcpy(&ps2.i, &value, 4); break;
    case VI_Q: memcpy(&ps2.q, &value, 4); break;
    default:
        if (reg < 16)
            ps2.vi[reg] = (uint16_t)value;
        else
            ps2.vu0_ctrl[reg - 16] = value;
    }
}

static void vwrite_raw(int reg, int dest, ps2_q128 v)
{
    int c;
    if (!reg)
        return;
    for (c = 0; c < 4; c++)
        if (dest & (1 << c))
            ps2.vf[reg].w[c] = v.w[c];
}

static void vread(int reg, float v[4])
{
    int c;
    for (c = 0; c < 4; c++)
        v[c] = vcomp(reg, c);
}

static float field(const Opd *o, int c) /* fs/ft component, honoring an explicit field suffix */
{
    return vcomp(o->reg, o->comp != NO_COMP ? o->comp : c);
}

static void vwrite(int reg, int dest, const float v[4])
{
    int c;
    if (!reg)
        return;
    for (c = 0; c < 4; c++)
        if (dest & (1 << c))
            ps2.vf[reg].f[c] = v[c];
}

static void set_gpr32(int reg, uint32_t v) { ps2.gpr[reg].d[0] = (uint64_t)(int64_t)(int32_t)v; }

static void vu_arith(const Ins *in)
{
    float s[4], t[4], r[4];
    int c, acc = in->op >= V_ADDA;
    const Opd *fd = &in->o[0], *fs = &in->o[1], *ft = &in->o[2];
    for (c = 0; c < 4; c++) {
        s[c] = fs->kind == K_ACC ? ps2.vacc[c] : vcomp(fs->reg, c);
        if (in->bc == 4)
            t[c] = ps2.q;
        else if (in->bc == 5)
            t[c] = ps2.i;
        else if (in->bc != NO_COMP)
            t[c] = vcomp(ft->reg, in->bc);
        else
            t[c] = vcomp(ft->reg, c);
    }
    for (c = 0; c < 4; c++) {
        float prod;
        if (!(in->dest & (1 << c)))
            continue;
        switch (in->op) {
        case V_ADD: case V_ADDA: r[c] = ps2_float_add(s[c], t[c]); break;
        case V_SUB: case V_SUBA: r[c] = ps2_float_sub(s[c], t[c]); break;
        case V_MUL: case V_MULA: r[c] = ps2_float_mul(s[c], t[c]); break;
        case V_MAX: r[c] = ps2_float_minmax(s[c], t[c], 1); break;
        case V_MINI: r[c] = ps2_float_minmax(s[c], t[c], 0); break;
        case V_MADD: case V_MADDA: r[c] = ps2_vu_madd(ps2.vacc[c], s[c], t[c], 0); break;
        case V_MSUB: case V_MSUBA: r[c] = ps2_vu_madd(ps2.vacc[c], s[c], t[c], 1); break;
        }
    }
    if (acc) {
        for (c = 0; c < 4; c++)
            if (in->dest & (1 << c))
                ps2.vacc[c] = r[c];
    } else
        vwrite(fd->reg, in->dest, r);
}

static void run(Prog *p, ps2_op *ops)
{
    int k;
    for (k = 0; k < p->n_ins; k++) {
        const Ins *in = &p->ins[k];
        const Opd *a = &in->o[0], *b = &in->o[1], *c = &in->o[2];
        float v[4], w[4];
        int i;
        switch (in->op) {
        case NOP:
            break;
        case LQ:
            memcpy(&ps2.gpr[a->reg], quad(p, address(b, ops)), 16);
            break;
        case SQ:
            ps2_store128((uintptr_t)quad(p, address(b, ops)), &ps2.gpr[a->reg]);
            break;
        case LQC2:
            if (a->reg)
                memcpy(&ps2.vf[a->reg], quad(p, address(b, ops)), 16);
            break;
        case SQC2: {
            ps2_q128 r = vraw(a->reg);
            ps2_store128((uintptr_t)quad(p, address(b, ops)), &r);
            break;
        }
        case LWC1:
            memcpy(&ps2.fpr[a->reg], address(b, ops), 4);
            break;
        case SWC1:
            memcpy(address(b, ops), &ps2.fpr[a->reg], 4);
            break;
        case MFC1: {
            uint32_t bits;
            memcpy(&bits, &ps2.fpr[b->reg], 4);
            set_gpr32(a->reg, bits);
            break;
        }
        case MTC1:
            memcpy(&ps2.fpr[b->reg], &ps2.gpr[a->reg].w[0], 4);
            break;
        case QMFC2:
            ps2.gpr[a->reg] = vraw(b->reg);
            break;
        case QMTC2:
            if (b->reg)
                memcpy(&ps2.vf[b->reg], &ps2.gpr[a->reg], 16);
            break;
        case CFC2:
            ps2.gpr[a->reg].d[0] = b->kind == K_VI ? vu0_control(b->reg) : 0;
            break;
        case CTC2:
            if (b->kind == K_VI)
                set_vu0_control(b->reg, ps2.gpr[a->reg].w[0]);
            break;
        case LUI:
            set_gpr32(a->reg, (uint32_t)b->imm << 16);
            break;
        case LI:
            set_gpr32(a->reg, (uint32_t)b->imm);
            break;
        case DADDU:
            ps2.gpr[a->reg].d[0] = ps2.gpr[b->reg].d[0] + ps2.gpr[c->reg].d[0];
            break;
        case ADDU:
            set_gpr32(a->reg, ps2.gpr[b->reg].w[0] + ps2.gpr[c->reg].w[0]);
            break;
        case MULTU: {
            uint64_t m = (uint64_t)ps2.gpr[a->reg].w[0] * ps2.gpr[b->reg].w[0];
            ps2.lo = (uint64_t)(int64_t)(int32_t)(uint32_t)m;
            ps2.hi = (uint64_t)(int64_t)(int32_t)(uint32_t)(m >> 32);
            break;
        }
        case PEXTLW: case PEXTUW: {
            ps2_q128 s = ps2.gpr[b->reg], t = ps2.gpr[c->reg], d;
            int h = in->op == PEXTUW ? 2 : 0;
            d.w[0] = t.w[h]; d.w[1] = s.w[h]; d.w[2] = t.w[h + 1]; d.w[3] = s.w[h + 1];
            ps2.gpr[a->reg] = d;
            break;
        }
        case PCPYLD: case PCPYUD: {
            ps2_q128 s = ps2.gpr[b->reg], t = ps2.gpr[c->reg], d;
            if (in->op == PCPYLD) { d.d[0] = t.d[0]; d.d[1] = s.d[0]; }
            else { d.d[0] = s.d[1]; d.d[1] = t.d[1]; }
            ps2.gpr[a->reg] = d;
            break;
        }
        case PADDW: case PSUBH: case PCGTH: case PEXTLH: case PPACH: {
            ps2_q128 s = ps2.gpr[b->reg], t = ps2.gpr[c->reg], d;
            for (i = 0; i < 8; i++)
                switch (in->op) {
                case PADDW: if (i < 4) d.w[i] = s.w[i] + t.w[i]; break;
                case PSUBH: d.h[i] = (uint16_t)(s.h[i] - t.h[i]); break;
                case PCGTH: d.h[i] = s.sh[i] > t.sh[i] ? 0xFFFF : 0; break;
                case PEXTLH: d.h[i] = (i & 1 ? s : t).h[i / 2]; break; /* t0 s0 t1 s1 .. */
                default: d.h[i] = (i < 4 ? t : s).h[(i & 3) * 2]; break; /* PPACH: even halves */
                }
            ps2.gpr[a->reg] = d;
            break;
        }
        case PAND: case PNOR: case PXOR: {
            ps2_q128 s = ps2.gpr[b->reg], t = ps2.gpr[c->reg], d;
            for (i = 0; i < 2; i++)
                d.d[i] = in->op == PAND ? s.d[i] & t.d[i] : in->op == PXOR ? s.d[i] ^ t.d[i] : ~(s.d[i] | t.d[i]);
            ps2.gpr[a->reg] = d;
            break;
        }
        case PSUBB: case PPACB: {
            ps2_q128 s = ps2.gpr[b->reg], t = ps2.gpr[c->reg], d;
            for (i = 0; i < 16; i++)
                d.b[i] = in->op == PSUBB ? (uint8_t)(s.b[i] - t.b[i])
                                         : (i < 8 ? t : s).b[(i & 7) * 2]; /* PPACB: even bytes, rt low */
            ps2.gpr[a->reg] = d;
            break;
        }
        case PSUBW: case PADDH: case PMINH: case PMAXH: {
            ps2_q128 s = ps2.gpr[b->reg], t = ps2.gpr[c->reg], d;
            for (i = 0; i < 8; i++)
                switch (in->op) {
                case PSUBW: if (i < 4) d.w[i] = s.w[i] - t.w[i]; break;
                case PADDH: d.h[i] = (uint16_t)(s.h[i] + t.h[i]); break;
                case PMINH: d.sh[i] = s.sh[i] < t.sh[i] ? s.sh[i] : t.sh[i]; break;
                default: d.sh[i] = s.sh[i] > t.sh[i] ? s.sh[i] : t.sh[i]; break;
                }
            ps2.gpr[a->reg] = d;
            break;
        }
        case PSRAW: {
            ps2_q128 t = ps2.gpr[b->reg];
            for (i = 0; i < 4; i++)
                t.sw[i] >>= c->imm & 31;
            ps2.gpr[a->reg] = t;
            break;
        }
        case PMULTH: { /* 8 halfword products: rd gets 0,2,4,6; LO 0,1,4,5; HI 2,3,6,7 */
            ps2_q128 s = ps2.gpr[b->reg], t = ps2.gpr[c->reg], d, lo, hi;
            int32_t m[8];
            for (i = 0; i < 8; i++)
                m[i] = (int32_t)s.sh[i] * t.sh[i];
            for (i = 0; i < 4; i++)
                d.sw[i] = m[i * 2];
            lo.sw[0] = m[0]; lo.sw[1] = m[1]; hi.sw[0] = m[2]; hi.sw[1] = m[3];
            lo.sw[2] = m[4]; lo.sw[3] = m[5]; hi.sw[2] = m[6]; hi.sw[3] = m[7];
            ps2.lo = lo.d[0]; ps2.lo1 = lo.d[1]; ps2.hi = hi.d[0]; ps2.hi1 = hi.d[1];
            ps2.gpr[a->reg] = d;
            break;
        }
        case PMFHI: case PMFLO:
            ps2.gpr[a->reg].d[0] = in->op == PMFHI ? ps2.hi : ps2.lo;
            ps2.gpr[a->reg].d[1] = in->op == PMFHI ? ps2.hi1 : ps2.lo1;
            break;
        case PCPYH: { /* each doubleword's halfword 0, four times */
            ps2_q128 t = ps2.gpr[b->reg], d;
            for (i = 0; i < 8; i++)
                d.h[i] = t.h[i & 4];
            ps2.gpr[a->reg] = d;
            break;
        }
        case PROT3W: {
            ps2_q128 t = ps2.gpr[b->reg], d;
            d.w[0] = t.w[1]; d.w[1] = t.w[2]; d.w[2] = t.w[0]; d.w[3] = t.w[3];
            ps2.gpr[a->reg] = d;
            break;
        }
        case F_ADD: ps2.fpr[a->reg] = ps2_ee_add(ps2.fpr[b->reg], ps2.fpr[c->reg]); break;
        case F_SUB: ps2.fpr[a->reg] = ps2_ee_sub(ps2.fpr[b->reg], ps2.fpr[c->reg]); break;
        case F_MUL: ps2.fpr[a->reg] = ps2_float_mul(ps2.fpr[b->reg], ps2.fpr[c->reg]); break;
        case F_DIV: ps2.fpr[a->reg] = ee_div(ps2.fpr[b->reg], ps2.fpr[c->reg]); break;
        case F_SQRT: ps2.fpr[a->reg] = ee_sqrt(ps2.fpr[b->reg]); break;
        case F_RSQRT: /* rounds toward zero, unlike DIV and SQRT */
            ps2.fpr[a->reg] = ps2_float_div(ps2.fpr[b->reg], ps2_float_sqrt(ps2.fpr[c->reg])); break;
        case F_ABS: ps2.fpr[a->reg] = fabsf(ps2.fpr[b->reg]); break;
        case F_NEG: ps2.fpr[a->reg] = -ps2.fpr[b->reg]; break;
        case F_MOV: ps2.fpr[a->reg] = ps2.fpr[b->reg]; break;
        case F_MAX: ps2.fpr[a->reg] = ps2_float_minmax(ps2.fpr[b->reg], ps2.fpr[c->reg], 1); break;
        case F_MIN: ps2.fpr[a->reg] = ps2_float_minmax(ps2.fpr[b->reg], ps2.fpr[c->reg], 0); break;
        case F_ADDA: ps2.fpu_acc = ps2_ee_add(ps2.fpr[a->reg], ps2.fpr[b->reg]); break;
        case F_SUBA: ps2.fpu_acc = ps2_ee_sub(ps2.fpr[a->reg], ps2.fpr[b->reg]); break;
        case F_MULA: ps2.fpu_acc = ps2_float_mul(ps2.fpr[a->reg], ps2.fpr[b->reg]); break;
        case F_MADDA: { float m = ps2_float_mul(ps2.fpr[a->reg], ps2.fpr[b->reg]); ps2.fpu_acc = ps2_ee_add(ps2.fpu_acc, m); break; }
        case F_MSUBA: { float m = ps2_float_mul(ps2.fpr[a->reg], ps2.fpr[b->reg]); ps2.fpu_acc = ps2_ee_sub(ps2.fpu_acc, m); break; }
        case F_MADD: { float m = ps2_float_mul(ps2.fpr[b->reg], ps2.fpr[c->reg]); ps2.fpr[a->reg] = ps2_ee_add(ps2.fpu_acc, m); break; }
        case F_MSUB: { float m = ps2_float_mul(ps2.fpr[b->reg], ps2.fpr[c->reg]); ps2.fpr[a->reg] = ps2_ee_sub(ps2.fpu_acc, m); break; }
        case V_ADD: case V_SUB: case V_MUL: case V_MAX: case V_MINI: case V_MADD: case V_MSUB:
        case V_ADDA: case V_SUBA: case V_MULA: case V_MADDA: case V_MSUBA:
            vu_arith(in);
            break;
        case V_OPMULA: /* ACC = fs.yzx * ft.zxy */
            vread(b->reg, v);
            vread(c->reg, w);
            ps2.vacc[0] = ps2_float_mul(v[1], w[2]);
            ps2.vacc[1] = ps2_float_mul(v[2], w[0]);
            ps2.vacc[2] = ps2_float_mul(v[0], w[1]);
            break;
        case V_OPMSUB: { /* fd = ACC - fs.yzx * ft.zxy: with vopmula, the cross product */
            float r[4];
            vread(b->reg, v);
            vread(c->reg, w);
            r[0] = ps2_vu_madd(ps2.vacc[0], v[1], w[2], 1);
            r[1] = ps2_vu_madd(ps2.vacc[1], v[2], w[0], 1);
            r[2] = ps2_vu_madd(ps2.vacc[2], v[0], w[1], 1);
            r[3] = 0;
            vwrite(a->reg, in->dest & 7, r);
            break;
        }
        case V_FTOI: case V_ITOF: {
            float scale = (float)(1 << in->arg);
            ps2_q128 src = vraw(b->reg), dst;
            for (i = 0; i < 4; i++) {
                if (in->op == V_FTOI) {
                    double x = (double)src.f[i] * scale;
                    dst.sw[i] = x >= 2147483647.0 ? 2147483647 : x <= -2147483648.0 ? (int32_t)0x80000000 : (int32_t)x;
                } else
                    dst.f[i] = (float)src.sw[i] / scale;
            }
            if (a->reg)
                for (i = 0; i < 4; i++)
                    if (in->dest & (1 << i))
                        ps2.vf[a->reg].w[i] = dst.w[i];
            break;
        }
        case V_MOVE:
            vwrite_raw(a->reg, in->dest, vraw(b->reg));
            break;
        case V_MR32: {
            ps2_q128 s = vraw(b->reg), r;
            r.w[0] = s.w[1]; r.w[1] = s.w[2]; r.w[2] = s.w[3]; r.w[3] = s.w[0];
            vwrite_raw(a->reg, in->dest, r);
            break;
        }
        case V_ABS:
            vread(b->reg, v);
            for (i = 0; i < 4; i++)
                v[i] = fabsf(v[i]);
            vwrite(a->reg, in->dest, v);
            break;
        case V_DIV: /* vdiv Q, fs[f], ft[f] */
            ps2.q = ps2_float_div(field(b, 0), field(c, 0));  /* VU: toward zero */
            break;
        case V_SQRT: /* vsqrt Q, ft[f] */
            ps2.q = ps2_float_sqrt(field(b, 0));
            break;
        case V_RSQRT: /* vrsqrt Q, fs[f], ft[f] */
            ps2.q = ps2_vu_rsqrt(field(b, 0), field(c, 0));
            break;
        case V_RINIT: { /* vrinit R, fs[f] */
            uint32_t bits;
            float f = field(b, 0);
            memcpy(&bits, &f, 4);
            ps2.r = 0x3F800000u | (bits & 0x7FFFFFu);
            break;
        }
        case V_RNEXT: case V_RGET: {
            if (in->op == V_RNEXT) { /* 23-bit LFSR, as the VU does it */
                uint32_t x = (ps2.r >> 4) & 1, y = (ps2.r >> 22) & 1;
                ps2.r = (((ps2.r << 1) ^ x ^ y) & 0x7FFFFFu) | 0x3F800000u;
            }
            if (a->reg)
                for (i = 0; i < 4; i++)
                    if (in->dest & (1 << i))
                        ps2.vf[a->reg].w[i] = ps2.r;
            break;
        }
        case V_IADDI:
            if (a->reg)
                ps2.vi[a->reg] = (uint16_t)(ps2.vi[b->reg] + c->imm);
            break;
        case V_SQI: case V_LQI: { /* vsqi fs, (vit++) / vlqi ft, (vis++) */
            const Opd *m = in->op == V_SQI ? b : a, *r = in->op == V_SQI ? a : b;
            ps2_q128 *cell;
            if (m->step < 0)
                ps2.vi[m->reg]--;
            cell = &ps2.vu0_mem[ps2.vi[m->reg] & 0xFF];
            if (in->op == V_SQI) {
                ps2_q128 src = vraw(r->reg);
                for (i = 0; i < 4; i++)
                    if (in->dest & (1 << i))
                        cell->w[i] = src.w[i];
            } else if (r->reg)
                for (i = 0; i < 4; i++)
                    if (in->dest & (1 << i))
                        ps2.vf[r->reg].w[i] = cell->w[i];
            if (m->step > 0)
                ps2.vi[m->reg]++;
            break;
        }
        case V_CALLMS: {
            ps2_vu0_micro_fn fn = micro[((unsigned)a->imm / 8) % (sizeof micro / sizeof *micro)];
            /* Native microprograms are host implementations of a coprocessor: they round
               like VU0 micro mode (toward zero, denormals flushed; ps2_vu0_run) and keep
               their intermediates outside EE first-fault diagnostics. */
            unsigned control = _mm_getcsr();
            _mm_setcsr((control & ~0x603Fu) | 0xFFC0u);
            if (fn)
                fn(&ps2);
            else if (ps2_vu0_run(&ps2, (unsigned)a->imm)) {
                char at[16];
                sprintf(at, "0x%x", (unsigned)a->imm);
                report(p, "vcallms: the microcode failed at", at);
            }
            _mm_setcsr(control);
            break;
        }
        }
        memset(&ps2.gpr[0], 0, sizeof ps2.gpr[0]); /* $zero */
    }
}

#include "ps2asm_fast.h"

int ps2_asm_check(const char *tmpl, const char *constraints, int n)
{
    int before = problems;
    lookup(tmpl, constraints, n);
    return problems - before;
}

int ps2_exec_check(const char *insn)
{
    int before = problems;
    lookup(insn, RECOMPILED, 0);
    return problems - before;
}

void ps2_exec(const char *insn)
{
    Prog *p = lookup(insn, RECOMPILED, 0);
    if (!p->bad)
        run(p, NULL);
}

void ps2_asm(const char *tmpl, const char *constraints, int n, ps2_op *ops)
{
    Prog *p = lookup(tmpl, constraints, n);
    int k;
    for (k = 0; k < p->n_ops; k++) { /* bind inputs */
        int r = p->alias[k] >= 0 ? p->alias[k] : k;
        unsigned size = ops[k].size;
        if (!p->in[k])
            continue;
        if (p->kind[r] == OPK_GPR) {
            ps2_q128 *g = &ps2.gpr[32 + r];
            memset(g, 0, sizeof *g);
            memcpy(g, ops[k].p, size > 16 ? 16 : size);
            if (size == 4) /* EE registers hold 32-bit values sign-extended */
                g->d[0] = (uint64_t)(int64_t)g->sw[0];
        } else if (p->kind[r] == OPK_FPR)
            memcpy(&ps2.fpr[32 + r], ops[k].p, 4);
    }
    if (p->bad)
        return; /* reported when parsed */
    asm_caller = __builtin_return_address(0);
    if (!run_fast_program(p, ops)) run(p, ops);
    for (k = 0; k < p->n_ops; k++) { /* write outputs back */
        unsigned size = ops[k].size;
        if (!p->out[k])
            continue;
        switch (p->kind[k]) {
        case OPK_GPR: memcpy(ops[k].p, &ps2.gpr[32 + k], size > 16 ? 16 : size); break;
        case OPK_FPR: memcpy(ops[k].p, &ps2.fpr[32 + k], 4); break;
        case OPK_LO: memcpy(ops[k].p, &ps2.lo, size > 8 ? 8 : size); break;
        case OPK_HI: memcpy(ops[k].p, &ps2.hi, size > 8 ? 8 : size); break;
        }
    }
}
