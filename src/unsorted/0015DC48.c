/* UNIT 0x0015DC48 0x0015DCD8 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Entry {
    char pad0[0xC];
    short xC;
    char padE[2];
} Entry;

typedef struct Obj {
    char pad0[0x10];
    Entry entries[72];
    u128 x490;
} Obj;

extern char D_004B2A90[];
extern char D_003F52D0[];
extern void func_001C57A8(void *, u128);
extern void func_0015C990(void);
extern void func_00190688(void *);

static __inline__ int IsType(Entry *e, int t) { return e->xC == t; }

void func_0015DC48(Obj *obj, int index) {
    if (IsType(&obj->entries[index], 0xA4)) {
        obj->x490 = 0;
        func_001C57A8(D_004B2A90, (u128)0x5443454C45535F53UL);
        func_0015C990();
    } else if (IsType(&obj->entries[index], 0x13)) {
        func_00190688(D_003F52D0);
    }
}
