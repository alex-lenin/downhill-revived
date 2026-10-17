/* UNIT 0x001E6E50 0x001E6E60 */
typedef struct Obj {
    char pad0[0x10EC];
    int x10EC;
    int x10F0;
    int x10F4;
} Obj;

void func_001E6E50(Obj *obj, int value)
{
    obj->x10EC = value;
    obj->x10F4 = 0;
    obj->x10F0 = 0;
}
