/* UNIT 0x001BCC50 0x001BCCA0 */
struct Obj { int x0, x4, x8, xC, x10; };
extern void func_001BCCA0(struct Obj *, int);
extern int func_00238F48(void);
void func_001BCC50(struct Obj *obj, int value, int arg) {
    int result;
    obj->x4 = value;
    obj->x0 = -1;
    func_001BCCA0(obj, arg);
    result = func_00238F48();
    obj->x8 = -1;
    obj->x10 = result;
}
