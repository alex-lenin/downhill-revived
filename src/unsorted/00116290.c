/* UNIT 0x00116290 0x001162D0 */
typedef struct State {
    char pad0[0x796C];
    int x796C;
} State;
typedef struct Obj {
    char pad0[0x58];
    State *x58;
    char pad5C[0x70];
    int flags;
} Obj;
void func_00116290(Obj *obj) {
    int state = obj->x58->x796C;
    if (state == 0xA0 || state == 0x160) {
        obj->flags |= 2;
    } else {
        obj->flags &= ~2;
    }
}
