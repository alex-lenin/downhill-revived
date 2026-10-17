/* UNIT 0x00164A10 0x00164A50 */
typedef struct Obj {
    char pad0[0x8340];
    int entries[30];
    int count;
} Obj;

int func_00164A10(Obj *obj) {
    int value;

    if (obj->count >= 30) {
        return 0;
    }
    value = obj->entries[obj->count];
    obj->entries[obj->count] = 0;
    obj->count++;
    return value;
}
