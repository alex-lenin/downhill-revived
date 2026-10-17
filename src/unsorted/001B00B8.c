/* UNIT 0x001B00B8 0x001B00C8 */
typedef struct Obj {
    int x0;
    char pad4[0x5C];
    int x60;
    char pad64[0x7C];
    int xE0;
} Obj;

void func_001B00B8(Obj *obj)
{
    obj->xE0 = 0;
    obj->x0 = 0;
    obj->x60 = 0;
}
