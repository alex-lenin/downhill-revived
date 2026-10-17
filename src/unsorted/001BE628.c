/* UNIT 0x001BE628 0x001BE638 */
typedef struct Obj {
    int x0;
    int x4;
    int x8;
} Obj;

void func_001BE628(Obj *obj, int value)
{
    obj->x4 = value;
    obj->x8 = -1;
}
