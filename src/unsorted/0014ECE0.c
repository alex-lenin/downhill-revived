/* UNIT 0x0014ECE0 0x0014ED28 */
struct Obj { int x0; char pad[0x464]; int x468; };
extern char D_003F52D0[];
extern void func_001B5AF0(void *, int, int);
extern void func_0018DB08(struct Obj *);
void func_0014ECE0(struct Obj *obj) {
    func_001B5AF0(D_003F52D0, 1, 1);
    obj->x0 = 0;
    obj->x468 = 8;
    func_0018DB08(obj);
}
