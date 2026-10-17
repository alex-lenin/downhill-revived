/* UNIT 0x001E4530 0x001E4580 */
struct Obj { char pad0[0x510]; int x510; };
extern int D_004C5B40[];
void func_001E4530(struct Obj *obj) {
    if (obj->x510 == -1) {
        if (D_004C5B40[0] < 11) { obj->x510 = 120; return; }
        if (D_004C5B40[0] < 14) { obj->x510 = 360; return; }
        obj->x510 = 720;
    }
}

