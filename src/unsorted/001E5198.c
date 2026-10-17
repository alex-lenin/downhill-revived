/* UNIT 0x001E5198 0x001E51A8 .sdata=0x29D790:0x29D794 */
struct Obj {
    char pad0[0x38];
    float x38;
};

float D_0029D790 = 1.0f;

float func_001E5198(struct Obj *obj)
{
    return D_0029D790 * obj->x38;
}
