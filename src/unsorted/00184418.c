/* UNIT 0x00184418 0x00184470 */
struct Obj { int x0; char pad4[0xc]; int x10; int x14; };
extern int func_00238F48(void);
int func_00184418(struct Obj *obj) {
    int elapsed, result;
    if (obj->x0 != 0) {
        elapsed = func_00238F48();
        result = (elapsed - obj->x10) > obj->x14 / 16;
    } else {
        result = 0;
    }
    return result;
}
