struct S { int x0, x4, x8; unsigned short xc, xe; int x10; };
struct Buf { int a, b, c, d; void Init(int, int, int); void Go(int); };
void f_a(S *s) { Buf buf; buf.c = 0; buf.Init(s->x8, s->xc & 0x7FF, s->xe); buf.Go(1); }
void f_b(S *s) { Buf buf; buf.c = 0; int w = s->xc & 0x7FF; buf.Init(s->x8, w, s->xe); buf.Go(1); }
struct Q { int x0, x4, x8; int x10; void M(void *a, S *s); };
extern void func_10A528(int, int *, void *);
void g_a(void *a, void *b, S *s) { func_10A528(s->x8, &s->x10, a); }
struct R { void M(void *b, S *s); };
void R::M(void *b, S *s) { func_10A528(s->x8, &s->x10, this); }
