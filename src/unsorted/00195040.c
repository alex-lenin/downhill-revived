/* UNIT 0x00195040 0x001950F8 */
struct Elem {
    char pad0[0x50];
};

struct Obj {
    struct Elem elems[10];
    int x320[33];
    int x3A4[33];
    char x428[33];
    char x449[33];
    short x46A[43];
    short x4C0[43];
    short x516[8];
    short x526;
    int x528;
};

extern void func_001951B0(struct Elem *e);

void func_00195040(struct Obj *obj)
{
    int i;

    for (i = 0; i < 33; i++) {
        obj->x320[i] = 0x0FFFFFFF;
        obj->x3A4[i] = 0;
        obj->x428[i] = 0x7F;
        obj->x449[i] = 0;
    }
    for (i = 0; i < 43; i++) {
        obj->x46A[i] = 0;
        obj->x4C0[i] = 0;
    }
    obj->x526 = 0;
    obj->x528 = 0;
    for (i = 0; i < 10; i++) {
        func_001951B0(&obj->elems[i]);
    }
}
