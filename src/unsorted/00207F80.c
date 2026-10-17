/* UNIT 0x00207F80 0x00207FA0 */
typedef struct {
    char x0;
    char pad1[0x1F];
    int x20;
    int x24;
    int x28;
    int x2C;
} Obj;

Obj *func_00207F80(Obj *obj)
{
    obj->x0 = 0;
    obj->x20 = 0;
    obj->x24 = 0;
    obj->x28 = 0;
    obj->x2C = 0;
    return obj;
}
