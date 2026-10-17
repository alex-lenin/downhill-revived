/* UNIT 0x001162D0 0x00116368 */
struct Rider { char pad[0x796C]; int x796C; };
struct Obj { char pad[0x58]; struct Rider *x58; char pad5C[0xB4-0x5C]; int xB4; };
extern int func_00222BD8(int, int);
void func_001162D0(struct Obj *obj) {
    obj->xB4 = 0;
    switch (obj->x58->x796C) {
    case 0x60: case 0x1A0: obj->xB4 = 1200; break;
    case 0x20: case 0xC0: case 0x120: obj->xB4 = 3600; break;
    default: obj->xB4 = 36000; break;
    }
    obj->xB4 = func_00222BD8(obj->xB4 / 2, obj->xB4 * 2);
}
