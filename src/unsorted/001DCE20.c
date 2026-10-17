/* UNIT 0x001DCE20 0x001DCE58 */
struct Obj {
    char pad0[0x40];
    float x40;
    float x44;
    float x48;
    float x4C;
    float x50;
    float x54;
};

void func_001DCE20(struct Obj *obj)
{
    obj->x4C = obj->x44 / obj->x40;
    obj->x48 = obj->x44 * 60.0f / obj->x54 / obj->x40;
    obj->x50 = obj->x48 + obj->x4C;
}
