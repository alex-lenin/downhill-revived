/* UNIT 0x0017BD60 0x0017BD80 */
typedef struct Obj {
    char pad0[0xF5C];
    int *xF5C;
} Obj;

void func_0017BD60(Obj *obj, int *item)
{
    if ((*item & 0x3F) == 1) {
        obj->xF5C = item;
    }
}
