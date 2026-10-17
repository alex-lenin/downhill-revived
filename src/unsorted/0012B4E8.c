/* UNIT 0x0012B4E8 0x0012B528 .sdata=0x29C268:0x29C270 */
struct Obj { char pad[0x198]; int x198; };
float D_0029C268[2] = {30.0f, 3.402823466e+38f};
extern void func_0012F328(int, int, float, float);
void func_0012B4E8(struct Obj *obj, int arg1, int arg2) {
    func_0012F328(arg1, arg2, D_0029C268[0], 1.0f);
    obj->x198 = 1;
}
