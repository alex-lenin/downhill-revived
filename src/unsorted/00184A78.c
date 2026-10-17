/* UNIT 0x00184A78 0x00184AA0 */
struct Obj {
    int x0;
    void *vtbl;
    int x8;
    int xC;
    int x10;
};

extern char D_00286188[];

struct Obj *func_00184A78(struct Obj *obj)
{
    obj->x0 = 0;
    obj->vtbl = D_00286188;
    obj->x8 = 0;
    obj->xC = 0;
    obj->x10 = 0;
    return obj;
}
