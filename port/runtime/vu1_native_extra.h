/* Additional mesh loops sharing the native clipping routine. */
#include "vu1_native_1430.h"
#include "vu1_native_2dc8.h"
static unsigned native_geometry_1638(Vu *u)
{
    unsigned begin = u->c->vi[8], end = u->c->vi[10];
    unsigned distance = (end - begin) & 0xffff;
    if (!distance || distance % 3 || distance / 3 > 64 || (u->q_ready && u->q_ready > u->cycle)) { return 0; }
    ps2_q128 *memory = u->c->vu0_mem;
#define cpu (*u->c)
    drain_flags(u); update_q(u);
    unsigned entry_status = u->status;
    __m128 v[32], acc = _mm_loadu_ps(cpu.vacc);
    for (unsigned i = 0; i < 32; i++) v[i] = vf4(u, i);
    NativeFlags flags = {_mm_setzero_si128(), _mm_setzero_si128(), _mm_setzero_si128(), _mm_setzero_ps(), 0};
    unsigned clip = u->clip, pairs = 0, index = begin;
    uint16_t *vi = cpu.vi;
    float q = cpu.q; unsigned q_flags = 0;
#define NATIVE_SAVE() do { \
    for (unsigned i = 1; i < 32; i++) _mm_storeu_ps(cpu.vf[i].f,v[i]); \
    _mm_storeu_ps(cpu.vacc,acc); cpu.q = q; \
    Upper last = {0}; fmac_flags4(&last,flags.last,flags.mask); \
    unsigned mac = last.mac; \
    unsigned status = (mac & 15 ? 1 : 0) | (mac & 240 ? 2 : 0) | (mac & 3840 ? 4 : 0) | (mac & 61440 ? 8 : 0); \
    unsigned sticky = (_mm_movemask_ps(_mm_castsi128_ps(flags.zero)) ? 1 : 0) | \
                      (_mm_movemask_ps(_mm_castsi128_ps(flags.sign)) ? 2 : 0) | \
                      (_mm_movemask_ps(_mm_castsi128_ps(flags.overflow)) ? 8 : 0); \
    u->status = (entry_status & ~0x3fu) | status | q_flags | (sticky << 6); \
    u->mac = mac; u->clip = u->clip_next = clip; u->n_flags = 0; u->flag_head = 0; \
    u->q_ready = 0; u->cycle += 64; u->flag_cycle = u->cycle; \
} while(0)
#define NADD(a,b,d) native_add(&flags,(a),(b),(d))
#define NSUB(a,b,d) native_sub(&flags,(a),(b),(d))
#define NMUL(a,b,d) native_mul(&flags,(a),(b),(d))
#define NMADD(a,b,c,d) native_madd(&flags,(a),(b),(c),(d))
#define SET(r,x,d) v[r] = native_merge(v[r], (x), (d))
#define LOAD(a) _mm_loadu_ps(memory[(a) & 1023].f)
#define STORE(a,x,d) store_fields(memory[(a) & 1023].f, _mm_castps_si128(x), (d))
    do {
        v[12]=NMUL(v[10],v[31],15); vi[9]=(uint16_t)memory[(index+6)&1023].w[3];
        SET(5,NADD(v[13],lane4(v[25],3),2),2); STORE(index,v[8],14);
        SET(5,NADD(v[24],v[13],12),12);
        float denominator = _mm_cvtss_f32(arithmetic4(lane4(v[26],3)));
        q_flags = denominator == 0 ? 0x20 : 0;
        q = denominator == 0 ? vu_max(signbit(denominator)) :
            _mm_cvtss_f32(arithmetic4(_mm_div_ss(_mm_set_ss(1),_mm_set_ss(denominator))));
        SET(24,minmax4(v[24],lane4(v[18],1),0),1); v[25]=v[26];
        v[12]=minmax4(v[12],_mm_set1_ps(cpu.i),0); v[10]=LOAD(index+4);
        acc=NMUL(v[0],v[22],15); v[20]=LOAD(181+vi[9]);
        clip=((clip<<6)|native_clip(v[5],lane4(v[13],3)))&0xffffff;
        SET(26,lane4(v[18],3),1);
        SET(24,minmax4(v[24],lane4(v[0],0),1),1); v[9]=LOAD(index+8);
        v[12]=native_ftoi(v[12],1); v[28]=v[5];
        v[10]=native_itof(v[10],1); vi[1]=q_flags;
        v[23]=NMADD(acc,v[26],_mm_set1_ps(q),15); vi[1]=(clip&0x02fbef)!=0;
        SET(24,NADD(v[11],lane4(v[24],3),1),1);
        if(q_flags) { SET(8,NADD(v[7],lane4(v[0],0),14),14); }
        else { SET(8,NMUL(v[7],_mm_set1_ps(q),14),14); }
        STORE(index+1,v[12],15); native_vertices++;
        if (vi[1]) {
            native_clipped_vertices++;
            NATIVE_SAVE();
            unsigned clipped = native_clip_run(u, 0x1778/8);
            if (!clipped) return 0;
            pairs += clipped;
            for (unsigned i=0;i<32;i++) v[i]=vf4(u,i);
            acc=_mm_loadu_ps(cpu.vacc); q=cpu.q;
            clip=u->clip; entry_status=u->status; q_flags=u->status&0x30;
            memset(&flags,0,sizeof flags);
        }
        acc=NMUL(v[4],lane4(v[0],3),15); v[11]=v[7];
        v[7]=native_itof(v[9],1.0f/4096); index=(index+3)&65535;vi[8]=(uint16_t)index;
        v[30]=native_ftoi(v[24],16);
        acc=NMADD(acc,v[1],lane4(v[20],0),15);v[29]=v[28];
        acc=NMADD(acc,v[2],lane4(v[20],1),15);v[24]=v[23];
        v[26]=NMADD(acc,v[3],lane4(v[20],2),15);
        SET(7,NADD(v[7],v[19],12),12);STORE(index-1,v[30],15);
        pairs+=21;
    } while (index != end);
    NATIVE_SAVE();
    return pairs;
#undef NATIVE_SAVE
#undef cpu
#undef NADD
#undef NSUB
#undef NMUL
#undef NMADD
#undef SET
#undef LOAD
#undef STORE
}

static unsigned native_geometry_17e0(Vu *u)
{
    unsigned begin = u->c->vi[8], end = u->c->vi[10];
    unsigned distance = (end - begin) & 0xffff;
    if (!distance || distance % 3 || distance / 3 > 64) { return 0; }
    ps2_q128 *memory = u->c->vu0_mem;
#define cpu (*u->c)
    drain_flags(u); update_q(u);
    unsigned entry_status = u->status;
    __m128 v[32], acc = _mm_loadu_ps(cpu.vacc);
    for (unsigned i = 0; i < 32; i++) v[i] = vf4(u, i);
    NativeFlags flags = {_mm_setzero_si128(), _mm_setzero_si128(), _mm_setzero_si128(), _mm_setzero_ps(), 0};
    unsigned clip = u->clip, pairs = 0, index = begin;
    uint16_t *vi = cpu.vi;
    float q = cpu.q; unsigned q_flags = u->status & 0x30;
#define NATIVE_SAVE() do { \
    for (unsigned i = 1; i < 32; i++) _mm_storeu_ps(cpu.vf[i].f,v[i]); \
    _mm_storeu_ps(cpu.vacc,acc); cpu.q = q; \
    Upper last = {0}; fmac_flags4(&last,flags.last,flags.mask); \
    unsigned mac = last.mac; \
    unsigned status = (mac & 15 ? 1 : 0) | (mac & 240 ? 2 : 0) | (mac & 3840 ? 4 : 0) | (mac & 61440 ? 8 : 0); \
    unsigned sticky = (_mm_movemask_ps(_mm_castsi128_ps(flags.zero)) ? 1 : 0) | \
                      (_mm_movemask_ps(_mm_castsi128_ps(flags.sign)) ? 2 : 0) | \
                      (_mm_movemask_ps(_mm_castsi128_ps(flags.overflow)) ? 8 : 0); \
    u->status = (entry_status & ~0x3fu) | status | q_flags | (sticky << 6); \
    u->mac = mac; u->clip = u->clip_next = clip; u->n_flags = 0; u->flag_head = 0; \
    u->q_ready = 0; u->cycle += 64; u->flag_cycle = u->cycle; \
} while(0)
#define NADD(a,b,d) native_add(&flags,(a),(b),(d))
#define NSUB(a,b,d) native_sub(&flags,(a),(b),(d))
#define NMUL(a,b,d) native_mul(&flags,(a),(b),(d))
#define NMADD(a,b,c,d) native_madd(&flags,(a),(b),(c),(d))
#define SET(r,x,d) v[r] = native_merge(v[r], (x), (d))
#define LOAD(a) _mm_loadu_ps(memory[(a) & 1023].f)
#define STORE(a,x,d) store_fields(memory[(a) & 1023].f, _mm_castps_si128(x), (d))
    do {
        acc=NMUL(v[4],lane4(v[0],3),15);v[9]=LOAD(index+8);
        acc=NMADD(acc,v[1],lane4(v[20],0),15);vi[9]=(uint16_t)memory[(index+9)&1023].w[3];
        clip=((clip<<6)|native_clip(v[5],lane4(v[13],3)))&0xffffff;SET(24,v[23],14);
        acc=NMADD(acc,v[2],lane4(v[20],1),15);STORE(index,v[8],14);
        SET(24,NADD(v[11],lane4(v[14],3),1),1);v[25]=v[26];
        v[28]=NADD(v[5],lane4(v[0],0),15);vi[1]=q_flags;
        SET(23,NMUL(v[26],_mm_set1_ps(q),14),14);
        v[26]=NMADD(acc,v[3],lane4(v[20],2),15);vi[1]=(clip&0x02fbef)!=0;
        if(q_flags) { SET(8,NADD(v[7],lane4(v[0],0),14),14); }
        else { SET(8,NMUL(v[7],_mm_set1_ps(q),14),14); }
        v[20]=LOAD(181+vi[9]);native_vertices++;
        if (vi[1]) {
            native_clipped_vertices++;
            NATIVE_SAVE();
            unsigned clipped = native_clip_run(u, 0x1900/8);
            if (!clipped) return 0;
            pairs += clipped;
            for (unsigned i=0;i<32;i++) v[i]=vf4(u,i);
            acc=_mm_loadu_ps(cpu.vacc); q=cpu.q;
            clip=u->clip; entry_status=u->status; q_flags=u->status&0x30;
            memset(&flags,0,sizeof flags);
        }
        v[30]=native_ftoi(v[24],16);v[11]=v[7];
        SET(5,NADD(v[13],lane4(v[25],3),2),2);index=(index+3)&65535;vi[8]=(uint16_t)index;
        SET(5,NADD(v[23],v[13],12),12);
        float denominator = _mm_cvtss_f32(arithmetic4(lane4(v[26],3)));
        q_flags = denominator == 0 ? 0x20 : 0;
        q = denominator == 0 ? vu_max(signbit(denominator)) :
            _mm_cvtss_f32(arithmetic4(_mm_div_ss(_mm_set_ss(1),_mm_set_ss(denominator))));
        v[7]=native_itof(v[9],1.0f/4096);
        v[29]=NMUL(v[28],lane4(v[0],3),15);STORE(index-1,v[30],15);
        pairs+=15;
    } while (index != end);
    NATIVE_SAVE();
    return pairs;
#undef NATIVE_SAVE
#undef cpu
#undef NADD
#undef NSUB
#undef NMUL
#undef NMADD
#undef SET
#undef LOAD
#undef STORE
}

static unsigned native_geometry_20c(Vu *u)
{
    unsigned begin = u->c->vi[8], end = u->c->vi[10];
    unsigned distance = (end - begin) & 0xffff;
    if (!distance || distance % 3 || distance / 3 > 64) { return 0; }
    ps2_q128 *memory = u->c->vu0_mem;
#define cpu (*u->c)
    drain_flags(u); update_q(u);
    unsigned entry_status = u->status;
    __m128 v[32], acc = _mm_loadu_ps(cpu.vacc);
    for (unsigned i = 0; i < 32; i++) v[i] = vf4(u, i);
    NativeFlags flags = {_mm_setzero_si128(), _mm_setzero_si128(), _mm_setzero_si128(), _mm_setzero_ps(), 0};
    unsigned clip = u->clip, pairs = 0, index = begin;
    uint16_t *vi = cpu.vi;
    float q = cpu.q; unsigned q_flags = u->status & 0x30;
#define NATIVE_SAVE() do { \
    for (unsigned i = 1; i < 32; i++) _mm_storeu_ps(cpu.vf[i].f,v[i]); \
    _mm_storeu_ps(cpu.vacc,acc); cpu.q = q; \
    Upper last = {0}; fmac_flags4(&last,flags.last,flags.mask); \
    unsigned mac = last.mac; \
    unsigned status = (mac & 15 ? 1 : 0) | (mac & 240 ? 2 : 0) | (mac & 3840 ? 4 : 0) | (mac & 61440 ? 8 : 0); \
    unsigned sticky = (_mm_movemask_ps(_mm_castsi128_ps(flags.zero)) ? 1 : 0) | \
                      (_mm_movemask_ps(_mm_castsi128_ps(flags.sign)) ? 2 : 0) | \
                      (_mm_movemask_ps(_mm_castsi128_ps(flags.overflow)) ? 8 : 0); \
    u->status = (entry_status & ~0x3fu) | status | q_flags | (sticky << 6); \
    u->mac = mac; u->clip = u->clip_next = clip; u->n_flags = 0; u->flag_head = 0; \
    u->q_ready = 0; u->cycle += 64; u->flag_cycle = u->cycle; \
} while(0)
#define NADD(a,b,d) native_add(&flags,(a),(b),(d))
#define NSUB(a,b,d) native_sub(&flags,(a),(b),(d))
#define NMUL(a,b,d) native_mul(&flags,(a),(b),(d))
#define NMADD(a,b,c,d) native_madd(&flags,(a),(b),(c),(d))
#define SET(r,x,d) v[r] = native_merge(v[r], (x), (d))
#define LOAD(a) _mm_loadu_ps(memory[(a) & 1023].f)
#define STORE(a,x,d) store_fields(memory[(a) & 1023].f, _mm_castps_si128(x), (d))
    do {
        acc=NMUL(v[0],v[22],15);v[12]=LOAD(index+4);
        v[24]=NMADD(acc,v[25],_mm_set1_ps(q),15);vi[9]=(uint16_t)memory[(index+3)&1023].w[3];
        SET(5,NADD(v[13],lane4(v[30],3),2),2);vi[1]=q_flags&0x20;
        SET(10,NMUL(v[9],_mm_set1_ps(q),14),14);
        v[7]=native_itof(v[12],1);cpu.i=255;
        if(vi[1])SET(10,NADD(v[9],lane4(v[0],0),14),14);
        SET(5,NADD(v[24],v[13],12),12);SET(11,v[9],1);
        SET(24,minmax4(v[24],lane4(v[18],1),0),1);v[15]=LOAD(373+vi[9]);
        v[23]=minmax4(v[23],_mm_set1_ps(cpu.i),0);v[20]=LOAD(181+vi[9]);
        acc=NMUL(v[4],lane4(v[0],3),15);STORE(index,v[10],14);
        clip=((clip<<6)|native_clip(v[5],lane4(v[13],3)))&0xffffff;v[9]=LOAD(index+5);
        SET(24,minmax4(v[24],lane4(v[0],0),1),1);SET(10,v[15],14);
        v[30]=native_ftoi(v[23],1);v[28]=v[5];
        acc=NMADD(acc,v[1],lane4(v[20],0),15);SET(23,v[15],1);
        acc=NMADD(acc,v[2],lane4(v[20],1),15);vi[1]=(clip&0x02fbef)!=0;
        v[25]=NMADD(acc,v[3],lane4(v[20],2),15);
        SET(24,NADD(v[11],lane4(v[24],3),1),1);STORE(index+1,v[30],15);
        native_vertices++;
        if(vi[1]) {
            native_clipped_vertices++;NATIVE_SAVE();
            unsigned clipped=native_clip_run(u,0x1198/8);if(!clipped)return 0;pairs+=clipped;
            for(unsigned i=0;i<32;i++)v[i]=vf4(u,i);
            acc=_mm_loadu_ps(cpu.vacc);q=cpu.q;clip=u->clip;entry_status=u->status;q_flags=u->status&0x30;
            memset(&flags,0,sizeof flags);
        }
        SET(23,NMUL(v[23],lane4(v[23],3),1),1);index=(index+3)&65535;vi[8]=(uint16_t)index;
        v[7]=NMUL(v[7],v[8],15);v[29]=v[28];
        v[9]=native_itof(v[9],1.0f/4096);
        float denominator=_mm_cvtss_f32(arithmetic4(lane4(v[25],3)));
        q_flags=denominator==0?0x20:0;
        q=denominator==0?vu_max(signbit(denominator)):_mm_cvtss_f32(arithmetic4(_mm_div_ss(_mm_set_ss(1),_mm_set_ss(denominator))));
        v[24]=native_ftoi(v[24],16);SET(30,v[25],1);
        acc=NMUL(v[31],lane4(v[23],3),15);SET(25,v[18],1);
        acc=native_merge(acc,NMADD(acc,v[26],lane4(v[0],3),14),14);
        v[23]=NMADD(acc,v[7],v[10],15);
        SET(9,NADD(v[9],v[19],12),12);STORE(index-1,v[24],15);
        pairs+=25;
    }while(index!=end);
    NATIVE_SAVE();
    return pairs;
#undef NATIVE_SAVE
#undef cpu
#undef NADD
#undef NSUB
#undef NMUL
#undef NMADD
#undef SET
#undef LOAD
#undef STORE
}

static unsigned native_geometry_260(Vu *u)
{
    unsigned begin = u->c->vi[8], end = u->c->vi[10];
    unsigned distance = (end - begin) & 0xffff;
    if (!distance || distance % 3 || distance / 3 > 64) { return 0; }
    ps2_q128 *memory = u->c->vu0_mem;
#define cpu (*u->c)
    drain_flags(u); update_q(u);
    unsigned entry_status = u->status;
    __m128 v[32], acc = _mm_loadu_ps(cpu.vacc);
    for (unsigned i = 0; i < 32; i++) v[i] = vf4(u, i);
    NativeFlags flags = {_mm_setzero_si128(), _mm_setzero_si128(), _mm_setzero_si128(), _mm_setzero_ps(), 0};
    unsigned clip = u->clip, pairs = 0, index = begin;
    uint16_t *vi = cpu.vi;
    float q = cpu.q; unsigned q_flags = u->status & 0x30;
#define NATIVE_SAVE() do { \
    for (unsigned i = 1; i < 32; i++) _mm_storeu_ps(cpu.vf[i].f,v[i]); \
    _mm_storeu_ps(cpu.vacc,acc); cpu.q = q; \
    Upper last = {0}; fmac_flags4(&last,flags.last,flags.mask); \
    unsigned mac = last.mac; \
    unsigned status = (mac & 15 ? 1 : 0) | (mac & 240 ? 2 : 0) | (mac & 3840 ? 4 : 0) | (mac & 61440 ? 8 : 0); \
    unsigned sticky = (_mm_movemask_ps(_mm_castsi128_ps(flags.zero)) ? 1 : 0) | \
                      (_mm_movemask_ps(_mm_castsi128_ps(flags.sign)) ? 2 : 0) | \
                      (_mm_movemask_ps(_mm_castsi128_ps(flags.overflow)) ? 8 : 0); \
    u->status = (entry_status & ~0x3fu) | status | q_flags | (sticky << 6); \
    u->mac = mac; u->clip = u->clip_next = clip; u->n_flags = 0; u->flag_head = 0; \
    u->q_ready = 0; u->cycle += 64; u->flag_cycle = u->cycle; \
} while(0)
#define NADD(a,b,d) native_add(&flags,(a),(b),(d))
#define NSUB(a,b,d) native_sub(&flags,(a),(b),(d))
#define NMUL(a,b,d) native_mul(&flags,(a),(b),(d))
#define NMADD(a,b,c,d) native_madd(&flags,(a),(b),(c),(d))
#define SET(r,x,d) v[r] = native_merge(v[r], (x), (d))
#define LOAD(a) _mm_loadu_ps(memory[(a) & 1023].f)
#define STORE(a,x,d) store_fields(memory[(a) & 1023].f, _mm_castps_si128(x), (d))
    do {
        v[9]=native_itof(v[9],1.0f/4096);vi[9]=(uint16_t)memory[(index+6)&1023].w[3];
        acc=NMUL(v[12],v[28],15);vi[6]=(uint16_t)memory[(index+3)&1023].w[3];
        acc=native_merge(acc,NMADD(acc,v[13],lane4(v[0],3),14),14);cpu.i=255;
        v[29]=NMADD(acc,v[14],lane4(v[23],3),15);index=(index+3)&65535;vi[8]=(uint16_t)index;
        SET(30,NADD(v[9],lane4(v[24],3),1),1);v[12]=LOAD(index+1);
        SET(30,NMUL(v[25],_mm_set1_ps(q),14),14);v[15]=LOAD(373+vi[6]);
        SET(21,NMUL(v[9],_mm_set1_ps(q),14),14);v[20]=LOAD(181+vi[9]);
        v[29]=minmax4(v[29],_mm_set1_ps(cpu.i),0);v[9]=LOAD(index+2);
        acc=NMUL(v[4],lane4(v[0],3),15);
        float denominator=_mm_cvtss_f32(arithmetic4(lane4(v[26],3)));
        q_flags=denominator==0?0x20:0;
        q=denominator==0?vu_max(signbit(denominator)):_mm_cvtss_f32(arithmetic4(_mm_div_ss(_mm_set_ss(1),_mm_set_ss(denominator))));
        v[30]=native_ftoi(v[30],16);SET(25,v[26],14);
        acc=NMADD(acc,v[1],lane4(v[20],0),15);SET(28,v[15],14);
        v[29]=native_ftoi(v[29],1);SET(23,v[15],1);
        acc=NMADD(acc,v[2],lane4(v[20],1),15);STORE(index-3,v[21],15);
        v[12]=native_itof(v[12],1);STORE(index-1,v[30],15);
        v[26]=NMADD(acc,v[3],lane4(v[20],2),15);
        SET(23,NMUL(v[23],lane4(v[23],3),1),1);STORE(index-2,v[29],15);
        native_vertices++;pairs+=16;
    }while(index!=end);
    NATIVE_SAVE();
    return pairs;
#undef NATIVE_SAVE
#undef cpu
#undef NADD
#undef NSUB
#undef NMUL
#undef NMADD
#undef SET
#undef LOAD
#undef STORE
}

/* The unclipped mesh loop B 08B0..0930 (dispatch 0x100: MSCAL 07F0, then MSCNT 1040 or 1CB0).
   Three input and three output qwords a vertex, no CLIP and no flag reads. Its DIV at 08D8 is read
   eight pairs later (0918/0920), after the divider's latency, so each vertex sees its own Q.
   The two accumulator products 0908 (vf0 * vf22) and 0930 (vf4 * vf0w) read registers the loop
   never writes: each is computed once, its sticky flags are the same every time. The vertex count
   is checked before anything is written; any other count leaves the pair-by-pair path to run. */
static unsigned native_geometry_800(Vu *u)
{
    unsigned begin = u->c->vi[8], end = u->c->vi[10];
    unsigned distance = (end - begin) & 0xffff;
    if (!distance || distance % 3 || distance / 3 > 341 || (u->q_ready && u->q_ready > u->cycle)) return 0;
    ps2_q128 *memory = u->c->vu0_mem;
#define cpu (*u->c)
    drain_flags(u); update_q(u);
    unsigned entry_status = u->status;
    __m128 v[32], acc = _mm_loadu_ps(cpu.vacc);
    for (unsigned i = 0; i < 32; i++) v[i] = vf4(u, i);
    NativeFlags flags = {_mm_setzero_si128(), _mm_setzero_si128(), _mm_setzero_si128(), _mm_setzero_ps(), 0};
    unsigned clip = u->clip, pairs = 0, index = begin;
    uint16_t *vi = cpu.vi;
    float q = cpu.q; unsigned q_flags = 0;
#define NADD(a,b,d) native_add(&flags,(a),(b),(d))
#define NMUL(a,b,d) native_mul(&flags,(a),(b),(d))
#define NMADD(a,b,c,d) native_madd(&flags,(a),(b),(c),(d))
#define SET(r,x,d) v[r] = native_merge(v[r], (x), (d))
#define LOAD(a) _mm_loadu_ps(memory[(a) & 1023].f)
#define STORE(a,x) _mm_storeu_ps(memory[(a) & 1023].f, (x))
    /* Loop invariants: I (MINIi), vf18, vf19, vf1-vf4, vf22, vf31. */
    const __m128 limit = _mm_set1_ps(cpu.i), zero_x = lane4(v[0], 0);
    const __m128 acc_0908 = NMUL(v[0], v[22], 15);
    const __m128 acc_0930 = NMUL(v[4], lane4(v[0], 3), 15);
    do {
        SET(23, minmax4(v[23], lane4(v[18], 1), 0), 1); v[11] = v[7];            /* 08B0 */
        v[7] = native_itof(v[9], 1.0f / 4096.0f);                                  /* 08B8 */
        v[10] = minmax4(v[10], limit, 0);                                          /* 08C0 */
        v[12] = native_itof(v[28], 1.0f); SET(24, v[23], 14);                      /* 08C8 */
        SET(24, minmax4(v[23], zero_x, 1), 1);                                     /* 08D0 */
        vi[9] = (uint16_t)memory[(index + 9) & 1023].w[3];
        SET(7, NADD(v[7], v[19], 12), 12);                                         /* 08D8 */
        float denominator = _mm_cvtss_f32(arithmetic4(lane4(v[26], 3)));
        q_flags = denominator == 0 ? 0x20 : 0;
        q = denominator == 0 ? vu_max(signbit(denominator)) :
            _mm_cvtss_f32(arithmetic4(_mm_div_ss(_mm_set_ss(1.0f), _mm_set_ss(denominator))));
        v[10] = native_ftoi(v[10], 1.0f); index = (index + 3) & 0xffff;            /* 08E0 */
        acc = NMADD(acc, v[1], lane4(v[20], 0), 15); SET(25, v[18], 1);            /* 08E8 */
        SET(24, NADD(v[11], lane4(v[24], 3), 1), 1); SET(25, v[26], 14);           /* 08F0 */
        acc = NMADD(acc, v[2], lane4(v[20], 1), 15);                               /* 08F8 */
        STORE(index - 3, v[8]); v[26] = NMADD(acc, v[3], lane4(v[20], 2), 15);     /* 0900 */
        STORE(index - 2, v[10]); acc = acc_0908;                                   /* 0908 */
        v[24] = native_ftoi(v[24], 16.0f); v[20] = LOAD(181 + vi[9]);              /* 0910 */
        v[23] = NMADD(acc, v[25], _mm_set1_ps(q), 15); v[28] = LOAD(index + 4);    /* 0918 */
        SET(8, NMUL(v[7], _mm_set1_ps(q), 14), 14); v[9] = LOAD(index + 5);        /* 0920 */
        v[10] = NMUL(v[12], v[31], 15);                                            /* 0928 */
        acc = acc_0930; STORE(index - 1, v[24]);                                   /* 0930 */
        native_vertices++;
        pairs += 17;
    } while (index != end);
    vi[8] = (uint16_t)index;
    /* The last FMAC is 0930's. */
    flags.last = _mm_mul_ps(operand4(v[4]), operand4(lane4(v[0], 3))); flags.mask = 15;
    for (unsigned i = 1; i < 32; i++) _mm_storeu_ps(cpu.vf[i].f, v[i]);
    _mm_storeu_ps(cpu.vacc, acc); cpu.q = q;
    Upper last = {0}; fmac_flags4(&last, flags.last, flags.mask);
    unsigned mac = last.mac;
    unsigned status = (mac & 15 ? 1 : 0) | (mac & 240 ? 2 : 0) | (mac & 3840 ? 4 : 0) | (mac & 61440 ? 8 : 0);
    unsigned sticky = (_mm_movemask_ps(_mm_castsi128_ps(flags.zero)) ? 1 : 0) |
                      (_mm_movemask_ps(_mm_castsi128_ps(flags.sign)) ? 2 : 0) |
                      (_mm_movemask_ps(_mm_castsi128_ps(flags.overflow)) ? 8 : 0);
    u->status = (entry_status & ~0x3fu) | status | q_flags | (sticky << 6);
    u->mac = mac; u->clip = u->clip_next = clip; u->n_flags = 0; u->flag_head = 0;
    u->q_ready = 0; u->cycle += 64; u->flag_cycle = u->cycle;
    return pairs;
#undef cpu
#undef NADD
#undef NMUL
#undef NMADD
#undef SET
#undef LOAD
#undef STORE
}

/* The skinning loop 0468-0570 (MSCAL 0010/0020 through 03C8): a vertex blended from three bone
   matrices by weights, its normal transformed and lit, 34 pairs a vertex. The loop reads no
   flags, Q, P or clip, and the VU stalls a read until its producer lands, so the pairs run in
   program order (each upper before its lower: 0510's MADDAz reads vf5 before the MOVE replaces
   it, and no lower reads its own pair's upper result). The last FMAC is 0570's MADDw. Declines
   before writing anything when a division or EFU result is still pending or the count could
   pass the run's step limit. */
static unsigned native_skin_468(Vu *u)
{
    int16_t count = (int16_t)u->c->vi[10];
    if ((u->q_ready && u->q_ready > u->cycle) || (u->p_ready && u->p_ready > u->cycle) || count > 8192) return 0;
    ps2_q128 *memory = u->c->vu0_mem;
#define cpu (*u->c)
    drain_flags(u); update_q(u);
    unsigned entry_status = u->status;
    __m128 v[32], acc = _mm_loadu_ps(cpu.vacc);
    for (unsigned i = 0; i < 32; i++) v[i] = vf4(u, i);
    NativeFlags flags = {_mm_setzero_si128(), _mm_setzero_si128(), _mm_setzero_si128(), _mm_setzero_ps(), 0};
    uint16_t *vi = cpu.vi;
    unsigned pairs = 0;
    const __m128 i_reg = _mm_set1_ps(cpu.i), one_w = lane4(v[0], 3), zero_x = lane4(v[0], 0);
#define NADD(a,b,d) native_add(&flags,(a),(b),(d))
#define NSUB(a,b,d) native_sub(&flags,(a),(b),(d))
#define NMUL(a,b,d) native_mul(&flags,(a),(b),(d))
#define NMADD(a,b,c,d) native_madd(&flags,(a),(b),(c),(d))
#define SET(r,x,d) v[r] = native_merge(v[r], (x), (d))
#define LOAD(a) _mm_loadu_ps(memory[(a) & 1023].f)
#define STORE(a,x,d) store_fields(memory[(a) & 1023].f, _mm_castps_si128(x), (d))
#define LANE_BITS(r,c) ((uint16_t)_mm_cvtsi128_si32(_mm_castps_si128(lane4(v[r], c))))
    do {
        SET(26, native_itof(v[26], 1.0f), 1); v[24] = LOAD(vi[1] + 859);              /* 0468 */
        acc = native_merge(acc, NMUL(v[21], lane4(v[17], 0), 14), 14); SET(9, v[5], 1); /* 0470 */
        acc = native_merge(acc, NMADD(acc, v[22], lane4(v[17], 1), 14), 14);           /* 0478 */
        vi[6] = vi[5] & vi[6];
        SET(25, NMADD(acc, v[23], lane4(v[17], 2), 14), 14); vi[3] = vi[5] & vi[3];   /* 0480 */
        acc = NMUL(v[21], lane4(v[5], 0), 15); v[17] = LOAD(vi[8] + 192);             /* 0488 */
        acc = NMADD(acc, v[22], lane4(v[5], 1), 15); v[21] = LOAD(vi[6] + 856);       /* 0490 */
        acc = NMADD(acc, v[23], lane4(v[5], 2), 15); v[22] = LOAD(vi[6] + 857);       /* 0498 */
        v[6] = NMADD(acc, v[24], one_w, 15); v[23] = LOAD(vi[6] + 858);               /* 04A0 */
        acc = NMUL(v[14], lane4(v[25], 0), 15);                                       /* 04A8 */
        acc = NMADD(acc, v[15], lane4(v[25], 1), 15);                                 /* 04B0 */
        v[25] = NMADD(acc, v[16], lane4(v[25], 2), 15);                               /* 04B8 */
        SET(29, NSUB(v[10], v[26], 1), 1); v[24] = LOAD(vi[6] + 859);                /* 04C0 */
        acc = NMUL(v[21], lane4(v[5], 0), 15); v[12] = LOAD(vi[8]); vi[8]++;          /* 04C8 */
        acc = NMADD(acc, v[22], lane4(v[5], 1), 15); v[21] = LOAD(vi[3] + 856);       /* 04D0 */
        acc = NMADD(acc, v[23], lane4(v[5], 2), 15); v[22] = LOAD(vi[3] + 857);       /* 04D8 */
        v[7] = NMADD(acc, v[24], one_w, 15); v[23] = LOAD(vi[3] + 858);               /* 04E0 */
        SET(30, NADD(v[9], v[29], 1), 1); v[24] = LOAD(vi[3] + 859);                 /* 04E8 */
        v[25] = minmax4(v[25], zero_x, 1); STORE(vi[8] - 3, v[20], 14);               /* 04F0 */
        acc = NMUL(v[21], lane4(v[5], 0), 15); vi[1] = LANE_BITS(12, 0);              /* 04F8 */
        acc = NMADD(acc, v[22], lane4(v[5], 1), 15); vi[6] = LANE_BITS(12, 1);        /* 0500 */
        SET(10, NMUL(v[12], i_reg, 1), 1); vi[3] = LANE_BITS(12, 2);                  /* 0508 */
        acc = NMADD(acc, v[23], lane4(v[5], 2), 15); v[5] = v[12];                    /* 0510 */
        v[17] = native_itof(v[17], 1.0f / 4096.0f); vi[1] = vi[5] & vi[1];           /* 0518 */
        SET(30, NSUB(v[0], v[30], 1), 1); vi[10]--;                                   /* 0520 */
        v[8] = NMADD(acc, v[24], one_w, 15);                                          /* 0528 */
        SET(25, NMUL(v[25], lane4(v[25], 3), 1), 1);                                  /* 0530 */
        acc = native_merge(acc, NMUL(v[11], lane4(v[25], 0), 14), 14);                /* 0538 */
        acc = native_merge(acc, NMADD(acc, v[27], lane4(v[25], 1), 14), 14); v[21] = LOAD(vi[1] + 856); /* 0540 */
        SET(25, NMADD(acc, v[28], lane4(v[25], 2), 14), 14); v[22] = LOAD(vi[1] + 857); /* 0548 */
        SET(25, NMUL(v[25], lane4(v[25], 3), 1), 1); v[23] = LOAD(vi[1] + 858);       /* 0550 */
        SET(26, native_ftoi(v[10], 1.0f), 1);                                         /* 0558 */
        acc = NMUL(v[6], lane4(v[9], 3), 15);                                         /* 0560 */
        acc = NMADD(acc, v[7], lane4(v[29], 3), 15);                                  /* 0568 IBGTZ vi10 */
        v[20] = NMADD(acc, v[8], lane4(v[30], 3), 15); STORE(vi[8] + 190, v[25], 15); /* 0570 (delay slot) */
        pairs += 34;
    } while ((int16_t)vi[10] > 0);
    for (unsigned i = 1; i < 32; i++) _mm_storeu_ps(cpu.vf[i].f, v[i]);
    _mm_storeu_ps(cpu.vacc, acc);
    Upper last = {0}; fmac_flags4(&last, flags.last, flags.mask);
    unsigned mac = last.mac;
    unsigned status = (mac & 15 ? 1 : 0) | (mac & 240 ? 2 : 0) | (mac & 3840 ? 4 : 0) | (mac & 61440 ? 8 : 0);
    unsigned sticky = (_mm_movemask_ps(_mm_castsi128_ps(flags.zero)) ? 1 : 0) |
                      (_mm_movemask_ps(_mm_castsi128_ps(flags.sign)) ? 2 : 0) |
                      (_mm_movemask_ps(_mm_castsi128_ps(flags.overflow)) ? 8 : 0);
    u->status = (entry_status & ~0xfu) | status | (sticky << 6);
    u->mac = mac; u->n_flags = 0; u->flag_head = 0;
    u->cycle += 64; u->flag_cycle = u->cycle;
    return pairs;
#undef cpu
#undef NADD
#undef NSUB
#undef NMUL
#undef NMADD
#undef SET
#undef LOAD
#undef STORE
#undef LANE_BITS
}

#include "vu1_native_0e10.h"

#include "vu1_native_2e48.h"
