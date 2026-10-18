/* Program 2DC8's 2F00..2FC0 loop (VI13=0x5BD).
   FSAND at 2F30 sees the MAC from 2F10: no intervening read can stall after
   that pair. Its Z/S bits and the current DIV D bit control the fallback.
   The final DIV remains pending for two more issue slots; the guarded tail
   has no early MAC/status read. Register names preserve microcode ordering. */
static unsigned native_geometry_2dc8(Vu *u) {
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
    float q = cpu.q, next_q = q;
    unsigned q_flags = u->status & 0x30, next_q_flags = q_flags;
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
        v[12] = NMADD(acc, v[10], lane4(v[20], 2), 15);
        v[25] = v[26];
        acc = native_merge(acc, NADD(v[9], lane4(v[9], 1), 2), 2);
        SET(17, LOAD(index + 5), 1);
        __m128 branch_value = NMADD(acc, v[19], lane4(v[9], 0), 2);
        SET(9, branch_value, 2);
        v[20] = LOAD(181 + vi[9]);
        Upper branch_flags = {0};
        fmac_flags4(&branch_flags, branch_value, 2);
        unsigned branch_status = (branch_flags.mac & 15 ? 1 : 0) | (branch_flags.mac & 240 ? 2 : 0);
        SET(24, NADD(v[23], lane4(v[18], 1), 1), 1);
        STORE(index, v[31], 14);
        acc = NMUL(v[4], lane4(v[0], 3), 15);
        vi[6] = (uint16_t)memory[(index + 3) & 1023].w[3];
        clip = ((clip << 6) | native_clip(v[5], lane4(v[13], 3))) & 0xffffff;
        vi[9] = (uint16_t)memory[(index + 9) & 1023].w[3];
        SET(23, NMUL(v[25], _mm_set1_ps(q), 14), 14);
        vi[1] = branch_status | (q_flags & 0x20);
        unsigned issued = vi[1] ? ((q_flags & 0x20) ? 25 : 24) : 21;
        SET(31, NMUL(v[12], _mm_set1_ps(q), 14), 14);
        SET(23, native_itof(v[17], 1.0f / 4096), 1);
        v[9] = LOAD(373 + vi[6]);
        if (vi[1]) {
            SET(19, NADD(v[0], lane4(v[0], 0), 1), 1);
            vi[1] = q_flags & 0x20;
            if (vi[1])
                SET(31, NADD(v[12], lane4(v[0], 0), 14), 14);
        }
        acc = NMADD(acc, v[1], lane4(v[20], 0), 15);
        SET(11, lane4(v[24], 3), 1);
        acc = NMADD(acc, v[2], lane4(v[20], 1), 15);
        v[28] = v[5];
        v[26] = NMADD(acc, v[3], lane4(v[20], 2), 15);
        vi[1] = (clip & 0x02fbef) != 0;
        v[9] = native_itof(v[9], 1.0f / 4096);
        /* Both issue in 2F88: SQ must read the old VF19.w before ADDx commits. */
        STORE(index + 1, v[19], 15);
        SET(19, NADD(v[6], lane4(v[0], 0), 1), 1);
        native_vertices++;
        if (vi[1]) {
            native_clipped_vertices++;
            NATIVE_SAVE();
            unsigned clipped = native_clip_run(u, 0x2f90 / 8);
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
        v[30] = native_ftoi(v[24], 16);
        v[6] = LOAD(1013);
        acc = NMUL(v[14], lane4(v[0], 3), 15);
        SET(24, v[23], 14);
        acc = NMADD(acc, v[7], lane4(v[20], 0), 15);
        /* DIV waits for a clipping DIV before replacing it. No intervening Q read. */
        if (u->q_ready) {
            q = u->q_next;
            q_flags = u->q_flags;
            u->q_ready = 0;
        }
        float denominator = _mm_cvtss_f32(arithmetic4(lane4(v[26], 3)));
        next_q_flags = denominator == 0 ? 0x20 : 0;
        next_q =
            denominator == 0
                ? vu_max(signbit(denominator))
                : _mm_cvtss_f32(arithmetic4(_mm_div_ss(_mm_set_ss(1), _mm_set_ss(denominator))));
        acc = NMADD(acc, v[8], lane4(v[20], 1), 15);
        index = (index + 3) & 65535;
        vi[8] = (uint16_t)index;
        SET(9, NMUL(v[9], v[6], 14), 14);
        v[29] = v[28];
        SET(5, NADD(v[13], lane4(v[25], 3), 2), 2);
        SET(5, NADD(v[24], v[13], 12), 12);
        STORE(index - 1, v[30], 15);
        pairs += issued;
        if (index != end) {
            q = next_q;
            q_flags = next_q_flags;
        }
    } while (index != end);
    NATIVE_SAVE();
    /* DIV at 2FA0 has issued five slots at exit; it completes in seven. */
    u->q_next = next_q;
    u->q_flags = next_q_flags;
    u->q_ready = u->cycle + 2;
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
