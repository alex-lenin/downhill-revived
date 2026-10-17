/* UNIT 0x0018EAC8 0x0018EAE8 */
typedef struct Obj {
    char pad0[0x500];
    int x500[12];
} Obj;

int func_0018EAC8(Obj *obj, unsigned int index)
{
    if (index < 12) {
        return obj->x500[index];
    }
    return 0;
}
