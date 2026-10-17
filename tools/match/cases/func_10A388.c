struct S { int x0, x4, x8; unsigned short xc, xe; int x10; };
extern void func_10A528(int, int *, void *);
void func_10A388(void *a, void *b, struct S *s) { func_10A528(s->x8, &s->x10, a); }
