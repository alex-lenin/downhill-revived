/* UNIT 0x001A9568 0x001A9598 */
typedef struct Obj {
    char pad0[0x18];
    int x18;
} Obj;

extern int func_00220230(int a);

void func_001A9568(Obj *obj)
{
    obj->x18 = func_00220230(0);
}
