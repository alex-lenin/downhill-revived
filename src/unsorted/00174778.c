/* UNIT 0x00174778 0x00174790 */
/* D_00285B90 is a C++ vtable (gcc 2.95 layout, 8-byte entries). */
extern int D_00285B90[];

struct Obj {
    int x0;
    int *vtbl;
};

struct Obj *func_00174778(struct Obj *obj)
{
    obj->x0 = 0;
    obj->vtbl = D_00285B90;
    return obj;
}
