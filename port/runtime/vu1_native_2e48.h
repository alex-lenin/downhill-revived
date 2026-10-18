/* Program 2E48's projected particle quads, 2F90..30B8, up to 41 quads.
   The AOT setup, alternating packet buffers, outer group loop and kicks stay.
   Preserve raw MR32 moves, upper/lower issue order, both DIV results and the
   four-slot MAC/CLIP reads. Clipped quads mark both output corner W fields.
   Unsupported counts or pending Q decline before any direct writes. */
static unsigned native_particles_2e48(Vu *u) {
  unsigned remaining = u->c->vi[8];
  if (!remaining || remaining > 41 || (u->q_ready && u->q_ready > u->cycle))
    return 0;
  ps2_q128 *memory = u->c->vu0_mem;
#define cpu (*u->c)
  drain_flags(u);
  update_q(u);
  unsigned entry_status = u->status;
  __m128 v[32], acc = _mm_loadu_ps(cpu.vacc);
  for (unsigned i = 0; i < 32; i++)
    v[i] = vf4(u, i);
  NativeFlags flags = {_mm_setzero_si128(), _mm_setzero_si128(),
                       _mm_setzero_si128(), _mm_setzero_ps(), 0};
  unsigned clip = u->clip, pairs = 0;
  uint16_t *vi = cpu.vi;
  float q = cpu.q;
  unsigned q_flags = u->status & 0x30;
#define NADD(a, b, d) native_add(&flags, (a), (b), (d))
#define NSUB(a, b, d) native_sub(&flags, (a), (b), (d))
#define NMUL(a, b, d) native_mul(&flags, (a), (b), (d))
#define NMADD(a, b, c, d) native_madd(&flags, (a), (b), (c), (d))
#define NMSUB(a, b, c, d)                                                      \
  native_result(&flags,                                                        \
                _mm_sub_ps(operand4(a), _mm_mul_ps(operand4(b), operand4(c))), \
                (d))
#define SET(r, x, d) v[r] = native_merge(v[r], (x), (d))
#define LOAD(a) _mm_loadu_ps(memory[(a) & 1023].f)
#define STORE(a, x, d)                                                         \
  store_fields(memory[(a) & 1023].f, _mm_castps_si128(x), (d))
#define DIVIDE(w)                                                              \
  do {                                                                         \
    float den = _mm_cvtss_f32(arithmetic4(lane4((w), 3)));                     \
    q_flags = den == 0 ? 0x20 : 0;                                             \
    q = den == 0 ? vu_max(signbit(den))                                        \
                 : _mm_cvtss_f32(arithmetic4(                                  \
                       _mm_div_ss(_mm_set_ss(1), _mm_set_ss(den))));           \
  } while (0)
  do {
    v[8] = NMADD(acc, v[9], lane4(v[25], 1), 15);
    SET(10, NMUL(v[10], v[25], 12), 12);
    SET(25, NSUB(v[25], v[26], 12), 12);
    SET(17, NADD(v[28], lane4(v[7], 3), 2), 2);
    DIVIDE(v[7]);
    float q1 = q;
    SET(18, NADD(v[28], lane4(v[8], 3), 2), 2);
    v[6] = LOAD(256 + vi[5]);
    acc = native_merge(acc, NMUL(v[16], v[10], 12), 12);
    v[5] = LOAD(vi[5]);
    vi[5]++;
    SET(9, NMSUB(acc, v[14], v[10], 12), 12);
    SET(7, lane4(v[27], 3), 1);
    SET(10, NMADD(acc, v[14], v[10], 12), 12);
    SET(9, _mm_shuffle_ps(v[0], v[0], 0x39), 3);
    acc = NMUL(v[0], lane4(v[27], 2), 15);
    v[6] = native_itof(v[6], 1.0f / 16);
    SET(16, _mm_shuffle_ps(v[5], v[5], 0x39), 2);
    /* 2FE0 consumes the first Q while replacing it with 1/vf8.w. */
    v[7] = NMADD(acc, v[7], _mm_set1_ps(q1), 15);
    DIVIDE(v[8]);
    v[9] = NMUL(v[9], _mm_set1_ps(q1), 15);
    vi[7] =
        (uint16_t)_mm_cvtsi128_si32(_mm_srli_si128(_mm_castps_si128(v[5]), 12));
    acc = NMUL(v[1], lane4(v[5], 0), 15);
    acc = NMADD(acc, v[2], lane4(v[5], 1), 15);
    SET(16, _mm_shuffle_ps(v[16], v[16], 0x39), 4);
    SET(7, minmax4(v[7], lane4(v[27], 0), 0), 1);
    SET(10, _mm_shuffle_ps(v[0], v[0], 0x39), 3);
    SET(17, NADD(v[28], v[7], 12), 12);
    SET(14, _mm_shuffle_ps(v[6], v[6], 0x39), 2);
    acc = NMADD(acc, v[3], lane4(v[5], 2), 15);
    vi[7] &= vi[14];
    v[8] = NMUL(v[8], _mm_set1_ps(q), 15);
    SET(16, _mm_shuffle_ps(v[16], v[16], 0x39), 8);
    v[10] = NMUL(v[10], _mm_set1_ps(q), 15);
    vi[6] = (uint16_t)(vi[6] + 6);
    v[19] = native_ftoi(v[7], 16);
    STORE(vi[6] + 1, v[15], 15);
    clip = ((clip << 6) | native_clip(v[17], lane4(v[28], 3))) & 0xffffff;
    STORE(vi[6] + 4, v[15], 15);
    SET(18, NADD(v[28], v[8], 12), 12);
    STORE(vi[6], v[9], 15);
    SET(8, native_ftoi(v[8], 16), 14);
    STORE(vi[6] + 3, v[10], 15);
    v[7] = NMADD(acc, v[4], lane4(v[0], 3), 15);
    SET(8, lane4(v[19], 3), 1);
    acc = NMUL(v[11], lane4(v[6], 0), 15);
    vi[13] = (uint16_t)_mm_cvtsi128_si32(
        _mm_srli_si128(_mm_castps_si128(v[19]), 12));
    clip = ((clip << 6) | native_clip(v[18], lane4(v[28], 3))) & 0xffffff;
    SET(14, _mm_shuffle_ps(v[14], v[14], 0x39), 4);
    acc = NMADD(acc, v[12], lane4(v[6], 1), 15);
    STORE(vi[6] + 2, v[19], 15);
    /* 3068 SQ reads the old VF8, before its upper MADD commits. */
    STORE(vi[6] + 5, v[8], 15);
    v[8] = NMADD(acc, v[13], lane4(v[6], 2), 15);
    acc = native_merge(acc, NADD(v[30], v[0], 12), 12);
    vi[13] |= vi[3];
    SET(10, NMADD(acc, v[31], lane4(v[7], 3), 12), 12);
    vi[1] = (clip & 0xbefbef) != 0;
    /* FMAND at 3098 sees this 3078 result, four pairs earlier. */
    __m128 flag_value = v[10];
    SET(25, minmax4(v[25], lane4(v[0], 3), 0), 12);
    vi[1] |= vi[9];
    v[9] = NMUL(v[8], lane4(v[6], 3), 15);
    remaining--;
    vi[8] = (uint16_t)remaining;
    SET(16, NMUL(v[27], lane4(v[8], 3), 4), 4);
    unsigned clipped = vi[1];
    SET(10, minmax4(v[10], v[29], 0), 12);
    Upper branch = {0};
    fmac_flags4(&branch, flag_value, 12);
    vi[9] = (uint16_t)(branch.mac & vi[15]);
    if (clipped) {
      memory[(vi[6] + 2) & 1023].w[3] = vi[13];
      memory[(vi[6] + 5) & 1023].w[3] = vi[13];
      native_clipped_vertices += 2;
      pairs += 2;
    }
    acc = NADD(v[7], lane4(v[0], 0), 15);
    v[7] = NMSUB(acc, v[9], lane4(v[25], 1), 15);
    vi[9] |= vi[7];
    pairs += 36;
    native_vertices += 2;
  } while (remaining);
  for (unsigned i = 1; i < 32; i++)
    _mm_storeu_ps(cpu.vf[i].f, v[i]);
  _mm_storeu_ps(cpu.vacc, acc);
  cpu.q = q;
  Upper last = {0};
  fmac_flags4(&last, flags.last, flags.mask);
  unsigned mac = last.mac;
  unsigned status = (mac & 15 ? 1 : 0) | (mac & 240 ? 2 : 0) |
                    (mac & 3840 ? 4 : 0) | (mac & 61440 ? 8 : 0);
  unsigned sticky = (_mm_movemask_ps(_mm_castsi128_ps(flags.zero)) ? 1 : 0) |
                    (_mm_movemask_ps(_mm_castsi128_ps(flags.sign)) ? 2 : 0) |
                    (_mm_movemask_ps(_mm_castsi128_ps(flags.overflow)) ? 8 : 0);
  u->status = (entry_status & ~0x3fu) | status | q_flags | (sticky << 6);
  u->mac = mac;
  u->clip = u->clip_next = clip;
  u->n_flags = 0;
  u->flag_head = 0;
  u->q_ready = 0;
  u->cycle += 64;
  u->flag_cycle = u->cycle;
  return pairs;
#undef cpu
#undef NADD
#undef NSUB
#undef NMUL
#undef NMADD
#undef NMSUB
#undef SET
#undef LOAD
#undef STORE
#undef DIVIDE
}
