/* UNIT 0x0022FD20 0x0022FD38 */
struct Obj { char pad0[0xC]; float xC; char pad10[0xC]; float x1C; };
void func_0022FD20(struct Obj *obj, float x, float y) {
    if (obj) {
        obj->xC = x;
        obj->x1C = y;
    }
}
