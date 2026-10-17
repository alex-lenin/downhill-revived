/* UNIT 0x00207B40 0x00207D90 */
struct Ref { short x0; short x2; short x4; };
struct Resource {
    unsigned short count;
    char pad2[3];
    unsigned char refCount;
    char pad6[2];
    struct Ref *refs;
    int xC;
    int slots[1];
};
struct Item { char pad0[0x20]; struct Resource *res; };
struct List { int x0; int count; struct Item *items[1]; };
struct Manager { char pad0[0x1680]; int count; };
struct Entry { char name[0x20]; int x20; };
struct Registry { int count; int x4; struct Entry entries[1]; };

extern struct Manager D_004D0380;
extern struct List *D_004D0108[];
extern struct Registry D_002794A8;

extern int strcmp(const char *, const char *);
extern int func_00207A10(char *);
extern void func_0020D6F8(struct Manager *, char *, int);
extern void func_002078C0(char *, char *, int *, int);
extern void func_00254544(char *, char *);
extern void func_00207740(int, struct Resource *);

void func_00207B40(void)
{
    char buffer[0x20];
    int next;
    struct Item **nextItem;
    int nextN;
    int i, n;
    struct List *list;
    struct Item **item;
    struct Resource *res;
    int s, empty, k, j, found, known;
    struct Ref *ref;
    struct Entry *entry;
    char *name;

    for (i = 0; i < D_004D0380.count; i = next) {
        next = i + 1;
        list = D_004D0108[i];
        if (list == 0)
            continue;
        n = list->count;
        item = list->items;
        if (n == 0)
            continue;
        do {
            nextN = n - 1;
            nextItem = item + 1;
            res = (*item)->res;
            empty = 0;
            for (s = 0; s < res->count; s++) {
                if (res->slots[s] == 0)
                    empty = 1;
            }
            if (empty != 0) {
                for (k = 0; k < res->refCount; k++) {
                    found = -1;
                    ref = &res->refs[k];
                    name = (char *)ref + ref->x2;
                    for (j = 0; j < D_002794A8.count; j++) {
                        if (strcmp(D_002794A8.entries[j].name, name) == 0)
                            found = j;
                    }
                    known = func_00207A10(name);
                    if (found < 0 && known == 0) {
                        entry = &D_002794A8.entries[D_002794A8.count];
                        func_0020D6F8(&D_004D0380, buffer, i);
                        func_002078C0(name, buffer, &entry->x20, D_002794A8.x4);
                        func_00254544(entry->name, name);
                        /* written out (not count++): keeps the loop small enough for
                           loop.c to hoist the k*6 constant, as retail did */
                        found = D_002794A8.count;
                        D_002794A8.count = found + 1;
                    }
                    if (known == 0)
                        func_00207740(D_002794A8.entries[found].x20, res);
                }
            }
            n = nextN;
            item = nextItem;
        } while (n != 0);
    }
}
