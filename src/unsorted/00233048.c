/* UNIT 0x00233048 0x00233118 */
typedef struct Obj {
    void *x0;
    void *x4[32];
    int x84[32];
    int x104[32];
    int x184;
    char pad188[0x4C];
    int x1D4;
    int x1D8;
    int x1DC;
    int x1E0;
    char pad1E4[8];
    int x1EC;
    int x1F0;
    int x1F4;
    char pad1F8[0x404];
    void *x5FC;
} Obj;

extern char D_002887C8[];

extern void func_001FD3B8(void *);
extern void func_002351C0(void *, int);
extern void func_001FD340(Obj *);

void func_00233048(Obj *obj, int flags)
{
    int i;

    obj->x1D8 = 0;
    obj->x5FC = D_002887C8;
    obj->x1D4 = 0;
    obj->x1DC = 0;
    obj->x1E0 = 0;
    for (i = 0; i < 32; i++) {
        obj->x84[i] = 0;
        if (obj->x4[i] != 0) {
            func_001FD3B8(obj->x4[i]);
        }
        obj->x4[i] = 0;
        obj->x104[i] = -1;
    }
    obj->x184 = 0;
    if (obj->x0 != 0) {
        func_002351C0(obj->x0, 3);
    }
    obj->x1EC = 1;
    obj->x0 = 0;
    obj->x1F0 = 0;
    obj->x1F4 = 0;
    if (flags & 1) {
        func_001FD340(obj);
    }
}
