/* UNIT 0x00138808 0x00138828 */
typedef struct Obj {
    char pad0[0x488];
    int x488;
    char pad48C[0x494 - 0x48C];
    int x494;
    char pad498[0x4AC - 0x498];
    short x4AC;
    char pad4AE[0x4B0 - 0x4AE];
    long x4B0;
} Obj;

void func_00138808(Obj *obj)
{
    obj->x4AC = 0;
    obj->x488 = 0;
    obj->x4B0 &= ~0x10;
    obj->x494 = 0;
}
