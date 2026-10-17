/* UNIT 0x001B12A0 0x001B12B0 */
struct Obj {
    char pad0[0x40];
    float x40;
};

void func_001B12A0(struct Obj *obj)
{
    obj->x40 = 1.0f;
}
