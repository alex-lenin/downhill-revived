/* UNIT 0x001BC480 0x001BC4C0 */
typedef struct Entry {
    int x0;
    char pad4[0x80];
} Entry;

typedef struct Obj {
    char pad0[0x644];
    Entry x644[4];
    char pad854[0xC];
    int x860;
} Obj;

int func_001BC480(Obj *obj, int index) {
    int id;
    int prev = obj->x860++;
    if (prev == 0) {
        obj->x860++;
    }
    id = obj->x860;
    obj->x644[index].x0 = id;
    return id;
}
