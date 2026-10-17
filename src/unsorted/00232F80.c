/* UNIT 0x00232F80 0x00233048 */
typedef struct Obj {
    int x0;
    int x4[32];
    int x84[32];
    int x104[32];
    int x184;
    char pad188[0x18C - 0x188];
    int x18C[17];
    int x1D0;
    int x1D4;
    int x1D8;
    int x1DC;
    int x1E0;
    int x1E4;
    int x1E8;
    int x1EC;
    int x1F0;
    int x1F4;
    char pad1F8[0x5F8 - 0x1F8];
    char x5F8;
    char x5F9;
    char pad5FA[0x5FC - 0x5FA];
    void *vtbl;
} Obj;

extern char D_002887C8[];
extern void func_00234F38(Obj *obj);

Obj *func_00232F80(Obj *obj)
{
    int i;

    obj->vtbl = D_002887C8;
    obj->x5F9 = 1;
    obj->x1D0 = 0;
    obj->x1D8 = 0;
    obj->x1D4 = 0;
    obj->x1DC = 0;
    obj->x1E0 = 0;
    obj->x1E4 = 0;
    obj->x1E8 = 0;
    obj->x5F8 = 0;
    for (i = 0; i < 32; i++) {
        obj->x84[i] = 0;
        obj->x4[i] = 0;
        obj->x104[i] = -1;
    }
    obj->x184 = 0;
    obj->x1EC = 1;
    obj->x0 = 0;
    obj->x1F0 = 0;
    obj->x1F4 = 0;
    for (i = 16; i >= 0; i--) {
        obj->x18C[i] = 0;
    }
    func_00234F38(obj);
    return obj;
}
