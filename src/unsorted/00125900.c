/* UNIT 0x00125900 0x00125928 */
typedef struct Obj {
    char pad0[0x8];
    unsigned int x8;
} Obj;

int func_00125900(Obj *obj)
{
    unsigned int v = obj->x8;
    if (v < 2 || v == 3) {
        return 0;
    }
    return 1;
}
