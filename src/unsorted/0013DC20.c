/* UNIT 0x0013DC20 0x0013DC78 */
struct Obj { char pad0[0x14]; short x14; };
struct Global { char pad0[0x64]; char x64[4]; };
extern int D_00362460[];
extern struct Global D_003F52D0;
extern unsigned int func_00190D60(void *, int, unsigned int);

static inline long func_0013DC20_inner(struct Global *g, int index)
{
    void *p = g->x64;
    long result;

    result = 0;
    if (index >= 0) {
        int value = func_00190D60(p, index, 1) != 0;
        result = value;
    }
    return result;
}

long func_0013DC20(struct Obj *obj)
{
    if (D_00362460[0]) {
        return 0;
    }
    return func_0013DC20_inner(&D_003F52D0, obj->x14);
}
