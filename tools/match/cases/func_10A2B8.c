struct B { int x0, x4, x8; void *xc; char x10[0x40]; char x50[1]; };
struct A { int x0, x4; unsigned short x8, xa; struct B *xc; };
extern void func_10A230(int, int);
extern void func_10A528(void *, void *, void *);
void func_10A2B8(struct A *a) {
    struct B *b = a->xc;
    if (b == 0) func_10A230(a->x8, a->xa);
    else func_10A528(b->xc, b->x10, b->x50);
}
