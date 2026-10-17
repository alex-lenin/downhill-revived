/* UNIT 0x001EAC40 0x001EAC98 */
struct Entry {
    char pad0[0x50];
};

struct Obj {
    struct Entry entries[3];
    char xF0;
    char xF1;
    char xF2;
    char padF3[0x150 - 0xF3];
    int x150;
};

extern void func_001EAEF8(struct Entry *entry);

void func_001EAC40(struct Obj *obj)
{
    int i;

    obj->xF0 = 0;
    obj->xF1 = 0;
    obj->xF2 = 0;
    obj->x150 = 0;
    for (i = 0; i < 3; i++) {
        func_001EAEF8(&obj->entries[i]);
    }
}
