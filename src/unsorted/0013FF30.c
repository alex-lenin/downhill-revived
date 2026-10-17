/* UNIT 0x0013FF30 0x0013FFC8 */
typedef unsigned int u128 __attribute__((mode(TI)));

struct Entry {
    char pad0[0x1c];
    short x1c;
};

struct VTable {
    char pad0[0x40];
    short delta;
    short pad42;
    void (*call)(void *);
};

struct Obj {
    char pad0[0x4f8];
    struct VTable *x4f8;
};

extern char D_004B2A90[];
extern char D_0036B730[];
extern char D_003F53FD[];
extern void func_001C57A8(void *, u128);
extern void func_0013A170(void);
extern void func_00140C38(void *);

void func_0013FF30(struct Obj *obj, int index)
{
    int kind = ((struct Entry *)(index * 16 + (int)obj))->x1c;

    if ((kind ^ 0x1a) == 0) {
        func_001C57A8(D_004B2A90, (u128)0x4b4341425f53);
        obj->x4f8->call((char *)obj + obj->x4f8->delta);
    } else {
        func_0013A170();
        D_003F53FD[0] = 1;
        func_00140C38(D_0036B730);
    }
}
