/* UNIT 0x001272E0 0x001272F0 */
struct Obj { char pad0[0x14]; unsigned char x14; };

int func_001272E0(void *unused, struct Obj *obj)
{
    return obj->x14 >> 2;
}
