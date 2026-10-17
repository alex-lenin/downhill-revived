/* UNIT 0x00148520 0x00148560 */
struct Obj { char pad0[0xA0]; char xA0[0x22C]; float x2CC; };
extern void func_0014D798(struct Obj *, void *, float *);
extern void func_0014D740(struct Obj *);
void func_00148520(struct Obj *obj) {
    float value;
    func_0014D798(obj, obj->xA0, &value);
    obj->x2CC = value;
    func_0014D740(obj);
}
