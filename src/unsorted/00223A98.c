/* UNIT 0x00223A98 0x00223AD8 */
struct S { int x0, x4, x8, xc; };
extern void func_00223B00(struct S *);
struct S *func_00223A98(struct S *s, int a, int b) {
    s->xc = a + b;
    s->x0 = a;
    if (b) func_00223B00(s);
    return s;
}
