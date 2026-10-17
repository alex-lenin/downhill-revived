/* UNIT 0x002066F0 0x002067D8 */
typedef struct Info {
    unsigned int pad0 : 7;
    unsigned int x0_7 : 11;
    unsigned int x0_18 : 14;
    char pad4[0xC];
    short index;
} Info;

typedef struct Slot {
    unsigned int x0_0 : 7;
    unsigned int x0_7 : 11;
    unsigned int x0_18 : 14;
    Info *info;
    char pad8[0xC];
    int x14;
    float x18;
    char pad1C[0xC];
    unsigned char x28;
    char pad29[0x13];
} Slot;

typedef struct Owner {
    char pad0[0x10];
    Slot *slots;
} Owner;

typedef struct Source {
    char pad0[0x4];
    float x4;
    char pad8[0x8];
    short index;
} Source;

typedef struct Handle {
    Owner *owner;
    Slot *slot;
    int x8;
    short index;
    short xE;
} Handle;

extern int D_0029E160;
extern void func_00206320(Handle *);

void func_002066F0(Source *src, Owner *owner, int index) {
    Slot *slot;
    Handle h;

    if (src) {
        src->index = index;
        slot = &owner->slots[index];
        slot->x0_18 = slot->info->x0_18;
        slot->x0_7 = slot->info->x0_7;
        if (slot->x28) {
            h.owner = owner;
            h.x8 = 0;
            h.slot = slot;
            h.index = slot->info->index;
            slot->x14 = D_0029E160;
            slot->x18 = src->x4;
            func_00206320(&h);
        } else {
            h.owner = owner;
            h.x8 = 0;
            h.slot = slot;
            h.index = slot->info->index;
            slot->x14 = 0;
            slot->x18 = src->x4;
        }
    }
}
