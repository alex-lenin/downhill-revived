/* UNIT 0x0022FD38 0x0022FD50 */
struct Obj { char pad0[0x3C]; float x3C; };
void func_0022FD38(struct Obj *obj, float value) {
    if (obj) obj->x3C = value * value;
}
