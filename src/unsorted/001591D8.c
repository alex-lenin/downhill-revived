/* UNIT 0x001591D8 0x00159220 */
struct Obj { int x0; unsigned char x4; };
extern struct Obj *func_00208FB8(int);
extern void func_00218278(struct Obj *, int);
struct Obj *func_001591D8(int arg0) {
    struct Obj *obj = func_00208FB8(arg0);
    obj->x4 = 1;
    func_00218278(obj, arg0);
    return obj;
}
