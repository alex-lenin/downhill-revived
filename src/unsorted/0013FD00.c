/* UNIT 0x0013FD00 0x0013FDE8 */
typedef struct Entry {
    char pad0[6];
    unsigned char x6;
    char pad7[9];
} Entry;

typedef struct Obj {
    char pad0[0x18];
    Entry entries[64];
    int count;
} Obj;

/* x18/x1E and x8E overlap entries[0] and entries[7] */

typedef struct Mode {
    int type;
    int x4;
    int x8;
} Mode;

typedef struct Global {
    char pad0[0x574];
    Mode *modes;
} Global;

extern Global D_00365800;
extern int func_0013A258(void);

static __inline__ void Entry_SetFlag(Entry *entry, int value)
{
    entry->x6 = value;
}

static __inline__ Mode *GetMode(int index)
{
    return &D_00365800.modes[index];
}

void func_0013FD00(Obj *obj)
{
    int count;
    int i;
    int type;

    count = obj->count;
    for (i = 0; i < count; i++) {
        Entry_SetFlag(&obj->entries[i], 1);
    }
    type = GetMode(func_0013A258())->type;
    if ((type >= 2 && type <= 4) || type == 0x22) {
        obj->entries[0].x6 = 0;
        obj->entries[7].x6 = 0;
    } else if ((type >= 0x23 && type <= 0x25) || type == 0x21) {
        for (i = 0; i < count; i++) {
            Entry_SetFlag(&obj->entries[i], 0);
        }
    }
}
