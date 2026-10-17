/* UNIT 0x0017E848 0x0017E898 */
typedef struct Obj { char pad0[0x1CC]; int x1CC; char pad1[0xC4]; int x294; char pad2[0x166]; unsigned char x3FE; } Obj;
extern void func_0017EB00(Obj *, int, int);
extern void func_0017EF70(Obj *, int, int);
void func_0017E848(Obj *obj, int arg)
{
    int value;
    func_0017EB00(obj, 14, arg);
    value = obj->x3FE ? obj->x294 : obj->x1CC;
    if (value) func_0017EF70(obj, 14, 4);
}
