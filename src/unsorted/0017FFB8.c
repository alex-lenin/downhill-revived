/* UNIT 0x0017FFB8 0x00180000 */
struct Obj { int x0; int x4; char pad8[0x19c]; float x1A4; char pad1A8[8]; int x1B0; };
extern void func_00181618(struct Obj *, float);
void func_0017FFB8(struct Obj *obj, int state) {
    obj->x1B0 = 0;
    if (state == 2) {
        func_00181618(obj, obj->x1A4);
    }
    obj->x4 = state;
}
