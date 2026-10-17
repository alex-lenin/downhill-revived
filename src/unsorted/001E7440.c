/* UNIT 0x001E7440 0x001E7460 */
struct Obj {
    char pad0[4];
    int x4;
    int x8;
    char padC[0xA0C - 0xC];
    int xA0C;
    int xA10;
    char padA14[4];
    char xA18;
    char xA19;
};

void func_001E7440(struct Obj *obj)
{
    obj->xA0C = 0;
    obj->xA10 = 0;
    obj->x8 = 0;
    obj->x4 = 0;
    obj->xA18 = 0;
    obj->xA19 = 0;
}
