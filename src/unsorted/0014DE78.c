/* UNIT 0x0014DE78 0x0014DE90 */
typedef struct Obj {
    int x0;
    int x4;
    int x8;
    int xC;
    int x10;
} Obj;

void func_0014DE78(Obj *obj, int a, int b, int size)
{
    obj->x0 = a;
    obj->x4 = b;
    obj->x10 = (size >> 5) - 1;
}
