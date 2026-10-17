/* UNIT 0x0011F880 0x0011F8C0 */
struct Obj { char pad[0x250]; float x250; };
extern void func_0011E568(struct Obj *, int);
void func_0011F880(struct Obj *obj) {
    if (obj->x250 < 22500.0f) func_0011E568(obj, 5);
}
