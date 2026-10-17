/* UNIT 0x00115F20 0x00115FA8 */
struct Rider { char pad[0x796C]; int x796C; };
struct Obj { char pad[0x58]; struct Rider *x58; char pad5C[8]; float x64; };
void func_00115F20(struct Obj *obj) {
    obj->x64 = 0.0f;
    switch (obj->x58->x796C) {
    case 0x40: obj->x64 = 0.5f; break;
    case 0xE0: case 0x100: case 0x140: obj->x64 = 0.2f; break;
    default: obj->x64 = 0.33f; break;
    }
}
