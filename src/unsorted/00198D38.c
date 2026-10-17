/* UNIT 0x00198D38 0x00198DD0 */
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

typedef struct Mgr {
    char pad0[0x35C];
    char x35C;
    char pad35D[0x17];
    char x374[1];
} Mgr;

extern Ids D_00437340;
extern Mgr D_004B25A0;
extern char D_003F53FD[];
extern char D_003F52D0[];

extern void func_001B8B10(Mgr *mgr);
extern void func_001C5B60(void *p);
extern void func_00190688(void *p);

static inline int IsType(Entry *entry, int type)
{
    return entry->xC == type;
}

void func_00198D38(Obj *obj, int index)
{
    if (IsType(&obj->entries[index], 0xA4)) {
        D_00437340.x500 = -1;
        D_00437340.x508 = -1;
        D_00437340.x504 = -1;
        func_001B8B10(&D_004B25A0);
        D_003F53FD[0] = 1;
        D_004B25A0.x35C = 1;
        func_001C5B60(D_004B25A0.x374);
    } else if (IsType(&obj->entries[index], 0x13)) {
        func_00190688(D_003F52D0);
    }
}
