/* Exact fixed-template implementations. Parsing, operand binding, diagnostics,
   and output writeback remain shared with the generic interpreter. Recognition
   validates the full supported operation shape, never a hash or caller address.
   DHD_ASM_GENERIC=1 retains the original interpreter for diagnosis and A/B. */
#include <emmintrin.h>
enum { FAST_NONE, FAST_MATRIX_DIRECTION, FAST_BOUNDS, FAST_VECTOR_ADD, FAST_VECTOR_SUB, FAST_QUAD_COPY };
static int fast_enabled = -1;

#define FV(r) {K_VF, r, NO_COMP, 0, 0}
#define FB(r,c) {K_VF, r, c, 0, 0}
#define FA {K_ACC, 0, NO_COMP, 0, 0}
#define FM(r,o) {K_MEM, r, NO_COMP, 0, o}
static const Ins matrix_direction_instructions[] = {
    {LQC2,0,NO_COMP,2,0,{FV(11),FM(33,0)}},
    {LQC2,0,NO_COMP,2,0,{FV(12),FM(32,0)}},
    {LQC2,0,NO_COMP,2,0,{FV(13),FM(32,16)}},
    {LQC2,0,NO_COMP,2,0,{FV(14),FM(32,32)}},
    {V_ADD,7,3,3,0,{FV(15),FV(0),FB(0,3)}},
    {V_MUL,7,NO_COMP,3,0,{FV(12),FV(12),FV(11)}},
    {V_MUL,7,NO_COMP,3,0,{FV(13),FV(13),FV(11)}},
    {V_MUL,7,NO_COMP,3,0,{FV(14),FV(14),FV(11)}},
    {V_ADDA,1,1,3,0,{FA,FV(12),FB(12,1)}},
    {V_MADD,1,2,3,0,{FV(11),FV(15),FB(12,2)}},
    {V_ADDA,2,2,3,0,{FA,FV(13),FB(13,2)}},
    {V_MADD,2,0,3,0,{FV(11),FV(15),FB(13,0)}},
    {V_ADDA,4,0,3,0,{FA,FV(14),FB(14,0)}},
    {V_MADD,4,1,3,0,{FV(11),FV(15),FB(14,1)}},
    {SQC2,0,NO_COMP,2,0,{FV(11),FM(34,0)}}
};
static const Ins bounds_instructions[] = {
    {LQC2,0,NO_COMP,2,0,{FV(11),FM(32,0)}},
    {V_MINI,15,NO_COMP,3,0,{FV(30),FV(30),FV(11)}},
    {V_MAX,15,NO_COMP,3,0,{FV(31),FV(31),FV(11)}}
};
static const Ins vector_sum = {V_ADD,7,NO_COMP,3,0,{FV(11),FV(11),FV(12)}};
static const Ins vector_difference = {V_SUB,7,NO_COMP,3,0,{FV(11),FV(11),FV(12)}};
#undef FV
#undef FB
#undef FA
#undef FM

static int fast_memory_instruction(const Prog *p, const Ins *in, unsigned op, unsigned kind, unsigned reg)
{
    /* The effective address remains dynamic and uses the original address()
       helper. Every other instruction field must match the supported shape. */
    Ins expected = {0};
    expected.op = (uint8_t)op; expected.bc = NO_COMP; expected.n = 2;
    expected.o[0] = (Opd){(uint8_t)kind, (uint8_t)reg, NO_COMP, 0, 0};
    const Opd *memory = &in->o[1];
    if (memory->comp != NO_COMP || memory->step) return 0;
    if (memory->kind == K_MEM) { if (memory->reg >= 64) return 0; }
    else if (memory->kind == K_OPMEM) { if (memory->reg >= p->n_ops) return 0; }
    else return 0;
    expected.o[1] = *memory;
    return !memcmp(in, &expected, sizeof expected);
}

static int select_fast_program(const Prog *p)
{
    if (p->n_ops == 3 && p->n_ins == 15 &&
        !memcmp(p->ins, matrix_direction_instructions, sizeof matrix_direction_instructions))
        return FAST_MATRIX_DIRECTION;
    if (p->n_ops == 1 && p->n_ins == 3 &&
        !memcmp(p->ins, bounds_instructions, sizeof bounds_instructions)) return FAST_BOUNDS;
    if (p->n_ins == 4 && fast_memory_instruction(p,&p->ins[0],LQC2,K_VF,11) &&
        fast_memory_instruction(p,&p->ins[1],LQC2,K_VF,12) &&
        fast_memory_instruction(p,&p->ins[3],SQC2,K_VF,11)) {
        if (!memcmp(&p->ins[2],&vector_sum,sizeof vector_sum)) return FAST_VECTOR_ADD;
        if (!memcmp(&p->ins[2],&vector_difference,sizeof vector_difference)) return FAST_VECTOR_SUB;
    }
    if (p->n_ins == 2 && fast_memory_instruction(p,&p->ins[0],LQ,K_GPR,2) &&
        fast_memory_instruction(p,&p->ins[1],SQ,K_GPR,2)) return FAST_QUAD_COPY;
    return FAST_NONE;
}

static __m128i ordered_float_bits(__m128i bits)
{
    /* Signed integer order equivalent to ps2_float_minmax's unsigned keys.
       Includes signed zero, subnormals and all exponent-255 encodings. */
    return _mm_xor_si128(bits, _mm_and_si128(_mm_srai_epi32(bits,31),_mm_set1_epi32(0x7fffffff)));
}
static __m128i select_bits(__m128i mask, __m128i a, __m128i b)
{ return _mm_or_si128(_mm_and_si128(mask,a),_mm_andnot_si128(mask,b)); }
static void update_bounds(Prog *p, ps2_op *ops)
{
    memcpy(&ps2.vf[11],quad(p,address(&p->ins[0].o[1],ops)),16);
    memset(&ps2.gpr[0],0,sizeof ps2.gpr[0]);
    __m128i value=_mm_loadu_si128((const __m128i *)&ps2.vf[11]);
    __m128i low=_mm_loadu_si128((const __m128i *)&ps2.vf[30]);
    __m128i high=_mm_loadu_si128((const __m128i *)&ps2.vf[31]);
    __m128i key=ordered_float_bits(value);
    _mm_storeu_si128((__m128i *)&ps2.vf[30],select_bits(_mm_cmpgt_epi32(key,ordered_float_bits(low)),low,value));
    _mm_storeu_si128((__m128i *)&ps2.vf[31],select_bits(_mm_cmpgt_epi32(ordered_float_bits(high),key),high,value));
}
static void vector_sum_or_difference(Prog *p, ps2_op *ops)
{
    memcpy(&ps2.vf[11],quad(p,address(&p->ins[0].o[1],ops)),16);
    memset(&ps2.gpr[0],0,sizeof ps2.gpr[0]);
    memcpy(&ps2.vf[12],quad(p,address(&p->ins[1].o[1],ops)),16);
    memset(&ps2.gpr[0],0,sizeof ps2.gpr[0]);
    if(p->fast_kind==FAST_VECTOR_ADD)
        for(unsigned i=0;i<3;i++)ps2.vf[11].f[i]=ps2_float_add(ps2.vf[11].f[i],ps2.vf[12].f[i]);
    else
        for(unsigned i=0;i<3;i++)ps2.vf[11].f[i]=ps2_float_sub(ps2.vf[11].f[i],ps2.vf[12].f[i]);
    ps2_q128 result=ps2.vf[11];
    ps2_store128((uintptr_t)quad(p,address(&p->ins[3].o[1],ops)),&result);
    memset(&ps2.gpr[0],0,sizeof ps2.gpr[0]);
}
static void quad_copy(Prog *p, ps2_op *ops)
{
    memcpy(&ps2.gpr[2],quad(p,address(&p->ins[0].o[1],ops)),16);
    memset(&ps2.gpr[0],0,sizeof ps2.gpr[0]);
    ps2_store128((uintptr_t)quad(p,address(&p->ins[1].o[1],ops)),&ps2.gpr[2]);
    memset(&ps2.gpr[0],0,sizeof ps2.gpr[0]);
}

static void matrix_direction(Prog *p)
{
    /* Loads/stores keep quadword rounding, address mapping and MMIO callbacks.
       The first instruction executes before the generic loop clears $zero. */
    uintptr_t vector = (uintptr_t)ps2.gpr[33].d[0];
    memcpy(&ps2.vf[11], quad(p, (uint8_t *)ps2_memory_address(vector)), 16);
    memset(&ps2.gpr[0], 0, sizeof ps2.gpr[0]);
    for (unsigned row = 0; row < 3; row++) {
        uintptr_t address = (uintptr_t)(ps2.gpr[32].d[0] + row * 16);
        memcpy(&ps2.vf[12 + row], quad(p, (uint8_t *)ps2_memory_address(address)), 16);
        memset(&ps2.gpr[0], 0, sizeof ps2.gpr[0]);
    }
    for (unsigned lane = 0; lane < 3; lane++) ps2.vf[15].f[lane] = ps2_float_add(0.0f, 1.0f);
    for (unsigned row = 12; row <= 14; row++)
        for (unsigned lane = 0; lane < 3; lane++)
            ps2.vf[row].f[lane] = ps2_float_mul(ps2.vf[row].f[lane], ps2.vf[11].f[lane]);
    ps2.vacc[0] = ps2_float_add(ps2.vf[12].f[0], ps2.vf[12].f[1]);
    ps2.vf[11].f[0] = ps2_vu_madd(ps2.vacc[0], ps2.vf[15].f[0], ps2.vf[12].f[2], 0);
    ps2.vacc[1] = ps2_float_add(ps2.vf[13].f[1], ps2.vf[13].f[2]);
    ps2.vf[11].f[1] = ps2_vu_madd(ps2.vacc[1], ps2.vf[15].f[1], ps2.vf[13].f[0], 0);
    ps2.vacc[2] = ps2_float_add(ps2.vf[14].f[2], ps2.vf[14].f[0]);
    ps2.vf[11].f[2] = ps2_vu_madd(ps2.vacc[2], ps2.vf[15].f[2], ps2.vf[14].f[1], 0);
    ps2_q128 result = ps2.vf[11];
    uintptr_t output = (uintptr_t)ps2.gpr[34].d[0];
    ps2_store128((uintptr_t)quad(p, (uint8_t *)ps2_memory_address(output)), &result);
    memset(&ps2.gpr[0], 0, sizeof ps2.gpr[0]);
}

/* Keep fast-template temporaries out of ps2_asm's operand-binding frame.
   Legacy translated callers can copy partially initialized stack quadwords;
   changing that frame's footprint changes their otherwise unused stored lanes. */
static __declspec(noinline) int run_fast_program(Prog *p, ps2_op *ops)
{
    if (!p->fast_kind) return 0;
    if (fast_enabled < 0) {
        const char *generic = getenv("DHD_ASM_GENERIC");
        fast_enabled = !generic || !atoi(generic);
    }
    if (!fast_enabled) return 0;
    if (p->fast_kind == FAST_MATRIX_DIRECTION) {
        matrix_direction(p);
        return 1;
    }
    if (p->fast_kind == FAST_BOUNDS) {
        update_bounds(p, ops);
        return 1;
    }
    if (p->fast_kind == FAST_VECTOR_ADD || p->fast_kind == FAST_VECTOR_SUB) {
        vector_sum_or_difference(p, ops);
        return 1;
    }
    if (p->fast_kind == FAST_QUAD_COPY) {
        quad_copy(p, ops);
        return 1;
    }
    return 0;
}
