/* UNIT 0x00124CE8 0x00124CF8 */
struct Obj { char pad0[0x8]; unsigned int x8; };

int func_00124CE8(struct Obj *obj)
{
    return obj->x8 > 1;
}
