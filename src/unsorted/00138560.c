/* UNIT 0x00138560 0x00138570 */
struct Obj { char pad0[0x4A4]; short x4A4; };

int func_00138560(struct Obj *obj)
{
    return obj->x4A4 > 1;
}
