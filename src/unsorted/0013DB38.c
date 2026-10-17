/* UNIT 0x0013DB38 0x0013DC20 .sdata=0x29C3D0:0x29C3D4 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Obj {
    char pad0[0x18];
    int x18;
    char pad1C[0x28 - 0x1C];
    unsigned char x28;
} Obj;

typedef struct Menu {
    char pad0[0xC0];
    int xC0;
} Menu;

unsigned char D_0029C3D0 = 0;

extern Menu D_003623A0;
extern char D_004B2A90[];

extern void func_002083E8(Menu *menu);
extern void func_00208440(Menu *menu, int *list, int a2, int a3);
extern void func_001C57A8(char *, u128);

void func_0013DB38(Obj *obj)
{
    int *list;

    if (D_003623A0.xC0 == 0) {
        list = &obj->x18;
        if (*list != 0) {
            if (obj->x28 != D_0029C3D0) {
                func_002083E8(&D_003623A0);
                D_0029C3D0 = obj->x28;
            }
            func_00208440(&D_003623A0, list, obj->x28 ^ 1, 1);
            if (obj->x28 != 0) {
                /* packed name "S_PREVIOUS" */
                func_001C57A8(D_004B2A90, ((u128)0x5355 << 64) | (u128)0x4F49564552505F53L);
            } else {
                /* packed name "S_NEXT" */
                func_001C57A8(D_004B2A90, (u128)0x5458454E5F53L);
            }
        }
    }
}
