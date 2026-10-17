/* UNIT 0x001BE158 0x001BE168 */
typedef struct Obj {
    char pad0[0x1FC];
    int x1FC;
    int x200;
} Obj;

void func_001BE158(Obj *obj, int value)
{
    obj->x200 = value;
    obj->x1FC = 1;
}
