/* UNIT 0x00163D80 0x00163DA0 */
typedef struct Inner { int x0; float x4; int x8; } Inner;
typedef struct Obj { char pad0[0x10]; Inner x10; } Obj;
void func_00163D80(Obj *obj) {
    Inner *inner = &obj->x10;
    inner->x0 = 0;
    inner->x4 = 10.0f;
    inner->x8 = 0;
}
