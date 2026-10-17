/* UNIT 0x00111150 0x00111160 */
struct Inner { char pad0[0x61D8]; int x61D8; };
struct Obj { struct Inner *x0; };

void func_00111150(struct Obj *obj)
{
    obj->x0->x61D8 = 1;
}
