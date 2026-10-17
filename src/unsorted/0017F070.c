/* UNIT 0x0017F070 0x0017F160 */
typedef struct Slot {
    char pad0[0x220];
} Slot;

typedef struct Obj {
    int x0;
    int x4;
    char pad8[0x8];
    int x10;
    int x14;
    char pad18[0x8];
    int x20;
    int x24;
    char pad28[0x8];
    Slot x30[40];
    int x5530[40];
    int x55D0[40];
    float x5670;
    float x5674;
    int x5678;
    float x567C[8];
    int x569C[40];
    int x573C;
    int x5740;
    int x5744;
    int x5748;
    int x574C[8];
    int x576C;
    int x5770;
} Obj;

extern float D_0029CAA8[];
extern void func_0017FA30(Slot *);

void func_0017F070(Obj *obj)
{
    int i;
    int j;
    float init;

    for (i = 0; i < 40; i++) {
        func_0017FA30(&obj->x30[i]);
        obj->x5530[i] = 0;
        obj->x55D0[i] = 0;
        obj->x569C[i] = 0;
    }
    init = D_0029CAA8[0];
    obj->x5670 = 0.36666667f;
    obj->x5674 = 0.36666667f;
    obj->x5678 = 0;
    for (j = 0; j < 8; j++) {
        obj->x567C[j] = init;
        obj->x574C[j] = -1;
    }
    obj->x5770 = 1;
    obj->x573C = 0;
    obj->x5740 = 0;
    obj->x5744 = 0;
    obj->x5748 = 0;
    obj->x576C = 0;
    obj->x0 = 0;
    obj->x4 = 0;
    obj->x10 = 0;
    obj->x14 = 0;
    obj->x20 = 0;
    obj->x24 = 0;
}
