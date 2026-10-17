/* UNIT 0x00182990 0x001829C0 */
struct Obj { int x0; int x4; };
extern int D_00381DE0[];
void func_00182990(struct Obj *obj, int value, int kind) {
    if (kind == 0xC1C) {
        obj->x0 = value;
    } else if (kind == 0xC1E) {
        D_00381DE0[0] = value;
        obj->x4 = value;
    }
}
