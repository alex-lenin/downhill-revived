/* UNIT 0x001E64F0 0x001E6548 */
struct Obj {
    int x0;
    char pad4[0x18 - 0x4];
    float x18;
    float x1C;
};

extern int D_0029D754;

int func_001E64F0(struct Obj *obj)
{
    if (obj->x0 != 0 || ((obj->x1C > 0.0f || obj->x18 > 0.0f) && D_0029D754 == 0)) {
        return 1;
    }
    return 0;
}
