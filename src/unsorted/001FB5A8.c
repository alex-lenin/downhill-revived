/* UNIT 0x001FB5A8 0x001FB5B8 */
typedef struct Obj {
    char pad0[0x7B9F];
    char x7B9F;
} Obj;

void func_001FB5A8(int unused, Obj *obj)
{
    obj->x7B9F = 1;
}
