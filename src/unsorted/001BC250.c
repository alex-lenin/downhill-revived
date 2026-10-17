/* UNIT 0x001BC250 0x001BC278 */
typedef struct Obj {
    char pad0[0x60C];
    unsigned int x60C;
} Obj;

unsigned int func_001BC250(Obj *obj)
{
    return obj->x60C * 1000 / 60;
}
