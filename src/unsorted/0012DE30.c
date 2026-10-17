/* UNIT 0x0012DE30 0x0012DE68 */
typedef struct Inner { char pad0[0x10]; int x10; } Inner;
typedef struct Obj { char pad0[0x794C]; Inner *x794C; int x7950; } Obj;
extern void func_00218498(Inner *, int);
void func_0012DE30(Obj *obj) {
    int value = obj->x7950;
    if (value != 0) {
        Inner *inner = obj->x794C;
        if (inner != 0) {
            inner->x10 = value;
            func_00218498(obj->x794C, 0);
        }
    }
}
