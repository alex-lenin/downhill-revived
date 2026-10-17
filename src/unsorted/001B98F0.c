/* UNIT 0x001B98F0 0x001B9910 */
typedef struct Entry {
    int x0;
    int x4;
    int x8;
} Entry;

typedef struct Obj {
    char pad0[0x14C];
    Entry entries[1];
} Obj;

void func_001B98F0(Obj *obj, int index, int a, int b, int c)
{
    Entry *e = &obj->entries[index];
    e->x0 = a;
    e->x4 = b;
    e->x8 = c;
}
