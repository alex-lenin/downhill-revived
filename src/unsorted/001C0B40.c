/* UNIT 0x001C0B40 0x001C0B60 */
struct Obj001C0B40 {
    char pad0[0x4];
    int x4;
    int x8;
    char padC[0x4];
    float x10;
    float x14;
    float x18;
    char pad1C[0x4];
    int x20;
    int x24;
};

void func_001C0B40(struct Obj001C0B40 *obj, int x8, int x4, int x20, int x24,
                   float x10, float x14, float x18)
{
    obj->x8 = x8;
    obj->x4 = x4;
    obj->x20 = x20;
    obj->x24 = x24;
    obj->x10 = x10;
    obj->x14 = x14;
    obj->x18 = x18;
}
