/* Exact decoder output plus ordered primitive callback/full-GS-state replay.
   The reference formulas are the pre-cache native_exact expressions. */
#include "gs.h"
#include "native_frontend.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static Gs left, right;
static NativeExactCache cache;
static unsigned negative, mode, calls, hits, misses;
static uint32_t seed = 0x32140769;
static uint32_t random32(void) { seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5; return seed; }
static uint64_t random64(void) { uint64_t lo = random32(); return lo | (uint64_t)random32() << 32; }
static NativeExactState reference(uint64_t zbuf, uint64_t test, uint64_t frame, unsigned abe)
{
    NativeExactState s = {0};
    s.zbp = (unsigned)zbuf & 511; s.fbp = (unsigned)frame & 511; s.fbw = (unsigned)(frame >> 16) & 63;
    s.zpsm = 48 | ((unsigned)(zbuf >> 24) & 15); s.fpsm = (unsigned)(frame >> 24) & 63;
    s.zwrite = !(zbuf >> 32 & 1); s.colour = (uint32_t)(frame >> 32) != 0xFFFFFFFFu;
    s.never = (test >> 16 & 1) && !(test >> 17 & 3);
    unsigned ztst = test >> 16 & 1 ? (unsigned)(test >> 17) & 3 : 1, ate = test & 1, atst = (unsigned)(test >> 1) & 7;
    unsigned afail = (unsigned)(test >> 12) & 3, date = (unsigned)(test >> 14) & 1;
    s.z_full = s.zwrite && ztst == 1 && !date && (!ate || atst == 1 || afail == 2) && (s.zpsm & 7) != 2;
    s.colour_full = ztst == 1 && !date && (!ate || atst == 1) && !abe &&
                    ((unsigned)(frame >> 24) & 63) == GS_PSMCT32 && !(uint32_t)(frame >> 32);
    return s;
}
static NativeExactState cached(uint64_t zbuf, uint64_t test, uint64_t frame, unsigned abe)
{
    int hit = cache.valid && cache.zbuf == zbuf && cache.test == test && cache.frame == frame && cache.abe == abe;
    hits += hit; misses += !hit;
    NativeExactState s = *native_exact_state(&cache,zbuf,test,frame,abe);
    if (negative && hit) s.colour ^= 1;
    return s;
}
typedef struct {
    NativeExactState state;
    GsVertex vertices[3];
    uint64_t offset, scissor;
    unsigned type, count, prim, ctxt;
} Record;
static Record records[2][8];
static unsigned record_count[2];
static void record(Gs *g, unsigned type, const GsVertex *v, unsigned count)
{
    if (record_count[mode] == 8 || count > 3) abort();
    Record *r = &records[mode][record_count[mode]++];
    memset(r,0,sizeof *r);
    unsigned prim = (unsigned)(g->reg[GS_PRMODECONT]&1 ? g->reg[GS_PRIM] : g->reg[GS_PRMODE]);
    unsigned ctxt = prim >> 9 & 1;
    uint64_t zbuf=g->reg[GS_ZBUF_1+ctxt], test=g->reg[GS_TEST_1+ctxt], frame=g->reg[GS_FRAME_1+ctxt];
    r->state=mode ? cached(zbuf,test,frame,prim>>6&1) : reference(zbuf,test,frame,prim>>6&1);
    memcpy(r->vertices,v,count*sizeof *v);
    r->offset=g->reg[GS_XYOFFSET_1+ctxt]; r->scissor=g->reg[GS_SCISSOR_1+ctxt];
    r->type=type; r->count=count; r->prim=prim; r->ctxt=ctxt; calls++;
}
static void write_pair(unsigned reg, uint64_t value)
{
    mode=0; gs_write(&left,reg,value);
    mode=1; gs_write(&right,reg,value);
}
int main(int argc,char **argv)
{
    (void)argv; negative=argc>1;
    /* Arbitrary raw keys, repeated keys, every key bit, and eligibility boundaries. */
    for(unsigned n=0;n<100000;n++) {
        uint64_t z=random64(),t=random64(),f=random64(); unsigned a=n&1;
        if (n&2) f &= 0x00ffffffu;
        if (n&4) t &= 0x3ffffu;
        if (n&8) t = (n>>4)&0x3ffffu;
        for(unsigned repeat=0;repeat<2;repeat++) {
            NativeExactState expected=reference(z,t,f,a), actual=cached(z,t,f,a);
            if(memcmp(&expected,&actual,sizeof expected)) { puts("decoder full-state DIFF"); return 1; }
        }
    }
    gs_reset(&left); gs_reset(&right);
    for(unsigned i=0;i<GS_VRAM_SIZE/4;i++) left.vram[i]=right.vram[i]=random32();
    left.draw=right.draw=record;
    static const unsigned regs[]={GS_ZBUF_1,GS_ZBUF_2,GS_TEST_1,GS_TEST_2,GS_FRAME_1,GS_FRAME_2,
        GS_PRMODE,GS_PRMODECONT,GS_XYOFFSET_1,GS_XYOFFSET_2,GS_SCISSOR_1,GS_SCISSOR_2};
    for(unsigned n=0;n<20000;n++) {
        memset(records,0,sizeof records); memset(record_count,0,sizeof record_count);
        write_pair(regs[n%12],random64());
        write_pair(GS_PRIM,3 | ((n&31)<<3));
        write_pair(GS_RGBAQ,random64());
        for(unsigned v=0;v<6;v++) write_pair(GS_XYZ2,random64());
        if(record_count[0]!=record_count[1] || memcmp(records[0],records[1],sizeof records[0])) {
            puts("ordered primitive/metadata DIFF"); return 1;
        }
        /* Includes VRAM, paths, vertices, registers, epochs, callbacks and provenance. */
        if(!(n%128) && memcmp(&left,&right,sizeof left)) { puts("full GS state/VRAM DIFF"); return 1; }
    }
    if(memcmp(&left,&right,sizeof left) || !hits || !misses || !calls) return 1;
    printf("MATCH: 200000 decoded states, 20000 primitive groups, %u callbacks, %u hits/%u misses; full GS/VRAM\n",calls,hits,misses);
    return 0;
}
