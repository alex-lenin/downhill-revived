/* UNIT 0x0022FD50 0x0022FD70 */
struct Obj { char pad[0x2C]; float x2C; char pad30[0xC]; float x3C; };
void func_0022FD50(struct Obj *obj, float radius) {
    if (obj) { obj->x2C = radius; obj->x3C = radius * radius; }
}
