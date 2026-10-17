/* UNIT 0x001AA6B8 0x001AA770 */
struct Obj { int x0; float x4, x8, xC; short x10, x12, x14, x16; int x18, x1C, x20, x24, x28; char x2C[1]; };
extern void func_00223348(float *, void *);
extern void func_001CF450(void *);
void func_001AA6B8(struct Obj *p, int a, float *v, void *r)
{
    float q[4];
    p->x18 = a;
    p->x4 = v[0];
    p->x8 = v[1];
    p->xC = v[2];
    func_00223348(q, r);
    p->x20 = 0;
    p->x10 = q[0] * 32767.0f;
    p->x12 = q[1] * 32767.0f;
    p->x14 = q[2] * 32767.0f;
    p->x16 = -q[3] * 32767.0f;
    func_001CF450(p->x2C);
}
