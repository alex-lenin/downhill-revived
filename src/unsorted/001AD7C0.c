/* UNIT 0x001AD7C0 0x001AD7E0 */
typedef struct Obj {
    char pad0[0xC];
    int xC;
    float x10;
    char pad14[0x28 - 0x14];
    int x28;
    char pad2C[0x30 - 0x2C];
    int x30;
    char pad34[0xF8 - 0x34];
    int xF8;
} Obj;

void func_001AD7C0(Obj *obj)
{
    obj->x10 = 1.0f;
    obj->x28 = 0;
    obj->x30 = 0;
    obj->xC = 0;
    obj->xF8 = 0;
}
