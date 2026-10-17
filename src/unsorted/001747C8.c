/* UNIT 0x001747C8 0x001747F0 */
typedef struct Obj {
    char pad0[0xC];
    int xC;
    int x10;
    int x14;
    int x18;
    char pad1C[0xC];
    char x28;
    char x29;
    char x2A;
    char x2B;
} Obj;

void func_001747C8(Obj *obj)
{
    obj->xC = 0;
    obj->x10 = 0;
    obj->x18 = 0;
    obj->x14 = 0;
    obj->x28 = 0;
    obj->x29 = 0;
    obj->x2A = 0;
    obj->x2B = 0;
}
