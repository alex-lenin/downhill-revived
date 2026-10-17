/* UNIT 0x001985C0 0x00198648 */
typedef struct Item {
    char pad0[0xC];
    short xC;
    short xE;
} Item;

typedef struct Obj {
    char pad0[0x10];
    Item x10[1];
} Obj;

extern char D_003F52D0[], D_004348F0[], D_00438750[];
extern void func_00190688(void *);
extern void func_001962F0(void *);
extern void func_001990C0(void *);
extern void func_00198680(Obj *);

static inline int IsType(Item *item, int type) {
    return item->xC == type;
}

void func_001985C0(Obj *obj, int index) {
    if (IsType(&obj->x10[index], 0x5B)) {
        func_00198680(obj);
    } else if (IsType(&obj->x10[index], 0x5C)) {
        func_001990C0(D_00438750);
    } else if (IsType(&obj->x10[index], 0x5D)) {
        func_001962F0(D_004348F0);
    }
    func_00190688(D_003F52D0);
}
