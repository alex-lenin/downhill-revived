struct S { int x0, x4, x8; unsigned short xc, xe; int x10; };
extern void func_2038E0(void *, int);
extern void func_203590(void *, int, int, int);
void func_10A3B0(struct S *s) { int buf[4]; buf[2] = 0; func_203590(buf, s->x8, s->xc & 0x7FF, s->xe); func_2038E0(buf, 1); }
