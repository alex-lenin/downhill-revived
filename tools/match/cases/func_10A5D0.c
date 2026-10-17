struct Mtx { float m[16]; } __attribute__((aligned(16)));
struct Vec { float v[4]; } __attribute__((aligned(16)));
struct Slot { void *x0; int pad4[3]; struct Mtx x10; struct Vec x50; unsigned char used; };
extern struct Slot D_766450[64];
struct Slot *func_10A5D0(void) {
    struct Slot *s = D_766450;
    int i;
    for (i = 0; i < 64; i++, s++)
        if (!s->used) break;
    if (i == 64) return 0;
    s->used = 1;
    return s;
}
