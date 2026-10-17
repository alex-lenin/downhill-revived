/* UNIT 0x001BEE08 0x001BEE28 */
typedef struct Obj {
    int x0;
    int x4;
    int x8;
    int xC;
    int x10;
} Obj;

void func_001BEE08(Obj *obj, int value)
{
    obj->x10 = 1;
    obj->xC = -1;
    obj->x8 = -1;
    obj->x4 = value;
}
