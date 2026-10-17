/* UNIT 0x00122E58 0x00122E68 */
struct Obj { char pad0[0x8]; unsigned int x8; };

int func_00122E58(struct Obj *obj)
{
    return obj->x8 > 1;
}
