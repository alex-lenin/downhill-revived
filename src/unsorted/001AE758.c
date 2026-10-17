/* UNIT 0x001AE758 0x001AE788 */
typedef struct Obj { char pad0[0x20]; float x20, x24, x28; } Obj;
void func_001AE758(Obj *obj, int index, float x, float y, float z) {
    obj = (Obj *)((char *)obj + index * 16);
    obj->x20 += x;
    obj->x24 += y;
    obj->x28 += z;
}
