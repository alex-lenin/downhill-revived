/* UNIT 0x0019A7C0 0x0019A7F8 */
typedef struct Obj { char pad[0x460]; int x460; int x464; char pad468[0x40]; int x4A8; } Obj;
extern void func_0018DB08(Obj *);
void func_0019A7C0(Obj *obj) {
    obj->x464 = 60;
    obj->x460 = 70;
    obj->x4A8 = 5;
    func_0018DB08(obj);
}
