/* UNIT 0x00125D58 0x00125D68 */
struct Obj { char pad0[0x8]; unsigned int x8; };

int func_00125D58(struct Obj *obj)
{
    return obj->x8 > 1;
}
