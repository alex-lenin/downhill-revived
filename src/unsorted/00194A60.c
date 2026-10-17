/* UNIT 0x00194A60 0x00194A90 */
typedef struct {
    char x0[0xC];
} Item;

typedef struct {
    char pad0[0x18];
    Item items[4];
    char pad48[0x8];
} Group;

static __inline__ Item *GetItem(Group *g, int item) {
    return &g->items[item];
}

Item *func_00194A60(char *owner, int item, int group) {
    return GetItem((Group *)(owner + group * 0x50 + 0x11948), item);
}
