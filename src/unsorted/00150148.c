/* UNIT 0x00150148 0x00150198 */
struct Obj { char pad[0x460]; int x460; int x464; int x468; };
extern char D_003F52D0[];
extern void func_001B5AF0(void *, int, int);
extern void func_0018DB08(struct Obj *);
void func_00150148(struct Obj *obj) {
    func_001B5AF0(D_003F52D0, 1, 1);
    func_0018DB08(obj);
    obj->x468 = 8;
    obj->x460 = 0xAE;
}

