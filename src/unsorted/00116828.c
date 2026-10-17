/* UNIT 0x00116828 0x00116868 */
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
void func_00116828(Obj *obj) {
    int state = obj->x58->x796C;
    if (state == 0x80 || state == 0x120) {
        obj->flags |= 4;
    } else {
        obj->flags &= ~4;
    }
}

