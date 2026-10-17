/* UNIT 0x0018AAE8 0x0018AB18 */
typedef struct Obj {
    int x0;
} Obj;

extern void func_00187728(Obj *obj);

void func_0018AAE8(Obj *obj)
{
    func_00187728(obj);
    obj->x0 = 4;
}
