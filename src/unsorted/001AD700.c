/* UNIT 0x001AD700 0x001AD710 */
struct Obj {
    char pad0[0x1C];
    float x1C;
    float x20;
};

void func_001AD700(struct Obj *obj, float value)
{
    obj->x20 = value;
    obj->x1C = __builtin_fabsf(value);
}
