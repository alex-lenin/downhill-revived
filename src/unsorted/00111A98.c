/* UNIT 0x00111A98 0x00111B18 */
struct Inner { char pad0[0x796c]; int x796c; char pad7970[8]; int x7978; char pad797c[0x10]; int x798c; };
struct Obj { struct Inner *x0; char pad4[0x39c]; int x3a0; };
struct Global { char pad0[0x501a8]; int x501a8; char pad501ac[0x10]; int x501bc; };
extern struct Global D_002AF2C0;
int func_00111A98(struct Obj *obj) {
    struct Inner *inner = obj->x0;
    int state = inner->x796c;
    if (state == 0x1c0 || state == 0x1e0) return D_002AF2C0.x501a8 - 1;
    if ((obj->x3a0 >> 11) & 1) return 1;
    if (D_002AF2C0.x501bc == inner->x7978) return 1;
    return inner->x798c;
}
