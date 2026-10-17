/* UNIT 0x001F84A8 0x001F8558 */
typedef struct Obj {
    char pad0[0x500];
    int x500;
    int x504;
    int x508;
    int x50C;
} Obj;

extern void func_0023D898(int, float *, float *);
extern void func_0023D8D8(int, float, float);

void func_001F84A8(Obj *p)
{
    int old = p->x500;
    int max = p->x504;
    int sum = old + p->x508;
    int v;
    float f;
    float a, b;

    v = (sum < max) ? sum : max;
    if (v < 0) {
        v = 0;
    }
    p->x50C--;
    p->x500 = v;
    if (v != old) {
        f = (float)v / (float)max * 2312.0f + 688.0f;
        func_0023D898(0, &a, &b);
        func_0023D8D8(0, f, b);
    }
}
