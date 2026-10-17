/* UNIT 0x00111A88 0x00111A98 */
struct Inner { char pad0[0x798C]; int x798C; };
struct Obj { struct Inner *x0; };

int func_00111A88(struct Obj *obj)
{
    return obj->x0->x798C;
}
