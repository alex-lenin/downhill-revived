/* UNIT 0x001AAAE8 0x001AAB18 */
typedef struct Obj {
    char pad0[0x18];
    int *x18;
    int x1C;
    int x20;
    int x24;
} Obj;

void func_001AAAE8(Obj *obj)
{
    int *p = obj->x18;

    if (p != 0) {
        *p = (*p & ~0x3F) | 0x2C;
    }
    obj->x20 = 0;
    obj->x18 = 0;
    obj->x1C = 0;
    obj->x24 = 0;
}
