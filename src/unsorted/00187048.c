/* UNIT 0x00187048 0x00187068 */
typedef struct Obj {
    char pad0[0x64];
    int x64[2];
    int count;
} Obj;

void func_00187048(Obj *obj, int value)
{
    obj->x64[obj->count] = value;
    obj->count++;
}
