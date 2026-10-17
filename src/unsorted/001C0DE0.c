/* UNIT 0x001C0DE0 0x001C0E00 */
typedef struct Obj {
    int x0;
    int x4;
    int x8;
    int xC;
    int x10;
} Obj;

void func_001C0DE0(Obj *obj, int a, int b)
{
    obj->x8 = a;
    obj->x4 = b;
    obj->x10 = 1;
    obj->xC = -1;
}
