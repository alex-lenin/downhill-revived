/* UNIT 0x0022F570 0x0022F588 */
struct Obj {
    int x0;
    int x4;
    int x8;
    int xC;
};

void func_0022F570(struct Obj *obj, int a, int b)
{
    obj->x0 = a;
    obj->x8 = a;
    obj->x4 = b;
    obj->xC = 0;
}
