/* UNIT 0x001852A8 0x001852D0 */
struct Obj {
    int x0;
    void *vtbl;
    char pad8[0xC8C - 0x8];
    int xC8C;
    int xC90;
    char xC94;
};

extern char D_002860E0[];

struct Obj *func_001852A8(struct Obj *obj, int arg)
{
    obj->x0 = 0;
    obj->vtbl = D_002860E0;
    obj->xC8C = arg;
    obj->xC90 = 0;
    obj->xC94 = 0;
    return obj;
}
