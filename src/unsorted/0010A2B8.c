/* UNIT 0x0010A2B8 0x0010A2F8 */
struct B { int x0, x4, x8; void *xc; char x10[0x40]; char x50[1]; };
struct A { int x0, x4; unsigned short x8, xa; struct B *xc; };
extern void func_0010A230(int, int);
extern void func_0010A528(void *, void *, void *);

void func_0010A2B8(struct A *a) {
    struct B *b = a->xc;
    if (b == 0) func_0010A230(a->x8, a->xa);
    else func_0010A528(b->xc, b->x10, b->x50);
}
