/* UNIT 0x00197FE0 0x001980B0 */
typedef struct Entry {
    char pad0[0xC];
    short xC;
    char padE[0x2];
} Entry;

typedef struct Obj {
    char pad0[0x10];
    Entry entries[1];
} Obj;

typedef struct Ids {
    char pad0[0x500];
    int x500;
    int x504;
    int x508;
} Ids;

extern Ids D_00437340;
extern char D_004B28FC[];
extern char D_003F53FD[];
extern char D_003F52D0[];

extern void func_002182A8(int handle, int arg);
extern void func_001980F0(Ids *obj);
extern void func_00198140(Ids *obj);
extern void func_00190688(void *p);

static inline int IsType(Entry *entry, int type)
{
    return entry->xC == type;
}

void func_00197FE0(Ids *ids, int index)
{
    Obj *obj = (Obj *)ids;

    if (ids->x504 != 0) {
        func_002182A8(ids->x504, 1);
        if (IsType(&obj->entries[index], 0x56)) {
            func_001980F0(ids);
        } else if (IsType(&obj->entries[index], 0x57)) {
            func_00198140(ids);
            D_00437340.x500 = -1;
            D_00437340.x508 = -1;
            D_00437340.x504 = -1;
            D_004B28FC[0] = 1;
            D_003F53FD[0] = 1;
        } else if (IsType(&obj->entries[index], 0x1A)) {
            func_002182A8(ids->x504, 1);
            func_00190688(D_003F52D0);
        }
    }
}
