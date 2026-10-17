/* UNIT 0x00206A40 0x00206A70 */
typedef struct Obj {
    int flags;
    int x4, x8, xC, x10, x14;
} Obj;
Obj *func_00206A40(Obj *obj, int value) {
    obj->x4 = value;
    obj->x8 = 0;
    obj->xC = 0;
    obj->flags = (obj->flags & ~0x3F) | 0x24;
    obj->x10 = 0;
    obj->x14 = 0;
    return obj;
}

