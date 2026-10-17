/* UNIT 0x001E3650 0x001E37F0 */
#include "menu_finalize.h"
typedef struct Obj {
    int x0;
    char pad4[0x414];
    int x418;
    char pad41C[0x44];
    int x460;
    char pad464[0x14];
    int x478;
    char pad47C[0x2C];
    int x4A8;
} Obj;

extern char D_003F52D0[];
extern char D_0029DE68[];
extern char D_004225A0[];
extern char D_004C37A0[];
extern int D_004C4718[];
extern int D_0026EFE8[];
extern void func_001B5AF0(void *, int, int);
extern void func_0018E638(Obj *, int, void *);
extern int func_00193F38(void *);
extern int func_001F8440(int);
extern int func_001E3060(void *, int);
extern void func_0018E408(Obj *, int, int);

extern void func_0018E6B8(Obj *, int, void *);

void func_001E3650(Obj *obj)
{
    int i;
    unsigned int mask;

    obj->x0 = 0;
    obj->x460 = 50;
    func_001B5AF0(D_003F52D0, 1, 1);
    if (D_004C4718[0] == 0)
        func_0018E638(obj, 0x78, D_0029DE68);
    else
        func_0018E638(obj, 0x79, D_0029DE68);
    i = 0;
    mask = func_00193F38(D_004225A0);
    obj->x418 = 0;
    for (; func_001F8440(i) >= 0; i++) {
        if (((1 << func_001F8440(i)) & mask)
            && func_001E3060(D_004C37A0, func_001F8440(i)) == 0
            && func_001F8440(i) != 0x15
            && func_001F8440(i) != 0x17)
            func_0018E408(obj, D_0026EFE8[func_001F8440(i)], 1);
    }
    obj->x478 = 1;
    FinalizeMenuLegacy(obj);
    func_0018E6B8(obj, 0xA5, D_0029DE68);
    obj->x4A8 = 5;
}
