/* UNIT 0x001FB550 0x001FB560 */
typedef struct Obj {
    char pad0[0x7B9E];
    char x7B9E;
} Obj;

void func_001FB550(int unused, Obj *obj)
{
    obj->x7B9E = 1;
}
