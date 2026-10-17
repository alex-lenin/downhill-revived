/* UNIT 0x001ABBE8 0x001ABC28 */
typedef struct Obj { char pad0[0x8098]; void *x8098; } Obj;
extern char D_00287EC0[];
extern void func_00230708(Obj *, int);
Obj *func_001ABBE8(Obj *obj, int value) {
    func_00230708(obj, value);
    obj->x8098 = D_00287EC0;
    return obj;
}
