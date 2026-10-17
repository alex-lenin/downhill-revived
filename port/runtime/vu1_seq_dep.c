/* Kick dependency analysis over a captured VU1 run sequence (DHD_VU_SEQ, port/hle/vif.c).

   A parallel VU1 would start run j while runs j-d..j-1 are still going. Its dispatcher has done
   every VIF1 upload in order, so it knows the data memory apart from what those runs write, and
   the registers as they were before run j-d. For d = 1..3 this replays run j from that state and
   compares with replaying it from its real state: the GIF packets it kicks, and every register
   and data qword it changes. Equal means run j did not need the in-flight runs' results this
   time (evidence, not proof). Inputs are local game captures; nothing here embeds game data.

   usage: vu1_seq_dep DIR [max_runs] > report.csv  (one line per run, then a summary on stderr) */
#include "ps2asm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define DEPTH 3
typedef struct { unsigned header[8]; ps2_cpu cpu; ps2_q128 data[1024]; } Run;
typedef struct { Run before; ps2_cpu after; ps2_q128 data_after[1024]; uint32_t gif; unsigned kicks; } Slot;

static ps2_q128 *data;
static uint32_t gif_hash;
static unsigned gif_kicks;

static uint32_t fnv(uint32_t h, const void *p, unsigned n)
{
    const unsigned char *b = p;
    for (unsigned i = 0; i < n; i++) h = (h ^ b[i]) * 16777619u;
    return h;
}

/* XGKICK: hash the GIF packet at `address` (tags and data, through the EOP tag's data). */
static int kick(void *opaque, unsigned address)
{
    (void)opaque;
    gif_kicks++;
    for (unsigned guard = 0; guard < 1024;) {
        const ps2_q128 *tag = &data[address & 1023];
        uint64_t t = tag->d[0];
        unsigned nloop = t & 0x7FFF, eop = t >> 15 & 1, flg = t >> 58 & 3, nreg = t >> 60 & 15;
        unsigned qw = flg == 0 ? nloop * (nreg ? nreg : 16) : flg == 1 ? (nloop * (nreg ? nreg : 16) + 1) / 2 : nloop;
        gif_hash = fnv(gif_hash, tag, 16);
        address++; guard++;
        for (unsigned i = 0; i < qw && guard < 1024; i++, address++, guard++) gif_hash = fnv(gif_hash, &data[address & 1023], 16);
        if (eop) break;
    }
    return 0;
}

static uint8_t *code_base;
static uint32_t loaded_hash;
static const char *dir;

static void load_code(uint32_t hash)
{
    if (hash == loaded_hash) return;
    char path[1024];
    snprintf(path, sizeof path, "%s/code-%08X.bin", dir, hash);
    FILE *f = fopen(path, "rb");
    if (!f || fread(code_base, 0x4000, 1, f) != 1) { fprintf(stderr, "missing %s\n", path); exit(2); }
    fclose(f);
    loaded_hash = hash; ps2_vu1_code_version++;
}

/* The guest registers a VU1 run can see (no host pointers, no EE state). */
#define REGS(c) X(vf) X(vacc) X(vi) X(q) X(i) X(r) X(vu0_ctrl) X(p)
static void copy_regs(ps2_cpu *to, const ps2_cpu *from)
{
#define X(f) memcpy(&to->f, &from->f, sizeof to->f);
    REGS(0)
#undef X
}
static int regs_differ_where(const ps2_cpu *a, const ps2_cpu *b, const ps2_cpu *c, const ps2_cpu *d)
{   /* any register changed by (a->b) or (c->d) that ends differently in b and d */
    int bad = 0;
#define X(f) { const uint8_t *pa = (const uint8_t *)&a->f, *pb = (const uint8_t *)&b->f, *pc = (const uint8_t *)&c->f, *pd = (const uint8_t *)&d->f; \
    for (unsigned k = 0; k < sizeof a->f; k++) if ((pa[k] != pb[k] || pc[k] != pd[k]) && pb[k] != pd[k]) bad = 1; }
    REGS(0)
#undef X
    return bad;
}
/* Which register groups changed between two states: bit 0 vf, 1 acc, 2 vi, 3 q/p/i/r, 4 control. */
static unsigned reg_groups(const ps2_cpu *a, const ps2_cpu *b)
{
    unsigned g = 0;
    if (memcmp(a->vf, b->vf, sizeof a->vf)) g |= 1;
    if (memcmp(a->vacc, b->vacc, sizeof a->vacc)) g |= 2;
    if (memcmp(a->vi, b->vi, sizeof a->vi)) g |= 4;
    if (a->q != b->q || a->p != b->p || a->i != b->i || a->r != b->r) g |= 8;
    if (memcmp(a->vu0_ctrl, b->vu0_ctrl, sizeof a->vu0_ctrl)) g |= 16;
    return g;
}

static void run(const Run *r, ps2_cpu *cpu, ps2_q128 *mem, uint32_t *gif, unsigned *kicks)
{
    load_code(r->header[4]);
    cpu->vu0_code = code_base; cpu->vu0_mem = mem;
    data = mem; gif_hash = 2166136261u; gif_kicks = 0;
    if (ps2_vu1_run(cpu, r->header[0], r->header[1], r->header[2], kick, NULL)) { fprintf(stderr, "VU failure\n"); exit(1); }
    *gif = gif_hash; *kicks = gif_kicks;
}

/* Replays runs a..b-1 from (cpu, mem) with the real VIF uploads between them; 1 when every
   GIF packet and the chain's own register and memory writes match the real runs. */
static int replay_chain(const Slot *runs, unsigned a, unsigned b, ps2_cpu *cpu_in, ps2_q128 *mem)
{
    static ps2_q128 begin[1024];
    ps2_cpu cpu = *cpu_in;
    memcpy(begin, mem, sizeof begin);
    ps2_cpu begin_cpu = cpu;
    int same = 1;
    for (unsigned i = a; i < b && same; i++) {
        if (i > a)  /* the VIF uploads between the two runs */
            for (unsigned p = 0; p < 1024; p++)
                if (memcmp(&runs[i - 1].data_after[p], &runs[i].before.data[p], 16)) mem[p] = runs[i].before.data[p];
        uint32_t gif; unsigned kicks;
        run(&runs[i].before, &cpu, mem, &gif, &kicks);
        same = gif == runs[i].gif && kicks == runs[i].kicks;
    }
    if (same && regs_differ_where(&runs[a].before.cpu, &runs[b - 1].after, &begin_cpu, &cpu)) same = 0;
    for (unsigned p = 0; same && p < 1024; p++)
        if ((memcmp(&runs[a].before.data[p], &runs[b - 1].data_after[p], 16) || memcmp(&begin[p], &mem[p], 16)) &&
            memcmp(&mem[p], &runs[b - 1].data_after[p], 16))
            same = 0;
    *cpu_in = cpu;
    return same;
}

/* Object level (DEP_CHAIN=1): a chain is an MSCAL run and the MSCNT runs after it. A parallel
   VU1 would hand whole chains to workers. Chain c starts from the registers before chain c-D
   (D chains in flight) apart from the carried VI registers DEP_CARRY (bit mask, default vi2),
   which the dispatcher would have to know, and from the data memory without those chains' VU
   writes. Within the chain the worker continues its own state, with each VIF upload applied. */
static int chain_mode(FILE *f, unsigned max)
{
    unsigned n = 0, cap = 0;
    Slot *runs = NULL;
    while (n < max) {
        if (n == cap) { cap = cap ? cap * 2 : 1024; runs = realloc(runs, cap * sizeof *runs); if (!runs) return 2; }
        if (fread(&runs[n].before, sizeof runs[n].before, 1, f) != 1) break;
        Slot *s = &runs[n++];
        s->after = s->before.cpu; memcpy(s->data_after, s->before.data, sizeof s->data_after);
        run(&s->before, &s->after, s->data_after, &s->gif, &s->kicks);
    }
    unsigned carry = getenv("DEP_CARRY") ? (unsigned)strtoul(getenv("DEP_CARRY"), NULL, 0) : 1u << 2;
    unsigned *start = malloc((n + 1) * sizeof *start), chains = 0;
    for (unsigned i = 0; i < n; i++)
        if (!runs[i].before.header[7] || !i) start[chains++] = i;
    start[chains] = n;
    static ps2_q128 mem[1024], begin[1024];
    struct { unsigned program, count, ok[DEPTH + 1]; } stats[64] = {{0}};
    for (unsigned c = 1; c + 1 <= chains - 1; c++) {  /* the last chain may be cut off */
        unsigned a = start[c], b = start[c + 1];
        unsigned program = runs[a].before.header[6], k;
        for (k = 0; k < 64 && stats[k].count && stats[k].program != program; k++) {}
        if (k == 64) continue;
        stats[k].program = program; stats[k].count++;
        for (unsigned d = 1; d <= DEPTH && d <= c; d++) {
            unsigned first = start[c - d];
            ps2_cpu cpu = runs[a].before.cpu;
            copy_regs(&cpu, &runs[first].before.cpu);
            for (int v = 0; v < 16; v++) if (carry >> v & 1) cpu.vi[v] = runs[a].before.cpu.vi[v];
            memcpy(mem, runs[a].before.data, sizeof mem);
            for (unsigned p = 0; p < 1024; p++) {
                int last = -1;
                for (unsigned i = first; i < a; i++) if (memcmp(&runs[i].before.data[p], &runs[i].data_after[p], 16)) last = (int)i;
                if (last >= 0 && !memcmp(&runs[a].before.data[p], &runs[last].data_after[p], 16))
                    mem[p] = runs[first].before.data[p];
            }
            int same = replay_chain(runs, a, b, &cpu, mem);
            stats[k].ok[d] += same;
            static int explain = -1;
            if (explain < 0) explain = getenv("DEP_EXPLAIN") ? atoi(getenv("DEP_EXPLAIN")) : 0;
            if (!same && d == 1 && explain > 0) {
                explain--;
                const ps2_cpu *o = &runs[first].before.cpu, *t = &runs[a].before.cpu;
                fprintf(stderr, "chain at run %u program %04X (%u runs) needs:", a, program, b - a);
                for (int item = 0; item < 32 + 1 + 16 + 4 + 16 + 1; item++) {
                    ps2_cpu x = *t;
                    char name[48];
                    memcpy(mem, runs[a].before.data, sizeof mem);
                    if (item < 32) { if (!memcmp(&x.vf[item], &o->vf[item], 16)) continue; x.vf[item] = o->vf[item]; sprintf(name, "vf%d", item); }
                    else if (item == 32) { if (!memcmp(x.vacc, o->vacc, 16)) continue; memcpy(x.vacc, o->vacc, 16); strcpy(name, "acc"); }
                    else if (item < 49) { int v = item - 33; if (x.vi[v] == o->vi[v]) continue;
                        sprintf(name, "vi%d(%u->%u)", v, o->vi[v], x.vi[v]); x.vi[v] = o->vi[v]; }
                    else if (item == 49) { if (x.q == o->q) continue; x.q = o->q; strcpy(name, "q"); }
                    else if (item == 50) { if (x.p == o->p) continue; x.p = o->p; strcpy(name, "p"); }
                    else if (item == 51) { if (x.i == o->i) continue; x.i = o->i; strcpy(name, "i"); }
                    else if (item == 52) { if (x.r == o->r) continue; x.r = o->r; strcpy(name, "r"); }
                    else if (item < 69) { int v = item - 53; if (x.vu0_ctrl[v] == o->vu0_ctrl[v]) continue;
                        sprintf(name, "ctrl%d(%X->%X)", v, o->vu0_ctrl[v], x.vu0_ctrl[v]); x.vu0_ctrl[v] = o->vu0_ctrl[v]; }
                    else {
                        unsigned m = 0, lo = 9999, hi = 0;
                        for (unsigned p = 0; p < 1024; p++) {
                            int last = -1;
                            for (unsigned i = first; i < a; i++) if (memcmp(&runs[i].before.data[p], &runs[i].data_after[p], 16)) last = (int)i;
                            if (last >= 0 && !memcmp(&runs[a].before.data[p], &runs[last].data_after[p], 16)) {
                                mem[p] = runs[first].before.data[p]; m++; if (p < lo) lo = p; if (p > hi) hi = p;
                            }
                        }
                        if (!m) continue;
                        sprintf(name, "mem[%u qw %u-%u]", m, lo, hi);
                    }
                    if (!replay_chain(runs, a, b, &x, mem)) fprintf(stderr, " %s", name);
                }
                fprintf(stderr, " (previous chain program %04X)\n", runs[first].before.header[6]);
            }
        }
    }
    printf("program,chains,indep1,indep2,indep3\n");
    for (unsigned k = 0; k < 64 && stats[k].count; k++)
        printf("%04X,%u,%u,%u,%u\n", stats[k].program, stats[k].count, stats[k].ok[1], stats[k].ok[2], stats[k].ok[3]);
    fprintf(stderr, "%u runs, %u chains, carried VI mask 0x%X\n", n, chains, carry);
    return 0;
}

int main(int argc, char **argv)
{
    if (argc < 2) { fprintf(stderr, "usage: vu1_seq_dep DIR [max_runs]\n"); return 2; }
    dir = argv[1];
    unsigned max = argc > 2 ? (unsigned)atoi(argv[2]) : ~0u;
    void *mapped = VirtualAlloc((void *)0x11000000, 0x10000, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (mapped != (void *)0x11000000) { fprintf(stderr, "VU1 address unavailable\n"); return 2; }
    code_base = (uint8_t *)mapped + 0x8000;
    char path[1024];
    snprintf(path, sizeof path, "%s/seq.bin", dir);
    FILE *f = fopen(path, "rb");
    if (!f) { perror(path); return 2; }
    if (getenv("DEP_CHAIN")) return chain_mode(f, max);
    static Slot ring[DEPTH + 1];
    static ps2_q128 mem[1024];
    unsigned long long ok[DEPTH + 1] = {0}, tried[DEPTH + 1] = {0};
    puts("run,field,entry,top,itop,kicks,dep1,dep2,dep3,regs1,mem1");
    for (unsigned j = 0; j < max; j++) {
        Slot *s = &ring[j % (DEPTH + 1)];
        if (fread(&s->before, sizeof s->before, 1, f) != 1) break;
        if (s->before.header[3] != sizeof(ps2_cpu)) { fprintf(stderr, "state size mismatch\n"); return 2; }
        /* the real run */
        s->after = s->before.cpu; memcpy(s->data_after, s->before.data, sizeof mem);
        run(&s->before, &s->after, s->data_after, &s->gif, &s->kicks);
        int dep[DEPTH + 1] = {0};
        unsigned groups = 0, memw = 0;
        for (unsigned d = 1; d <= DEPTH && d <= j; d++) {
            const Slot *first = &ring[(j - d) % (DEPTH + 1)];
            ps2_cpu cpu = s->before.cpu;
            copy_regs(&cpu, &first->before.cpu);
            memcpy(mem, s->before.data, sizeof mem);
            unsigned written = 0;
            for (unsigned p = 0; p < 1024; p++) {
                int last = -1;
                for (unsigned i = j - d; i < j; i++) {
                    const Slot *w = &ring[i % (DEPTH + 1)];
                    if (memcmp(&w->before.data[p], &w->data_after[p], 16)) last = (int)i;
                }
                if (last < 0) continue;
                const Slot *w = &ring[last % (DEPTH + 1)];
                if (!memcmp(&s->before.data[p], &w->data_after[p], 16)) {  /* not uploaded over since */
                    mem[p] = first->before.data[p]; written++;
                }
            }
            if (d == 1) { groups = reg_groups(&first->before.cpu, &s->before.cpu); memw = written; }
            static ps2_q128 start[1024];
            memcpy(start, mem, sizeof start);
            uint32_t gif; unsigned kicks;
            run(&s->before, &cpu, mem, &gif, &kicks);
            int same = gif == s->gif && kicks == s->kicks &&
                       !regs_differ_where(&s->before.cpu, &s->after, &first->before.cpu, &cpu);
            /* every qword run j writes (in either replay) must end the same */
            for (unsigned p = 0; same && p < 1024; p++)
                if ((memcmp(&s->before.data[p], &s->data_after[p], 16) || memcmp(&start[p], &mem[p], 16)) &&
                    memcmp(&mem[p], &s->data_after[p], 16))
                    same = 0;
            dep[d] = !same; tried[d]++; ok[d] += same;
        }
        static int explain = -1;
        if (explain < 0) explain = getenv("DEP_EXPLAIN") ? atoi(getenv("DEP_EXPLAIN")) : 0;
        if (dep[1] && explain > 0) {
            /* Which single register (or the in-flight memory) carries the dependency: replay the
               real state with only that item taken from before run j-1. */
            explain--;
            const Slot *first = &ring[(j - 1) % (DEPTH + 1)];
            fprintf(stderr, "run %u entry %04X needs:", j, s->before.header[0]);
            for (int item = 0; item < 32 + 1 + 16 + 4 + 16 + 1; item++) {
                ps2_cpu cpu = s->before.cpu;
                const ps2_cpu *o = &first->before.cpu;
                memcpy(mem, s->before.data, sizeof mem);
                char name[32];
                if (item < 32) { if (!memcmp(&cpu.vf[item], &o->vf[item], 16)) continue; cpu.vf[item] = o->vf[item]; sprintf(name, "vf%d", item); }
                else if (item == 32) { if (!memcmp(cpu.vacc, o->vacc, 16)) continue; memcpy(cpu.vacc, o->vacc, 16); strcpy(name, "acc"); }
                else if (item < 49) { int k = item - 33; if (cpu.vi[k] == o->vi[k]) continue;
                    sprintf(name, "vi%d(%u->%u)", k, o->vi[k], cpu.vi[k]); cpu.vi[k] = o->vi[k]; }
                else if (item == 49) { if (cpu.q == o->q) continue; cpu.q = o->q; strcpy(name, "q"); }
                else if (item == 50) { if (cpu.p == o->p) continue; cpu.p = o->p; strcpy(name, "p"); }
                else if (item == 51) { if (cpu.i == o->i) continue; cpu.i = o->i; strcpy(name, "i"); }
                else if (item == 52) { if (cpu.r == o->r) continue; cpu.r = o->r; strcpy(name, "r"); }
                else if (item < 69) { int k = item - 53; if (cpu.vu0_ctrl[k] == o->vu0_ctrl[k]) continue;
                    sprintf(name, "ctrl%d(%X->%X)", k, o->vu0_ctrl[k], cpu.vu0_ctrl[k]); cpu.vu0_ctrl[k] = o->vu0_ctrl[k]; }
                else {
                    unsigned n = 0, lo = 9999, hi = 0;
                    for (unsigned p = 0; p < 1024; p++)
                        if (memcmp(&first->before.data[p], &first->data_after[p], 16) &&
                            !memcmp(&s->before.data[p], &first->data_after[p], 16)) {
                            mem[p] = first->before.data[p]; n++; if (p < lo) lo = p; if (p > hi) hi = p;
                        }
                    if (!n) continue;
                    sprintf(name, "mem[%u qw %u-%u]", n, lo, hi);
                }
                uint32_t gif; unsigned kicks;
                ps2_cpu end = cpu;
                static ps2_q128 out[1024];
                memcpy(out, mem, sizeof out);
                run(&s->before, &end, out, &gif, &kicks);
                if (gif != s->gif || kicks != s->kicks) fprintf(stderr, " %s", name);
            }
            fprintf(stderr, " (top %u itop %u, prev entry %04X top %u)\n", s->before.header[1], s->before.header[2],
                    first->before.header[0], first->before.header[1]);
        }
        printf("%u,%u,%04X,%u,%u,%u,%d,%d,%d,%u,%u\n", j, s->before.header[5], s->before.header[0], s->before.header[1],
               s->before.header[2], s->kicks, dep[1], dep[2], dep[3], groups, memw);
    }
    fclose(f);
    for (unsigned d = 1; d <= DEPTH; d++)
        fprintf(stderr, "depth %u: %llu of %llu runs independent of the %u before (%.1f%%)\n", d, ok[d], tried[d], d,
                tried[d] ? 100.0 * ok[d] / tried[d] : 0);
    return 0;
}
