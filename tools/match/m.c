struct S { int x0, x4, x8; unsigned short xc, xe; int x10; };
struct Buf { int a, b, c, d; };
struct T { int x0, x4; float x8; };
extern void func_10A528(int, int *, void *);
extern void func_203590(struct Buf *, int, int, int);
extern void func_2038E0(struct Buf *, int);
extern void func_148560(void *, int);
extern void func_236B28(void *, void *, int, int);
extern int D_43F348; extern char D_37EA60[], D_759780[], D_260868[];

void func_10A388(void *a, void *b, struct S *s) { func_10A528(s->x8, &s->x10, a); }

void func_10A3B0(struct S *s) {
    struct Buf buf;
    buf.c = 0;
    func_203590(&buf, s->x8, s->xc & 0x7FF, s->xe);
    func_2038E0(&buf, 1);
}

void func_10A3F0(int a, struct T *t) {
    D_43F348 = 0;
    func_148560(D_37EA60, 7);
    func_236B28(D_759780, D_260868, a, (int)(t->x8 * 60.0f));
}
