/* UNIT 0x0018E4F8 0x0018E518 */
typedef struct Obj {
    char pad0[0x424];
    int x424[4];
    int count;
} Obj;

void func_0018E4F8(Obj *obj, int value)
{
    obj->x424[obj->count] = value;
    obj->count++;
}
