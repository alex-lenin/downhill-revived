/* UNIT 0x0022FC78 0x0022FC98 */
struct Obj { char pad[0x30]; float x30, x34, x38; };
void func_0022FC78(struct Obj *obj, float x, float y, float z) {
    if (obj) { obj->x30 = x; obj->x34 = y; obj->x38 = z; }
}
