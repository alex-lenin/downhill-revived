/* UNIT 0x001907B0 0x001907D0 */
typedef struct Obj {
    char pad0[0x188];
    int x188;
} Obj;

extern void func_00190740(Obj *obj, int value);

void func_001907B0(Obj *obj)
{
    func_00190740(obj, obj->x188);
}
