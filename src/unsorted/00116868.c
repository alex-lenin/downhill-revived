/* UNIT 0x00116868 0x001168A0 */
typedef struct Inner { char pad0[0x796C]; int x796C; } Inner;
typedef struct Obj { char pad0[0x58]; Inner *x58; char pad5C[0x70]; int xCC; } Obj;
void func_00116868(Obj *obj) {
    if (obj->x58->x796C == 0xC0) {
        obj->xCC |= 8;
    } else {
        obj->xCC &= ~8;
    }
}
