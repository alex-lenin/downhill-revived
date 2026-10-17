/* UNIT 0x00197DA0 0x00197E50 */
typedef struct Item {
    char pad0[0xC];
    short xC;
    short xE;
} Item;

typedef struct Obj {
    char pad0[0x10];
    Item x10[1];
} Obj;

extern char D_003F52D0[], D_00437D50[], D_00438250[], D_00435500[], D_00386680[];
extern void func_00190660(void *, void *);
extern void func_001973B0(void *);

static inline int IsType(Item *item, int type) {
    return item->xC == type;
}

void func_00197DA0(Obj *obj, int index) {
    if (IsType(&obj->x10[index], 0x22)) {
        func_00190660(D_003F52D0, D_00437D50);
    } else if (IsType(&obj->x10[index], 0x1A)) {
        func_00190660(D_003F52D0, D_00438250);
    } else if (IsType(&obj->x10[index], 0x59)) {
        func_001973B0(D_00435500);
    } else if (IsType(&obj->x10[index], 0x29)) {
        func_00190660(D_003F52D0, D_00386680);
    }
}
