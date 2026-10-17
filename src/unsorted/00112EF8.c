/* UNIT 0x00112EF8 0x00113098 */
struct Position { char pad[0x18]; float x18; };
struct Rider { char pad0[0x11E8]; int x11E8; char pad1[0x60]; int x124C; char pad2[0x184]; unsigned char x13D4; char pad3[0x2CEB]; char x40C0[4]; char pad4[0x3868]; struct Position *x792C; };
struct Obj { struct Rider *x0; int x4; struct Rider *x8; char pad0[0x400]; int x40C; char pad1[0x30]; unsigned long x440; };
extern float func_001385A8(void *);
void func_00112EF8(struct Obj *obj) {
    struct Rider *other, *rider;
    int active;
    obj->x440 &= ~(1UL << 34);
    obj->x440 &= ~(1UL << 35);
    obj->x440 &= ~(1UL << 38);
    other = obj->x8;
    if (other != 0 && obj->x40C >= 1801) {
        if (other->x792C->x18 - obj->x0->x792C->x18 > 120.0f) {
            active = obj->x0->x13D4 ? obj->x0->x124C : obj->x0->x11E8;
            if (active) {
                if (func_001385A8(obj->x0->x40C0) > 0.35f) goto set_flag;
                obj->x440 &= ~(1UL << 38);
                return;
            }
            goto inactive;
        }
    } else {
        active = obj->x0->x13D4 ? obj->x0->x124C : obj->x0->x11E8;
        if (active) {
            if (func_001385A8(obj->x0->x40C0) > 0.35f) goto set_flag;
            obj->x440 &= ~(1UL << 38);
            return;
        }
        goto inactive;
    }
    return;
inactive:
    if (func_001385A8(obj->x0->x40C0) > 0.5f) {
set_flag:
        obj->x440 |= 1UL << 38;
    }
}
