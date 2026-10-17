/* UNIT 0x0010CAF0 0x0010CB70 */
struct Obj { char pad0[0x7c]; int x7c; char pad80[0x3c0]; unsigned long x440; };
extern void func_0010BD10(struct Obj *, int);
extern void func_0010E6C0(struct Obj *);
extern void func_0010E420(struct Obj *, int);
extern void func_0010F068(struct Obj *, int);
extern void func_0010CB70(struct Obj *, int);
void func_0010CAF0(struct Obj *obj, int arg) {
    func_0010BD10(obj, arg);
    if (obj->x7c) func_0010E6C0(obj);
    if (obj->x440 & ((unsigned long)0x8000 << 25)) func_0010E420(obj, arg);
    else func_0010F068(obj, arg);
    func_0010CB70(obj, arg);
}
