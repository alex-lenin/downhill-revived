/* UNIT 0x001E7508 0x001E7520 */
struct Obj {
    int x0;
    char pad4[0x3C - 0x4];
    int x3C;
    char pad40[0xA0 - 0x40];
    int xA0;
    char padA4[0x104 - 0xA4];
    int x104;
    char pad108[0x168 - 0x108];
    int x168;
};

void func_001E7508(struct Obj *obj, int value)
{
    obj->x0 = value;
    obj->x3C = 0;
    obj->xA0 = 0;
    obj->x104 = 0;
    obj->x168 = 0;
}
