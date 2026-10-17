/* UNIT 0x001613E8 0x00161408 */
typedef struct Obj {
    char pad0[0x10];
    int x10;
    int x14;
    int x18;
    int x1C;
    int x20;
    int x24;
    int x28;
} Obj;

void func_001613E8(Obj *obj)
{
    obj->x10 = 0;
    obj->x14 = 0;
    obj->x18 = 0;
    obj->x1C = 0;
    obj->x24 = 0;
    obj->x20 = 0;
    obj->x28 = 0;
}
