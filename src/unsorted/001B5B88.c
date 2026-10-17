/* UNIT 0x001B5B88 0x001B5BA8 */
typedef struct Obj {
    char pad0[0x58];
    int *x58;
} Obj;

int func_001B5B88(Obj *obj)
{
    if (obj->x58 != 0) {
        return *obj->x58;
    }
    return 0;
}
