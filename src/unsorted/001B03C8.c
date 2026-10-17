/* UNIT 0x001B03C8 0x001B03D8 */
typedef struct Obj {
    char pad0[0x10];
    int x10;
    char pad14[0xC];
    int x20;
} Obj;

void func_001B03C8(Obj *obj)
{
    obj->x10 = 0;
    obj->x20 = 0;
}
