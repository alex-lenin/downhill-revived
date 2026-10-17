/* UNIT 0x00162F38 0x00162F88 */
struct Payload { int words[6]; };
struct Obj { int x0, x4; struct Payload x8; };
extern struct Payload D_00768498;
int func_00162F38(struct Obj *obj) {
    obj->x0 = 4;
    obj->x4 = 0;
    obj->x8 = D_00768498;
    return 0x20;
}
