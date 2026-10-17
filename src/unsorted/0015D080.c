/* UNIT 0x0015D080 0x0015D0C8 */
struct Obj { char pad0[0x460]; int x460; };
extern char D_003F52D0[];
extern void func_001B5AF0(void *, int, int);
extern void func_0018DB08(struct Obj *);
void func_0015D080(struct Obj *obj) {
    func_001B5AF0(D_003F52D0, 1, 1);
    func_0018DB08(obj);
    obj->x460 = 0xae;
}
