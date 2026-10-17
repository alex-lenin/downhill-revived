/* UNIT 0x002063F0 0x00206440 */
typedef struct Vec3 {
    int x, y, z;
} Vec3;

typedef struct Entry {
    char pad0[0x8];
    Vec3 x8;
    char pad14[0x28];
} Entry;

typedef struct Source {
    char pad0[0x4];
    Vec3 x4;
    Entry *x10;
} Source;

typedef struct Ref {
    char pad0[0x10];
    unsigned short x10;
} Ref;

extern void func_00204A98(Entry *entry, Source *src);

void func_002063F0(Ref *ref, Source *src)
{
    Entry *entry = &src->x10[ref->x10];

    entry->x8 = src->x4;
    func_00204A98(entry, src);
}
