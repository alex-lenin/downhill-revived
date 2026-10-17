/* UNIT 0x0018F2F0 0x0018F310 */
typedef struct Obj {
    char pad0[0x4A8];
    int x4A8;
} Obj;

extern void func_0018DB08(Obj *obj);

void func_0018F2F0(Obj *obj)
{
    obj->x4A8 = 5;
    func_0018DB08(obj);
}
