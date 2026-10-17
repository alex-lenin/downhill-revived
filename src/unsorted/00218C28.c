/* UNIT 0x00218C28 0x00218D88 */
typedef struct EntryBits {
    unsigned int index : 14;
    unsigned int pad : 6;
    unsigned int bank : 5;
    unsigned int count : 3;
    unsigned int pad2 : 3;
    unsigned int active : 1;
} EntryBits;

typedef struct Entry {
    int x0;
    union {
        EntryBits b;
        unsigned int word;
    } x4;
    int slot;
    int xC;
} Entry;

typedef struct Slot {
    unsigned int stamp;
    unsigned short id;
    unsigned short value;
} Slot;

typedef struct Bank {
    unsigned short count;
    char pad2[6];
} Bank;

extern Entry D_00621310[];
extern Slot D_00625190[][300];
extern int D_0062E850[];
extern int D_0062E890[];
extern Bank *D_0029E214;
extern unsigned int D_0029E218;

/* Picks a slot in the entry's bank: the first empty one, else the least recently used. */
unsigned short func_00218C28(int id)
{
    int bank;
    unsigned short count;
    int i;
    int best;
    unsigned int max;
    unsigned int age;
    unsigned int stamp;
    Slot *slot;

    bank = D_00621310[id].x4.b.bank;
    count = D_0029E214[bank].count;
    D_0062E850[bank]++;
    D_0062E890[id]++;
    max = 0;
    i = 0;
    best = 0;
    while (i < count) {
        stamp = D_00625190[bank][i].stamp;
        if (stamp == 0) {
            best = i;
            break;
        }
        age = D_0029E218 - stamp;
        if (age >= max) {
            max = age;
            best = i;
        }
        i++;
    }
    slot = (Slot *)(best * 8 + bank * 0x960 + (char *)D_00625190);
    if (slot->id) {
        D_00621310[slot->id].x4.b.active = 0;
    }
    D_00621310[id].slot = best;
    slot->stamp = D_0029E218;
    slot->id = id;
    return slot->value;
}
