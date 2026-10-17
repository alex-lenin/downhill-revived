/* UNIT 0x00198F10 0x00198F70 */
typedef struct Entry {
    char pad0[0xC];
    short xC;
    char padE[2];
} Entry;

typedef struct Obj {
    char pad0[0x10];
    Entry entries[1];
} Obj;

extern char D_003F52D0[];
extern char D_00388E90[];
extern void func_00190660(void *, void *);
extern void func_00190688(void *);

static __inline__ int IsType(Entry *e, int t) { return e->xC == t; }

void func_00198F10(Obj *obj, int index) {
    if (IsType(&obj->entries[index], 0x12)) {
        func_00190660(D_003F52D0, D_00388E90);
    } else if (IsType(&obj->entries[index], 0x13)) {
        func_00190688(D_003F52D0);
    }
}
