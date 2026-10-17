/* UNIT 0x002081B8 0x00208210 */
struct Obj { char pad0[0x20]; int x20; };
extern int func_002082A8(struct Obj *);
extern int func_00208298(struct Obj *);
extern int func_002082C8(struct Obj *);
extern int D_0029D9B0;
void func_002081B8(struct Obj *obj) {
    if (func_002082A8(obj)) {
        func_00208298(obj);
        obj->x20 = 5;
        D_0029D9B0 = 0;
        return;
    }
    if (func_002082C8(obj)) obj->x20 = 3;
}

