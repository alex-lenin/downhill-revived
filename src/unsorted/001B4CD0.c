/* UNIT 0x001B4CD0 0x001B4DD0 */
typedef struct Entry {
    void *x0;
    void *x4;
    int x8;
    int xC;
    int x10;
} Entry;

typedef struct Slot {
    char pad0[0x150];
    char x150[0x1380 - 0x150];
} Slot;

typedef struct Obj {
    char pad0[0xC];
    int xC;
    int x10[4];
    int x20;
    Entry x24;
    int x38;
    int x3C;
    char pad40[0x56 - 0x40];
    unsigned char x56;
} Obj;

extern Entry D_0075A9D0[];
extern Slot D_003B2270[];
extern void func_0023C8D8(int, int);
extern void func_0023D020(int, int);
extern void func_0023D610(int);
extern void func_00176E40(void *, int);

static inline void SetSlot(int index, int value)
{
    if (index >= 0 && index < 4) {
        Slot *slot = &D_003B2270[index];
        func_00176E40(slot->x150, value);
    }
}

void func_001B4CD0(Obj *obj)
{
    int count;
    int i;

    count = obj->x3C;
    if (obj->x56 == 0) {
        return;
    }
    if (count >= 2) {
        D_0075A9D0[obj->x20] = obj->x24;
        func_0023C8D8(obj->x38, obj->x20);
        if (obj->xC != 0) {
            func_0023D020(0, obj->xC);
        }
    }
    for (i = 0; i < count; i++) {
        SetSlot(i, obj->x10[i]);
    }
    func_0023D610(obj->x3C);
}
