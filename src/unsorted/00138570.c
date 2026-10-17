/* UNIT 0x00138570 0x00138580 */
struct Obj { char pad0[0x4B0]; unsigned char x4B0; };

int func_00138570(struct Obj *obj)
{
    return obj->x4B0 & 1;
}
