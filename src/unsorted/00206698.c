/* UNIT 0x00206698 0x002066F0 */
struct Target { char pad0[0x1C]; int flags; int x20; void *x24; };
struct Obj { struct Target *x0; int x4; };
extern void func_00206AD8(void *);
void func_00206698(struct Obj *obj) {
    struct Target *target = obj->x0;
    if (target && obj->x4 && !(target->flags & 1)) {
        void *child = target->x24;
        if (child) func_00206AD8(child);
    }
}
