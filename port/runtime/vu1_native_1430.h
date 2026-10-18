/* Program 1430's mesh/light loop, 1510..1620 (dispatch VI13=0x288).
   Registers remain named as in the microprogram to preserve operation order.
   No MAC or sticky-status reads occur in this loop: FSAND reads only the new
   DIV's D bit. FCAND reads the CLIP issued four pairs earlier. The closed
   clipping routine returns through VI5 to 15E0; it retains exact pipeline
   helpers. Unsupported vertex counts or a pending Q decline before writing. */
static unsigned native_geometry_1430(Vu *u) {
    unsigned begin = u->c->vi[8], end = u->c->vi[10], distance = (end - begin) & 65535;
    if (!distance || distance % 3 || distance / 3 > 64 || (u->q_ready && u->q_ready > u->cycle))
        return 0;
    ps2_q128 *memory = u->c->vu0_mem;
#define cpu (*u->c)
    drain_flags(u);
    update_q(u);
    unsigned entry_status = u->status;
    __m128 v[32], acc = _mm_loadu_ps(cpu.vacc);
    for (unsigned i = 0; i < 32; i++)
        v[i] = vf4(u, i);
    NativeFlags flags = {_mm_setzero_si128(), _mm_setzero_si128(), _mm_setzero_si128(),
                         _mm_setzero_ps(), 0};
    unsigned clip = u->clip, pairs = 0, index = begin;
    uint16_t *vi = cpu.vi;
    float q = cpu.q;
    unsigned q_flags = u->status & 0x30;
#define NATIVE_SAVE()                                                                              \
    do {                                                                                           \
        for (unsigned i = 1; i < 32; i++)                                                          \
            _mm_storeu_ps(cpu.vf[i].f, v[i]);                                                      \
        _mm_storeu_ps(cpu.vacc, acc);                                                              \
        cpu.q = q;                                                                                 \
        Upper last = {0};                                                                          \
        fmac_flags4(&last, flags.last, flags.mask);                                                \
        unsigned mac = last.mac;                                                                   \
        unsigned status = (mac & 15 ? 1 : 0) | (mac & 240 ? 2 : 0) | (mac & 3840 ? 4 : 0) |        \
                          (mac & 61440 ? 8 : 0);                                                   \
        unsigned sticky = (_mm_movemask_ps(_mm_castsi128_ps(flags.zero)) ? 1 : 0) |                \
                          (_mm_movemask_ps(_mm_castsi128_ps(flags.sign)) ? 2 : 0) |                \
                          (_mm_movemask_ps(_mm_castsi128_ps(flags.overflow)) ? 8 : 0);             \
        u->status = (entry_status & ~0x3fu) | status | q_flags | (sticky << 6);                    \
        u->mac = mac;                                                                              \
        u->clip = u->clip_next = clip;                                                             \
        u->n_flags = 0;                                                                            \
        u->flag_head = 0;                                                                          \
        u->q_ready = 0;                                                                            \
        u->cycle += 64;                                                                            \
        u->flag_cycle = u->cycle;                                                                  \
    } while (0)
#define NADD(a, b, d) native_add(&flags, (a), (b), (d))
#define NMUL(a, b, d) native_mul(&flags, (a), (b), (d))
#define NMADD(a, b, c, d) native_madd(&flags, (a), (b), (c), (d))
#define SET(r, x, d) v[r] = native_merge(v[r], (x), (d))
#define LOAD(a) _mm_loadu_ps(memory[(a) & 1023].f)
#define STORE(a, x, d) store_fields(memory[(a) & 1023].f, _mm_castps_si128(x), (d))
    do {
        float denominator = _mm_cvtss_f32(arithmetic4(lane4(v[25], 3)));
        q_flags = denominator == 0 ? 0x20 : 0;
        unsigned issued = q_flags ? 35 : 34;
        q = denominator == 0
                ? vu_max(signbit(denominator))
                : _mm_cvtss_f32(arithmetic4(_mm_div_ss(_mm_set_ss(1), _mm_set_ss(denominator))));
        SET(30, lane4(v[25], 3), 1);
        SET(23, NMUL(v[23], lane4(v[23], 3), 1), 1);
        SET(16, lane4(v[0], 3), 1);
        SET(9, NADD(v[9], v[19], 12), 12);
        SET(25, lane4(v[18], 3), 1);
        acc = NMUL(v[0], v[22], 15);
        v[6] = LOAD(373 + vi[9]);
        v[10] = LOAD(20);
        SET(23, NMUL(v[23], lane4(v[23], 3), 1), 1);
        v[24] = NMADD(acc, v[25], _mm_set1_ps(q), 15);
        vi[1] = q_flags;
        SET(20, NMUL(v[9], _mm_set1_ps(q), 14), 14);
        v[6] = native_itof(v[6], 1.0f / 4096);
        if (q_flags)
            SET(20, NADD(v[9], lane4(v[0], 0), 14), 14);
        acc = NMUL(v[10], lane4(v[23], 3), 15);
        v[23] = NMADD(acc, v[12], v[31], 15);
        cpu.i = 255.0f;
        SET(5, NADD(v[24], v[13], 12), 12);
        v[8] = LOAD(7);
        SET(5, NADD(v[13], lane4(v[30], 3), 2), 2);
        STORE(index, v[20], 14);
        SET(24, minmax4(v[24], lane4(v[18], 1), 0), 1);
        v[10] = LOAD(8);
        v[23] = minmax4(v[23], _mm_set1_ps(cpu.i), 0);
        v[11] = v[9];
        acc = NMUL(v[14], lane4(v[6], 0), 15);
        v[9] = LOAD(index + 5);
        clip = ((clip << 6) | native_clip(v[5], lane4(v[13], 3))) & 0xffffff;
        v[20] = LOAD(181 + vi[9]);
        SET(24, minmax4(v[24], lane4(v[0], 0), 1), 1);
        v[30] = native_ftoi(v[23], 1);
        acc = NMADD(acc, v[8], lane4(v[6], 1), 15);
        v[28] = v[5];
        v[23] = NMADD(acc, v[10], lane4(v[6], 2), 15);
        vi[1] = (clip & 0x02fbef) != 0;
        SET(24, NADD(v[11], lane4(v[24], 3), 1), 1);
        v[9] = native_itof(v[9], 1.0f / 4096);
        STORE(index + 1, v[30], 15);
        native_vertices++;
        if (vi[1]) {
            native_clipped_vertices++;
            NATIVE_SAVE();
            unsigned clipped = native_clip_run(u, 0x15e0 / 8);
            if (!clipped)
                return ~0u;
            pairs += clipped;
            for (unsigned i = 0; i < 32; i++)
                v[i] = vf4(u, i);
            acc = _mm_loadu_ps(cpu.vacc);
            q = cpu.q;
            clip = u->clip;
            entry_status = u->status;
            q_flags = u->status & 0x30;
            memset(&flags, 0, sizeof flags);
        }
        v[23] = minmax4(v[23], lane4(v[0], 0), 1);
        v[12] = LOAD(index + 4);
        acc = NMUL(v[4], lane4(v[0], 3), 15);
        v[8] = LOAD(18);
        acc = NMADD(acc, v[1], lane4(v[20], 0), 15);
        v[25] = v[26];
        v[24] = native_ftoi(v[24], 16);
        index = (index + 3) & 65535;
        vi[8] = (uint16_t)index;
        SET(23, NMUL(v[23], lane4(v[23], 3), 1), 1);
        v[29] = v[28];
        acc = NMADD(acc, v[2], lane4(v[20], 1), 15);
        v[10] = LOAD(19);
        v[25] = NMADD(acc, v[3], lane4(v[20], 2), 15);
        vi[9] = (uint16_t)memory[(index + 3) & 1023].w[3];
        v[12] = native_itof(v[12], 1);
        SET(23, NMUL(v[23], lane4(v[23], 3), 1), 1);
        STORE(index - 1, v[24], 15);
        pairs += issued;
    } while (index != end);
    NATIVE_SAVE();
    return pairs;
#undef NATIVE_SAVE
#undef cpu
#undef NADD
#undef NMUL
#undef NMADD
#undef SET
#undef LOAD
#undef STORE
}
