/* UNIT 0x00208160 0x002081B8 */
struct Obj { char pad0[0x20]; int x20, x24, x28; };
extern int func_00202760(int, int, int, void *);
extern int func_0020E808(int);
extern int D_004D02F0[];
extern int D_0029E168;
void func_00208160(struct Obj *obj) {
    if (func_00202760(D_004D02F0[0], func_0020E808(obj->x24), obj->x28, &D_0029E168)) obj->x20 = 4;
}


