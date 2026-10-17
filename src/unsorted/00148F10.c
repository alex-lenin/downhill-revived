/* UNIT 0x00148F10 0x00148F60 */
struct Obj { char pad[0x2B0]; int x2B0; int x2B4; int x2B8; int x2BC; };
extern void func_00148F60(struct Obj *);
extern void func_00149988(struct Obj *);
void func_00148F10(struct Obj *obj) {
    if (obj->x2B0 && obj->x2B4) {
        if (!obj->x2BC) func_00148F60(obj);
        else func_00149988(obj);
    }
}
