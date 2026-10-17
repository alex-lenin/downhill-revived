/* UNIT 0x00140130 0x001401B0 */
typedef unsigned int u128 __attribute__((mode(TI)));

struct VTable {
    char pad0[0x40];
    short delta;
    short pad42;
    void (*method)(void *);
};

struct Obj {
    char pad0[0x4F8];
    struct VTable *vtable;
};

extern char D_003F53FD[];
extern char D_004B2A90[];
extern void func_001C57A8(void *, u128);

void func_00140130(struct Obj *obj, int index) {
    struct VTable *v;
    int found;

    /* 16-byte entries, short type at +0x1C; the boolean temp gives retail's lh/xori/bnez */
    found = *(short *)((char *)obj + index * 16 + 0x1C) == 0x1A;
    if (found) {
        func_001C57A8(D_004B2A90, (u128)0x4B4341425F53ULL);
        v = obj->vtable;
        v->method((char *)obj + v->delta);
    } else {
        D_003F53FD[0] = 1;
    }
}
