struct Mtx { float m[16]; } __attribute__((aligned(16)));
struct Vec { float v[4]; } __attribute__((aligned(16)));
struct Slot { void *x0; int pad4[3]; struct Mtx x10; struct Vec x50; unsigned char used; };
struct Src { int x0; float x4; };
extern struct Slot *func_10A5D0(void);
extern char D_759780[], D_260848[];
extern void func_236B28(void *, void *, void *, int);
void func_10A528(struct Src *src, struct Mtx *m, struct Vec *v) {
    struct Slot *s = func_10A5D0();
    if (s) {
        s->x0 = src;
        s->x10 = *m;
        s->x50 = *v;
        func_236B28(D_759780, D_260848, s, (int)(src->x4 * 60.0f));
    }
}
