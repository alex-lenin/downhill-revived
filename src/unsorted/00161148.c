/* UNIT 0x00161148 0x00161198 */
struct Obj { char pad[0x78D0]; void *x78D0; };
struct State { char pad[0x50130]; struct Obj *x50130; };
extern struct State D_002AF2C0;
extern char D_003F52D0[];
extern void func_00190688(void *);
extern void func_0019A138(void *);
void func_00161148(void) {
    void *target;
    func_00190688(D_003F52D0);
    if (D_002AF2C0.x50130) {
        target = D_002AF2C0.x50130->x78D0;
        if (target) func_0019A138(target);
    }
}
