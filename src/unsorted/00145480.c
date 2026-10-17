/* UNIT 0x00145480 0x001454D8 */
struct Obj { int x0, x4, x8, xC; };
extern char D_002AF2C0[];
extern int func_0011C0D0(void *, int, int, float);
extern void func_00144B88(int, int);
void func_00145480(struct Obj *obj) {
    if (func_0011C0D0(D_002AF2C0, obj->x8 + 0x10, 0, 200.0f))
        func_00144B88(obj->xC, 1);
}
