/* Pure per-primitive metadata. Key raw registers, not state_version: replay and
   PRMODE changes may bypass that epoch. Never cache VRAM ownership or aliases. */
#ifndef NATIVE_FRONTEND_H
#define NATIVE_FRONTEND_H
#include <stdint.h>
typedef struct {
    unsigned zbp, fbp, fbw, zpsm, fpsm;
    int never, zwrite, colour, z_full, colour_full;
} NativeExactState;
typedef struct {
    uint64_t zbuf, test, frame;
    unsigned abe, valid;
    NativeExactState state;
} NativeExactCache;
static const NativeExactState *native_exact_state(NativeExactCache *c,
                                                 uint64_t zbuf, uint64_t test,
                                                 uint64_t frame, unsigned abe)
{
    if (c->valid && c->zbuf == zbuf && c->test == test && c->frame == frame && c->abe == abe)
        return &c->state;
    c->zbuf = zbuf; c->test = test; c->frame = frame; c->abe = abe; c->valid = 1;
    NativeExactState *s = &c->state;
    s->zbp = (unsigned)zbuf & 511; s->fbp = (unsigned)frame & 511;
    s->fbw = (unsigned)(frame >> 16) & 63;
    s->zpsm = 48 | ((unsigned)(zbuf >> 24) & 15); s->fpsm = (unsigned)(frame >> 24) & 63;
    s->never = (test >> 16 & 1) && !(test >> 17 & 3);
    s->zwrite = !(zbuf >> 32 & 1); s->colour = (uint32_t)(frame >> 32) != 0xffffffffu;
    unsigned ztst = test >> 16 & 1 ? (unsigned)(test >> 17) & 3 : 1;
    unsigned ate = test & 1, atst = (unsigned)(test >> 1) & 7;
    unsigned afail = (unsigned)(test >> 12) & 3, date = (unsigned)(test >> 14) & 1;
    s->z_full = s->zwrite && ztst == 1 && !date && (!ate || atst == 1 || afail == 2) && (s->zpsm & 7) != 2;
    /* The caller still combines this with the current, dynamic alias test. */
    s->colour_full = ztst == 1 && !date && (!ate || atst == 1) && !abe &&
                     s->fpsm == 0 && !(uint32_t)(frame >> 32);
    return s;
}
#endif
