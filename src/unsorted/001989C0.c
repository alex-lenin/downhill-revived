/* UNIT 0x001989C0 0x00198AC0 */
typedef struct Obj {
    char pad0[0x418];
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
extern void func_0018E638(Obj *, int, void *);
extern int func_001F8440(int);
extern void func_0018E408(Obj *, int, int);
extern void func_0018E6B8(Obj *, int, void *);
extern void func_0018DB08(Obj *);

void func_001989C0(Obj *obj)
{
    int i;
    unsigned int mask;

    i = 0;
    mask = func_00193F38(D_004225A0);
    func_001B5AF0(D_003F52D0, 1, 1);
    func_0018E638(obj, 0x6B, &D_0029DE68);
    obj->x418 = 0;
    for (; func_001F8440(i) >= 0; i++) {
        if ((1 << func_001F8440(i)) & mask) {
            func_0018E408(obj, D_0026EFE8[func_001F8440(i)], 1);
        }
    }
    obj->x4A8 = 5;
    func_0018E6B8(obj, 0xA6, &D_0029DE68);
    func_0018DB08(obj);
}
