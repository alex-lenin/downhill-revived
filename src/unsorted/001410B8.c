/* UNIT 0x001410B8 0x00141128 */
struct Arena { int x0, x4; };
struct Obj { char pad0[0x94]; int x94, x98, x9C; };
extern struct Arena D_00663B08[], D_004D0368[];
void func_001410B8(struct Obj *obj) {
    int p, q, r;
    p = (D_00663B08[0].x4 + 127) & ~127;
    D_00663B08[0].x4 = p + 0x9B000;
    obj->x94 = p;
    q = (D_004D0368[0].x4 + 127) & ~127;
    D_004D0368[0].x4 = q + 0x9B000;
    obj->x98 = q;
    r = (D_00663B08[0].x4 + 3) & ~3;
    D_00663B08[0].x4 = r + 0x20000;
    obj->x9C = r;
}
