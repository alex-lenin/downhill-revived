/* UNIT 0x001BE9D0 0x001BEA48 */
typedef struct { int x0, x4, x8; char padC[0x14]; int x20, x24, x28, x2C, x30, x34; } Obj;
extern int func_00238F48(void);
extern int func_00222B08(int, int);
void func_001BE9D0(Obj *p, int a, int b, int c, int d, int e, int f) {
    p->x8 = a;
    p->x4 = b;
    p->x20 = c;
    p->x24 = d;
    p->x28 = func_00238F48();
    p->x30 = e;
    p->x34 = f;
    if (e != -1) p->x2C = func_00222B08(e, f);
    else p->x2C = 0;
}
