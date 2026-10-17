/* UNIT 0x00208240 0x00208298 */
struct Obj { char pad0[0x20]; int x20; };
extern int func_002082A8(struct Obj *);
extern int func_00208298(struct Obj *);
extern int func_002082C8(struct Obj *);
extern int D_0029D9B0;
extern struct Obj D_004D02C0;
void func_00208240(struct Obj *obj) {
    if (func_002082A8(obj)) {
        func_00208298(obj);
        obj->x20 = 8;
        D_0029D9B0 = 0;
        return;
    }
    if (func_002082C8(&D_004D02C0)) obj->x20 = 6;
}
