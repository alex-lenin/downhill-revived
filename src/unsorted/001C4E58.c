/* UNIT 0x001C4E58 0x001C4E68 */
typedef struct Obj {
    char pad0[0x18];
    int x18;
} Obj;

Obj *func_001C4E58(Obj *obj)
{
    obj->x18 = 0;
    return obj;
}
