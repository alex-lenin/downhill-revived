/* UNIT 0x001EB0F8 0x001EB108 */
typedef struct Obj {
    char pad0[0x10];
    int x10;
    int x14;
} Obj;

void func_001EB0F8(Obj *obj)
{
    obj->x10 = 0;
    obj->x14 = 0;
}
