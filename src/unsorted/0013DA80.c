/* UNIT 0x0013DA80 0x0013DB38 */
struct Obj {
    char pad0[0x4];
    int x4;
    char pad8[0x4];
    char xC;
    char padD;
    char xE;
    char padF[0x5];
    short x14;
    char pad16[0x2];
    char x18[0x10];
    char x28;
};

extern void func_0018D6F0(struct Obj *obj);
extern void func_00203590(void *p, int a, int b, int c);

void func_0013DA80(struct Obj *obj, int x4, int a, int b, int c, int x28, int xE,
                   unsigned char x14)
{
    func_0018D6F0(obj);
    obj->x4 = x4;
    obj->xE = xE;
    obj->x28 = x28;
    obj->x14 = x14;
    obj->xC = 1;
    if (a || b || c) {
        func_00203590(obj->x18, a, b, c);
    }
}
