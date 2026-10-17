/* UNIT 0x0015ED18 0x0015F038 */
typedef unsigned int u128 __attribute__((mode(TI)));
struct Item {
    int x0;
    short x4;
    char p6[10];
};
struct Entry {
    char p0[12];
    short xC;
    short xE;
};
struct Menu {
    char p0[0x10];
    struct Entry entries[1];
    char p20[0x4e8];
    int x508;
};
struct Values {
    int x0, x4, x8;
    char pC[4];
    unsigned char x10[16];
    char p20[7];
    unsigned char x27;
};
struct Settings {
    char p0[0x11a60];
    struct Values values;
};
extern struct Settings D_004225A0;
extern int D_003F5400[], D_0039E82C[], D_0039E830[];
extern char D_004B2A90[];
extern long D_0029DE68;
extern int func_0018D860(struct Item *);
extern int func_002205A0(int);
extern int func_00220180(int);
extern void func_0015F4C8(struct Menu *, int, int, int);
extern void func_00201A30(int);
extern void func_001C57A8(void *, u128);
extern void func_00220198(int, unsigned char);
extern void func_0018D898(struct Item *, short, void *);
#define SOUND_NEXT ((u128)0x5458454e5f53UL)
#define SOUND_PREV (((u128)0x5355<<64)|(u128)0x4f49564552505f53UL)
static inline struct Values *Values(struct Settings *p) {
    return &p->values;
}
static inline int Equal(int a, int b) {
    return a == b;
}
static inline int Code(struct Item *item) {
    return item->x4;
}
static inline struct Item *Item(struct Menu *o, int index) {
    return (struct Item *)((char *)o + (index * 16 + 0x18));
}
static inline int IsType(struct Entry *entry, short code) {
    return entry->xC == code;
}
static inline unsigned char HasCode(struct Item *item, short code) {
    return item->x4 == code;
}
static inline int Is(struct Menu *o, int index, short code) {
    return IsType(&o->entries[index], code);
}

void func_0015ED18(struct Menu *o, int index, int back) {
    int channel, handle;
    unsigned char enabled;

    if (Is(o, index, 0x4b)) {
        if (back) {
            if (o->x508 >= 2) {
                o->x508--;
                func_0015F4C8(o, func_0018D860(Item(o, index)), 0x11, o->x508);
            }
        } else if (o->x508 < 15) {
            o->x508++;
            func_0015F4C8(o, func_0018D860(Item(o, index)), 0x11, o->x508);
        }
    } else if (Is(o, index, 0x4f) || Is(o, index, 0x50)) {
        unsigned char toggled;
        short newCode;
        struct Values *values = Values(&D_004225A0);
        struct Item *item = (struct Item *)((char *)o + index * 16);

        item = (struct Item *)((char *)item + 0x18);
        toggled = HasCode(item, 0x50);
        values->x27 = toggled;
        newCode = (toggled == 0) ? 0x50 : 0x4f;
        item->x4 = newCode;
        func_00201A30(Code(item) != 0x50);
        if (back) {
            func_001C57A8(D_004B2A90, SOUND_NEXT);
        } else {
            func_001C57A8(D_004B2A90, SOUND_PREV);
        }
    }

    if (Is(o, index, 0x49) || Is(o, index, 0x4a)) {
        channel = D_003F5400[0];
        handle = func_002205A0(channel);
        enabled = func_00220180(handle) ^ 1;
        func_00220198(handle, enabled);
        func_0018D898(Item(o, index), Is(o, index, 0x49) ? 0x4a : 0x49, &D_0029DE68);
        Values(&D_004225A0)->x10[channel] = enabled;
        if (back) {
            func_001C57A8(D_004B2A90, SOUND_NEXT);
        } else {
            func_001C57A8(D_004B2A90, SOUND_PREV);
        }
    } else {
        struct Item *item = (struct Item *)((char *)o + index * 16);

        item = (struct Item *)((char *)item + 0x18);
        if (Equal(Code(item), 0x9c) || Equal(Code(item), 0x9b)) {
            func_0018D898(Item(o, index), Equal(Code(item), 0x9c) ? 0x9b : 0x9c, &D_0029DE68);
            D_0039E82C[0] = Equal(Code(item), 0x9c);
        } else if (Equal(Code(item), 0x9e) || Equal(Code(item), 0x9d)) {
            func_0018D898(Item(o, index), Equal(Code(item), 0x9e) ? 0x9d : 0x9e, &D_0029DE68);
            D_0039E830[0] = Equal(Code(item), 0x9e);
        }
    }
}
