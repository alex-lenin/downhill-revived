/* UNIT 0x0022FD70 0x0022FD90 */
struct Obj { char pad[0x20]; float x20, x24, x28; };
void func_0022FD70(struct Obj *obj, float x, float y, float z) {
    if (obj) { obj->x20 = x; obj->x24 = y; obj->x28 = z; }
}
