/* UNIT 0x001AC140 0x001AC150 */
typedef struct {
    int x0;
    int x4;
} Obj;

void func_001AC140(Obj *obj, int a, int b)
{
    obj->x0 = a;
    obj->x4 = b;
}
