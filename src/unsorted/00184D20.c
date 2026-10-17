/* UNIT 0x00184D20 0x00184D48 */
struct Obj {
    int x0;
    void *vtbl;
    int x8;
    int xC;
    int x10;
};

extern char D_00286150[];

struct Obj *func_00184D20(struct Obj *obj)
{
    obj->x0 = 0;
    obj->vtbl = D_00286150;
    obj->x8 = 0;
    obj->xC = 0;
    obj->x10 = 0;
    return obj;
}
