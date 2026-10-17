/* UNIT 0x001908D0 0x00190900 */
typedef struct Obj {
    char pad0[0x18C];
    int x18C;
} Obj;

extern void func_00190528(Obj *obj, int a, int b);

void func_001908D0(Obj *obj)
{
    func_00190528(obj, obj->x18C, 0);
    obj->x18C = 0;
}
