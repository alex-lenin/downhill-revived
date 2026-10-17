/* UNIT 0x00134AA8 0x00134AC0 */
typedef struct Obj {
    char pad0[0x20];
    void *x20;
} Obj;

extern char D_00283B40[];

Obj *func_00134AA8(Obj *obj)
{
    obj->x20 = D_00283B40;
    return obj;
}
