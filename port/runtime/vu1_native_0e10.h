/* Program 0E10's particle sprite loop, 0EE0..0FC8, up to 49 sprites.
   Keep register names and upper/lower issue order visible for comparison.
   DIV Q is consumed seven pairs later. FCAND sees both CLIPs after their
   four-slot latency; the tail has no MAC/status reads before retirement.
   MR32 rotates raw UV bits, including NaNs. A clipped final sprite falls
   through a second MADD/ADDA and advances VI6, unlike a clipped interior one.
   Unsupported counts or a pending Q decline before any direct writes. */
static unsigned native_particles_0e10(Vu *u) {
  unsigned remaining = u->c->vi[8];
  if (!remaining || remaining > 49 || (u->q_ready && u->q_ready > u->cycle))
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
  do {
    v[17] = NMADD(acc, v[22], lane4(v[15], 3), 15);
    float denominator = _mm_cvtss_f32(arithmetic4(lane4(v[6], 3)));
    q_flags = denominator == 0 ? 0x20 : 0;
    q = denominator == 0 ? vu_max(signbit(denominator))
                         : _mm_cvtss_f32(arithmetic4(_mm_div_ss(
                               _mm_set_ss(1), _mm_set_ss(denominator))));
    SET(9, NADD(v[28], lane4(v[6], 3), 2), 2);
    SET(18, minmax4(v[18], lane4(v[18], 2), 0), 1);
    SET(11, v[15], 14);
    acc = native_merge(acc, NADD(v[0], v[18], 12), 12);
    SET(6, lane4(v[27], 3), 1);
    SET(16, NMADD(acc, v[17], lane4(v[17], 2), 12), 12);
    acc = NMUL(v[0], lane4(v[29], 3), 15);
    STORE(vi[6] + 1, v[8], 12);
    SET(18, minmax4(v[18], lane4(v[0], 3), 0), 1);
    vi[5] = (uint16_t)(vi[5] - 1);
    v[5] = LOAD(vi[5]);
    acc = NMADD(acc, v[6], _mm_set1_ps(q), 15);
    v[15] = LOAD(256 + vi[5]);
    v[7] = NMADD(acc, v[16], _mm_set1_ps(q), 15);
    v[6] = NMSUB(acc, v[16], _mm_set1_ps(q), 15);
    SET(11, NMUL(v[17], lane4(v[18], 3), 1), 1);
    SET(8, _mm_shuffle_ps(v[8], v[8], 0x39), 6);
    SET(15, native_itof(v[15], 1), 1);
    vi[1] =
        (uint16_t)_mm_cvtsi128_si32(_mm_srli_si128(_mm_castps_si128(v[15]), 8));
    SET(10, NADD(v[28], lane4(v[7], 3), 2), 2);
    SET(7, minmax4(v[7], v[31], 0), 1);
    SET(9, NADD(v[28], v[6], 12), 12);
    SET(8, _mm_shuffle_ps(v[8], v[8], 0x39), 12);
    SET(10, NADD(v[28], v[7], 12), 12);
    vi[1] &= vi[15];
    v[6] = native_ftoi(v[6], 16);
    v[7] = native_ftoi(v[7], 16);
    remaining--;
    vi[8] = (uint16_t)remaining;
    clip = ((clip << 6) | native_clip(v[9], lane4(v[28], 3))) & 0xffffff;
    clip = ((clip << 6) | native_clip(v[10], lane4(v[28], 3))) & 0xffffff;
    STORE(vi[6] + 3, v[8], 12);
    SET(11, native_ftoi(v[11], 1), 1);
    STORE(vi[6] + 2, v[6], 14);
    acc = NMUL(v[1], lane4(v[5], 0), 15);
    STORE(vi[6] + 4, v[7], 15);
    acc = NMADD(acc, v[2], lane4(v[5], 1), 15);
    v[8] = LOAD(520 + vi[1]);
    acc = NMADD(acc, v[3], lane4(v[5], 2), 15);
    vi[1] = (clip & 0xbef) != 0;
    v[6] = NMADD(acc, v[4], lane4(v[0], 3), 15);
    acc = NMUL(v[0], lane4(v[19], 0), 15);
    STORE(vi[6], v[11], 15);
    v[18] = NMADD(acc, v[20], lane4(v[5], 3), 15);
    acc = NADD(v[21], lane4(v[0], 0), 15);
    pairs += 28;
    native_vertices += 2;
    if (vi[1]) {
      vi[10] = (uint16_t)(vi[10] - 1);
      native_clipped_vertices += 2;
      if (!remaining) {
        v[18] = NMADD(acc, v[20], lane4(v[5], 3), 15);
        acc = NADD(v[21], lane4(v[0], 0), 15);
        vi[6] = (uint16_t)(vi[6] + 5);
        pairs += 2;
      }
    } else
      vi[6] = (uint16_t)(vi[6] + 5);
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
#undef NMUL
#undef NMADD
#undef NMSUB
#undef SET
#undef LOAD
#undef STORE
}
