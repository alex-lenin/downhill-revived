/* UNIT 0x001E4110 0x001E4120 */
typedef struct Obj {
    char pad0[0x500];
    int x500;
    char pad504[0xC];
    int x510;
} Obj;

void func_001E4110(Obj *obj)
{
    obj->x500 = 0;
    obj->x510 = -1;
}
