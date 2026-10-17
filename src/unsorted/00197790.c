/* UNIT 0x00197790 0x00197810 */
struct Entry {
    char pad0[0xC];
    short type;
    char padE[2];
};

struct Obj {
    char pad0[0x10];
    struct Entry entries[1];
};

extern unsigned char D_004B28FC[];
extern unsigned char D_003F53FD[];
extern void func_00197840(void *);
extern void func_00197B68(void *);
extern void func_00197810(void *);

static inline int IsEntry(struct Entry *e, int type)
{
    return e->type == type;
}

static inline void HandleOther(struct Obj *obj, struct Entry *e)
{
    if (IsEntry(e, 0x54))
        func_00197810(obj);
}

void func_00197790(struct Obj *obj, int index)
{
    if (IsEntry(&obj->entries[index], 0x51)) {
        func_00197840(obj);
        D_004B28FC[0] = 1;
        D_003F53FD[0] = 1;
    } else if (IsEntry(&obj->entries[index], 0x52)) {
        func_00197B68(obj);
        D_004B28FC[0] = 1;
        D_003F53FD[0] = 1;
    } else {
        HandleOther(obj, &obj->entries[index]);
    }
}
