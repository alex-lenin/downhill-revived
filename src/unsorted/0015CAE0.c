/* UNIT 0x0015CAE0 0x0015CC08 */
typedef struct Obj {
    int x0;
    char pad4[0x418 - 0x4];
    int x418;
    char pad41C[0x4A8 - 0x41C];
    int x4A8;
} Obj;

extern char D_004225A0[];
extern char D_003F52D0[];
extern int D_0029DE68;
extern int D_0026EFE8[];
extern int func_00193F38(void *);
extern void func_001B5AF0(void *, int, int);
extern int func_001F8440(int);
extern void func_0018E408(Obj *, int, int);
extern void func_0018E6B8(Obj *, int, void *);
extern void func_0018DB08(Obj *);

void func_0015CAE0(Obj *obj)
{
    int i;
    unsigned int mask;

    i = 0;
    obj->x0 = 0;
    mask = func_00193F38(D_004225A0);
    obj->x418 = 0;
    func_001B5AF0(D_003F52D0, 1, 1);
    for (; func_001F8440(i) >= 0; i++) {
        if (((1 << func_001F8440(i)) & mask) && func_001F8440(i) != 21
            && func_001F8440(i) != 23) {
            func_0018E408(obj, D_0026EFE8[func_001F8440(i)], 1);
        }
    }
    obj->x4A8 = 5;
    func_0018E6B8(obj, 0xA6, &D_0029DE68);
    func_0018DB08(obj);
}
