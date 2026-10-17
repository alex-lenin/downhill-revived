/* UNIT 0x00184F98 0x00184FB8 */
extern char D_00286118[];

typedef struct Obj {
    int x0;
    void *x4;
    char pad8[0xC88 - 0x8];
    int xC88;
} Obj;

Obj *func_00184F98(Obj *obj)
{
    obj->x0 = 0;
    obj->x4 = D_00286118;
    obj->xC88 = 0;
    return obj;
}
