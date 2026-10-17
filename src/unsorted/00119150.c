/* UNIT 0x00119150 0x00119180 */
struct Obj {
    char pad0[0xC];
    short xC;
};

extern float D_0039E818[];

int func_00119150(int unused, struct Obj *obj, int scale)
{
    int value = obj->xC;

    if (scale) {
        value = (float)value * D_0039E818[0];
    }
    return value;
}
