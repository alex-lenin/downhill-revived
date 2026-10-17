/* UNIT 0x00164A50 0x00164A78 */
typedef struct Obj {
    char pad0[0x8340];
    int x8340[30];
    int x83B8;
} Obj;

void func_00164A50(Obj *obj, int value)
{
    obj->x83B8--;
    obj->x8340[obj->x83B8] = value;
}
