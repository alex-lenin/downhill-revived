/* UNIT 0x00207740 0x002078C0 */
typedef struct Fixup {
    short index;
    int offset;
} Fixup;

typedef struct FixupTable {
    short x0;
    short count;
    Fixup fixups[1];
} FixupTable;

typedef struct Slot {
    int x0;
    void *data;
    char pad8[0x3C - 0x8];
} Slot;

typedef struct Target {
    char pad0[0x10];
    void *slots[1];
} Target;

typedef struct Obj {
    char pad0[0x10];
    Slot *slots;
    char pad14[0x20 - 0x14];
    Target *target;
} Obj;

typedef struct List {
    int x0;
    unsigned int count;
    Obj *entries[1];
} List;

typedef struct Manager {
    char pad0[0x1680];
    int numLists;
} Manager;

extern List *D_004D0108[];
extern Manager D_004D0380;

extern void func_002066F0(void *data, Obj *obj, int index);

void func_00207740(FixupTable *table, Target *target)
{
    int i;
    unsigned int j;
    int n;
    Fixup *fixup;
    List *list;
    Obj *obj;
    void *data;
    int index;
    int count;
    Fixup *f;
    void **slots;
    void *value;

    slots = target->slots;
    f = table->fixups;
    count = table->count;
    while (count != 0) {
        count--;
        value = (char *)table + f->offset;
        slots[f->index] = value;
        f++;
    }
    for (i = 0; i < D_004D0380.numLists; i++) {
        list = D_004D0108[i];
        if (list == 0) {
            continue;
        }
        for (j = 0; j < list->count; j++) {
            obj = list->entries[j];
            if (obj->target != target) {
                continue;
            }
            n = table->count;
            fixup = table->fixups;
            while (n != 0) {
                n--;
                index = fixup->index;
                data = (char *)table + fixup->offset;
                obj->slots[index].data = data;
                func_002066F0(data, obj, index);
                fixup++;
            }
        }
    }
}
