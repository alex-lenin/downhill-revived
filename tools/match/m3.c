struct S { int x0, x4, x8; unsigned short xc, xe; int x10; };
struct Buf { int a, b, c, d; Buf(); void Init(int, int, int); void Go(int); };
struct Buf2 { int a, b, c, d; void Init(int, int, int); void Go(int); };
void c1(S *s) { Buf2 buf; Buf2 *p = &buf; buf.c = 0; p->Init(s->x8, s->xc & 0x7FF, s->xe); p->Go(1); }
void c2(S *s) { Buf2 buf; buf.c = 0; buf.Init(s->x8, s->xc & 0x7FF, s->xe); buf.Go(true); }
struct V { int x8() ; };
void c3(S *s) { Buf2 buf; buf.c = 0; buf.Init(s->x8, s->xc & 0x7FF, s->xe); int one = 1; buf.Go(one); }
extern "C" void func_2038E0(void *, int);
extern "C" void func_203590(void *, int, int, int);
void c4(S *s) { int buf[4]; buf[2] = 0; func_203590(buf, s->x8, s->xc & 0x7FF, s->xe); func_2038E0(buf, 1); }
