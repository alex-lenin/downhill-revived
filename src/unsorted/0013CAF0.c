/* UNIT 0x0013CAF0 0x0013CB00 */
struct Obj {
    char pad0[0x510];
    int x510;
    int x514;
    int x518;
};

void func_0013CAF0(struct Obj *obj, int a, int b, int c)
{
    obj->x510 = a;
    obj->x514 = b;
    obj->x518 = c;
}
