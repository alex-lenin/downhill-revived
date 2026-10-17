/* UNIT 0x001EAAD0 0x001EABE0 */
struct Info {
    char pad0[0x10];
    unsigned char x10;
    char pad11[3];
    unsigned short x14;
};
struct Player {
    char pad0[0x40C0];
    char x40C0[0x2550];
    char x6610[0x1344];
    struct Info *x7954;
    char pad7958[4];
    float x795C;
    char pad7960[0x28];
    unsigned int x7988;
};
struct Obj {
    struct Player *x0;
    char pad4[0x20];
    float x24;
};
struct Slot { char pad[0x1380]; };
struct Table {
    char pad0[0x150];
    struct Slot slots[4];
};

extern struct Table D_003B2270;
extern float func_001E51B8(void);
extern float func_001385A8(void *);
extern void func_001E63F0(void *, float);
extern void func_00179108(struct Slot *, int, int);

static __inline__ int IsActive(struct Player *player) {
    struct Info *info = player->x7954;
    unsigned char active = 0;
    float speed;
    float magnitude;

    if (info != 0 && ((info->x14 >> 1) & 1)) {
        speed = player->x795C;
        /* Zero-byte labels (cf. src/unsorted/001100D0.c, 00135478.c): scheduling
           barriers only, they emit no bytes. The first gives magnitude its own
           register instead of tying abs.s to the load (retail: abs.s $f1,$f0);
           the second keeps the x10 load below abs.s. */
        __asm__ volatile ("L1EAAD0_a:" : "=f"(magnitude));
        magnitude = __builtin_fabsf(speed);
        __asm__ volatile ("L1EAAD0_b:");
        if (magnitude < info->x10 * 3) active = 1;
    }
    return active;
}

void func_001EAAD0(struct Obj *obj) {
    struct Player *player = obj->x0;
    float step;
    struct Table *table;
    unsigned int slot;
    int value;

    if ((unsigned char)IsActive(player)) {
        step = func_001E51B8();
        step *= func_001385A8(player->x40C0);
        obj->x24 += step;
        func_001E63F0(player->x6610, step);
        slot = player->x7988;
        table = &D_003B2270;
        value = obj->x24;
        if (slot < 4) {
            func_00179108(&table->slots[slot], 0x2F, value);
        }
    } else {
        obj->x24 = 1.0f;
    }
}
