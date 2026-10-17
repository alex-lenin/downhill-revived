/* UNIT 0x001BBD50 0x001BBD80 */
struct Obj001BBD50 {
    char pad0[0x8];
    int x8;
    char padC[0xC];
    int x18;
};

int func_001BBD50(struct Obj001BBD50 *obj)
{
    if (obj->x18 == 1) {
        obj->x8 = 0x3F;
    } else if (obj->x18 == 2) {
        obj->x8 = 0x40;
    } else {
        obj->x8 = 0x41;
    }
    return obj->x8;
}
